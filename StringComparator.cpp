#include "StringComparator.h"

//default weights and penalty specifies the problem to be a simple longest common sub-sequence, where mismatches will not be included
StringComparator::StringComparator(string x, string y) {
	stringX = x;
	stringY = y;
	penalty = 10.f;
	fill(weight, weight + 26, 1.f);
}

//non default weights and penalties allow for more interesting 'approximate' best common sub-sequence
//now the 'best' is no longer automatically the longest, and some mismatches may be worth the penalty
StringComparator::StringComparator(string x, string y, float w[26], float d) {
	stringX = x;
	stringY = y;
	penalty = d;
	copy(w, w + 26, weight);
}

void StringComparator::SetNewValues(float w[26], float d) {
	penalty = d;
	copy(w, w + 26, weight);
}

//the value or score of a sub-sequence is defined as the sum of the weights w_l for each char in the sequence
//(i.e. w_A is the weight of matching letter A) and a penalty -delta for each mismatch
//this returns which of these values (w_l or -delta) should be added to the current score if the chars in question are included
float StringComparator::getValueofPair(char a, char b) {
	if (a == b) return weight[tolower(a) - 97];
	else return penalty * -1;
}

//convert a list of characters to a proper string
//during the "real" dynamic programming bit, lists are used instead of strings to get rid of linear concat times
string StringComparator::CharListToString(list<char> chars) {
	string str = "";
	for (list<char>::iterator iter = chars.begin(); iter != chars.end(); iter++) {
		str += *iter;
	}
	return str;
}

//this function returns the best (not necessarily the longest) approximate common substring using dynamic programming (bottom up approach)
pair<string, string> StringComparator::FindCommonSubsequence() {
	int xSize = stringX.length(); int ySize = stringY.length();
	//since we'll only be referencing values from the most recent pass, there's no need for an entire matrix
	vector<float> *optimumValueA = new vector<float>(ySize + 1, 0);
	vector<float> *optimumValueB = new vector<float>(ySize + 1, 0);
	vector<pair<list<char>, list<char>>>* optimumStringA = new vector<pair<list<char>, list<char>>>(ySize + 1);
	vector<pair<list<char>, list<char>>> *optimumStringB = new vector<pair<list<char>, list<char>>>(ySize + 1);
	//a couple of temp variables to allow easy swapping 
	vector<float>* tempVal;
	vector<pair<list<char>, list<char>>> *tempString;

	//when either string is of length 0, no common sub-string is possible (gaps not allowed), so start everything there
	for (int j = 0; j <= ySize; j++) {
		get<0>((*optimumStringA)[j]).push_back('\0');
		get<1>((*optimumStringA)[j]).push_back('\0');
		get<0>((*optimumStringB)[j]).push_back('\0');
		get<1>((*optimumStringB)[j]).push_back('\0');
		(*optimumValueA)[j] = 0;
		(*optimumValueB)[j] = 0;
	}
	//for all other cases, the best sub-sequence is the max between 
	//1. taking only the currently considered (i,j) by itself
	//2. taking the currently considered (i,j) together with the best sub-sequence ending at (i-1,j-1)
	pair<list<char>, list<char>> &currBestStrings = (*optimumStringA)[0];
	float maximumVal = 0.f; float pairVal = 0.f;
	int adjIndX = 0; int adjIndY = 0; float currBest = 0;
	for (int i = 1; i <= xSize; i++) {
		//swap the A&B vectors
		tempVal = optimumValueA;
		optimumValueA = optimumValueB;
		optimumValueB = tempVal;
		tempString = optimumStringA;
		optimumStringA = optimumStringB;
		optimumStringB = tempString;

		for (int j = 1; j <= ySize; j++) {
			adjIndX = i - 1; adjIndY = j - 1;
			pairVal = getValueofPair(stringX[adjIndX], stringY[adjIndY]);
			maximumVal = max(pairVal, pairVal + (*optimumValueB)[j - 1]);
			(*optimumValueA)[j] = maximumVal;

			//use of lists like this to avoid the linear time string concatenation within the double for loop
			if (maximumVal == pairVal) {
				get<0>((*optimumStringA)[j]) = { stringX[adjIndX] };
				get<1>((*optimumStringA)[j]) = { stringY[adjIndY] };
			}
			else {
				get<0>((*optimumStringA)[j]) = move(get<0>((*optimumStringB)[j - 1]));
				get<0>((*optimumStringA)[j]).push_back(stringX[adjIndX]);
				get<1>((*optimumStringA)[j]) = move(get<1>((*optimumStringB)[j - 1]));
				get<1>((*optimumStringA)[j]).push_back(stringY[adjIndY]);
			}
			if (maximumVal > currBest) {
				currBest = maximumVal;
				currBestStrings = (*optimumStringA)[j]; 
			}
		}
	}
	//now, outside the double for loops, we can afford the linear time cost to create the strings themselves from the lists
	pair<string, string> toReturn;
	toReturn = make_pair(CharListToString(get<0>(currBestStrings)), CharListToString(get<1>(currBestStrings)));
	//clean up
	delete optimumStringA; delete optimumStringB;
	delete optimumValueA; delete optimumValueB;
	//return the pair of strings, so in the case of a mismatch, both sub-sequences can be examined
	return toReturn;
}

//use stopwatch to measure the time it takes to run the above algorithm
double StringComparator::TimeToFindSubsequence(){
	chrono::steady_clock::time_point start = chrono::steady_clock::now();
	FindCommonSubsequence();
	chrono::steady_clock::time_point end = chrono::steady_clock::now();
	chrono::duration<float> timeSpent = chrono::duration_cast<chrono::duration<double>>(end - start);
	return timeSpent.count();
}

string StringComparator::GetStringX() {
	return stringX;
}

string StringComparator::GetStringY() {
	return stringY;
}

float StringComparator::GetPenalty() {
	return penalty;
}

float* StringComparator::GetWeights() {
	return weight;
}