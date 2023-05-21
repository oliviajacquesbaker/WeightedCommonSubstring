#pragma once
#include <string>
#include <vector>
#include <chrono> 
#include <algorithm> 
#include<list>

using namespace std;

class StringComparator {
	
	string stringX;
	string stringY;
	float weight[26];
	float penalty;
	float getValueofPair(char a, char b);
	string CharListToString(list<char> chars);

public: 
	StringComparator(string x, string y);
	StringComparator(string x, string y, float w[26], float d);
	void SetNewValues(float w[26], float d);
	pair<string, string> FindCommonSubsequence();
	double TimeToFindSubsequence();
	string GetStringX();
	string GetStringY();
	float GetPenalty();
	float* GetWeights();
};
