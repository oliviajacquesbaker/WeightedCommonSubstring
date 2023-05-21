//Olivia Jacques-Baker
//Assignment 2 - COT5405 Analysis of Algorithms
//Part 1: Weighted Approximate Common Sub-Sequence

//Problem statement:    Given two input strings, not necesarily of the same length, based on alphanumeric characters [A-Z], determine the best common sub-sequence.
//                      The score that determines the best sub-sequence is defined as the sum of the weights w_l for each char in the sequence
//                      (i.e. w_A is the weight of matching letter A) and a penalty -delta for each mismatch. Gaps are NOT allowed in the solution.

#include <iostream>
#include <sstream>
#include <iomanip>
#include <random>
#include <functional>
#include "StringComparator.h"
using namespace std;

//function declarations:
void progInteractions();
void reset();
void badResponse();
void handleInitialSetupResponses(int response);
void handleWeightInput(float* weights);
void presetWeights(float* weights);
void setPenalty(float amount, float* penalty);
void manualOrAutomatic(int response);
void getBestSubsequence();
string getRandomString(int length = 10);
void timeAnalysis(float* weight, float penalty);
void timeAnalysisSingle();
void runtimeResponse(int response);
void runtimeComplexityInteraction();
void viewInfo(StringComparator sequences);

bool manual = false; string preset = "simple";
StringComparator sequences("","");
int currInst = 0;
bool running = true;

int main() {
    while (running) {
        cout << "\n======================\n" << endl;
        cout << "Weighted approximate common subsequence - Olivia Jacques-Baker" << endl;
        cout << "This is the main menu. Follow the instructions below to create and compare strings." << endl;
        cout << "\n======================\n" << endl;

        progInteractions();
    }
    return 0;
}

//handles all interactions with the user as they create and compare strings
void progInteractions() {
    string stringX; string stringY; float penalty = 0.f; float weights[26] = { 1.f };
    float response = 0.f;
    string instructions[3] = {
        "Please choose a main operation:\n1:create strings to compare\t2: see average runtime analysis\t\t0: quit program",
        "Before creating strings to compare, please indicate whether you would like the weights and penalties to be specified or automatic:\n1: manual input for these values\n2: use simple weights (1 for each letter) and default penalty (10)\n3: use frequency based weights and manually inputed penalty\n4: use inverse-frequency based weights and manually inputed penalty\n0: quit program",
        "Please input a positive integer value that indicates the amount to be subtracted for a mismatch."
    };

    //handle responses of the interactions specified above (instructions[])
    //response 0 indicates user wishes to quit the program, any other, handle appropriately.
    while (currInst < 3) {
        cout << instructions[currInst] << endl;
        cin >> response;
        if (currInst == 2) setPenalty(response, &penalty);
        else if (response == 0) {
            running = false;
            return;
        }
        else handleInitialSetupResponses(static_cast<int>(response));
    }

    //weights may be a preset of all = 1, frequency based, or inverse frequency based
    //user can select one of these presets or enter each letter's weight manually
    if (manual) handleWeightInput(weights);
    else if (preset != "simple") presetWeights(weights);

    cout << "Please now enter the first string you would like to compare." << endl;
    cin >> stringX;
    cout << "Please now enter the second string you would like to compare." << endl;
    cin >> stringY;

    if(manual || preset != "simple") sequences = StringComparator(stringX, stringY, weights, penalty);
    else  sequences = StringComparator(stringX, stringY);

    //this loop marks where the string comparator object has been created and the user can now interact with it
    while (true) {
        cout << "Please select an action:\n1: find the best common sub-sequence\t\t2: view the strings, weight, and penalty information\n3: get runtimes \t4: return to main menu\t0: quit program" << endl;
        cin >> response;
        if (response == 1) getBestSubsequence();
        else if (response == 2) viewInfo(sequences);
        else if (response == 3) timeAnalysisSingle();
        else if (response == 4) {
            reset();
            break;
        }
        else if (response == 0) {
            running = false;
            return;
        }
        else badResponse();
    }
}

//reset globals before returning to the main menu
void reset() {
    manual = false;
    preset = "simple";
    currInst = 0;
}

//used whenever user input is either not a specified option for selecting choices
//or when user input is outside the specified range for a variable they are manually entering
void badResponse() {
    cout << "Please provide input within the bounds of the instructions." << endl;
}

//passes user response along to appropriate handler
//the case of response == 0 (user request to quit program) is handled before this function gets called so not checked here
void handleInitialSetupResponses(int response) {
    if (currInst == 0) runtimeResponse(response);
    else if (currInst == 1) manualOrAutomatic(response);
    else badResponse();
}

//reacts to user's decision to run time complexity experiments or to manually create strings to interact with
void runtimeResponse(int response) {
    if (response == 1) currInst++;
    else if (response == 2) runtimeComplexityInteraction();
    else badResponse();
}

//sets indicators for if one of the previously described presets for weights should be used.
void manualOrAutomatic(int response) {
    if (response == 1) {
        manual = true;
        cout << "Manual input has been selected. ";
        currInst = 2;
    }
    else if (response == 2) {
        cout << "Default settings has been selected. ";
        currInst = 3;
    }
    else if (response == 3) {
        preset = "frequency";
        cout << "Frequency based weights have been selected. ";
        currInst = 2;
    }
    else if (response == 4) {
        preset = "inverse";
        cout << "Inverse-frequency based weights have been selected. ";
        currInst = 2;
    }
    else badResponse();
}

//penalties must be positive because they are subtracted from the score, not added
void setPenalty(float amount, float* penalty) {
    if (amount >= 0.f) {
        *penalty = amount;
        currInst++;
        cout << "Penalty set. ";
    }
    else badResponse();
}

//there are no restrictions on what a weight can be (if the user wished, they could in theory have negative weights)
//though "expected" results will only occur when weights are positive
void handleWeightInput(float* weights) {
    char currLetter = 'A'; float response = 0.f;
    cout << "Please now input the weight of matching the following letters:" << endl;
    for (int i = 0; i < 26; i++) {
        cout << currLetter << ": ";
        cin >> response;
        weights[i] = response;
        currLetter++;
    }
}

//weights for these presets are from the frequency of letters in english vocabulary, from https://www3.nd.edu/~busiforc/handouts/cryptography/letterfrequencies.html
void presetWeights(float* weights) {
    if (preset == "frequency") {
        float w[26] = { 43.31f, 10.56f, 23.13f, 17.25f, 56.88f,  9.24f, 12.59f, 15.31f, 38.45f,  1.00f,  5.61f, 27.98f, 15.36f, 33.92f, 36.51f, 16.14f,  1.00f, 38.64f, 29.23f, 35.43f, 18.51f,  5.13f,  6.57f,  1.48f,  9.06f,  1.39f };
        copy(w, w + 26, weights);
    }
    else if (preset == "inverse") {
        float w[26] = { 1/43.31f, 1/10.56f, 1/23.13f, 1/17.25f, 1/56.88f,  1/9.24f, 1/12.59f, 1/15.31f, 1/38.45f,  1/1.00f,  1/5.61f, 1/27.98f, 1/15.36f, 1/33.92f, 1/36.51f, 1/16.14f,  1/1.00f, 1/38.64f, 1/29.23f, 1/35.43f, 1/18.51f,  1/5.13f,  1/6.57f,  1/1.48f,  1/9.06f,  1/1.39f };
        copy(w, w + 26, weights);
    }
}

//show user all previously set information for their current string comparator object
void viewInfo(StringComparator sequences) {
    cout << "String 1: " << sequences.GetStringX() << "\t\tString 2: " << sequences.GetStringY() << endl;
    cout << "Penalty for mismatches: -" << sequences.GetPenalty() << endl;
    char currLetter = 'A'; 
    cout << "Weight of matching the following letters:" << endl;
    for (int i = 0; i < 26; i++) {
        cout << currLetter << ": " << sequences.GetWeights()[i] << endl;
        currLetter++;
    }
}

//used for the automatic time analysis so many test cases can be generated quickly.
string getRandomString(int length) {
    string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string toReturn = ""; 
    default_random_engine generator;
    uniform_int_distribution<int> distributionInt(1, alphabet.length());
    auto randInt = bind(distributionInt, generator);
    for (int i = 0; i < length; i++) {
        toReturn += alphabet[randInt()];
    }
    return toReturn;
}

//automatic time analysis - used to generate data that can be used to 
//experiementally prove the run time O(m*n) for the dynamic programming algorithm in StringComparator.cpp
void timeAnalysis(float* weights, float penalty) {
    bool simple = true;
    for (int i = 0; i < 26; i++) {
        if (weights[i] != 1) {
            simple = false;
            break;
        }
    }
    string weightIndication = (simple) ? "simple" : "frequency-based";
    cout << "======================" << endl;
    cout << "Timing finding common subsequences in strings up to size 10,000 characters each with " << weightIndication <<" weights and a penalty of -" <<penalty<< " for a mismatch, please wait..." << endl;
    cout << "======================" << endl;
    double time = 0; int numChars;
    StringComparator sequences("", "");
    string stringX; string stringY;
    for (int i = 0; i <= 5000; i += 100) {
        numChars = i;
        cout << "Time to find common subsequence for strings of size " << numChars << " characters: " << endl;
        for (int j = 0; j < 10; j++) {
            stringX = getRandomString(numChars);
            stringY = getRandomString(numChars);
            sequences = StringComparator(stringX, stringY, weights, penalty);
            time = sequences.TimeToFindSubsequence();
            cout << fixed;
            cout << setprecision(9);
            cout << time << endl;
        }
        cout << "Complete. User is reccommended to take the median of the above times. Moving on to next string size..." << endl;
    }
}

//manual (single) time analysis
//allows the user to see how long finding the best common sub-sequence takes for their manually created strings
void timeAnalysisSingle() {
    double time = sequences.TimeToFindSubsequence();
    cout << fixed;
    cout << setprecision(9);
    cout << "Time to find best common subsequence for these strings: " << time << " seconds" << endl;
}

//if a best common sub-sequence is found, show the user how it appears in both string1 and string2 
//allows for user to see if mismatches were included
void getBestSubsequence() {
    pair<string, string> subsequence = sequences.FindCommonSubsequence();
    if (get<0>(subsequence).length() > 0 && get<0>(subsequence)[0] != '\0') cout << "The best common subsequence has been found! Here is its info:\nSubsequence from string 1: " << get<0>(subsequence) << "\nSubsequence from string 2: " << get<1>(subsequence) << endl;
    else cout << "No common subsequence was found for these strings." << endl;
}

//if the user wants to run time complexity experiments, they can choose whether to run 'standard' experiements with default weights and penalty values
//or to use the frequency-based preset. inverse-frequency preset is not used here.
void runtimeComplexityInteraction() {
    int resp;
    cout << "What settings would you like to complete the time analysis with?\n1: Simple Settings (weights all = 1, penalty = 10)\n2: Advanced Settings (weights based on letter frequency, penalty tested at different inbetween values)" << endl;
    cin >> resp;
    if (resp == 1) {
        float weight[26];
        fill(weight, weight + 26, 1.f);
        timeAnalysis(weight, 10);
        reset();
    }
    else if (resp == 2) {
        //weights are from the frequency of letters in english vocabulary, from https://www3.nd.edu/~busiforc/handouts/cryptography/letterfrequencies.html
        //                      A       B       C       D       E       F       G       H       I       J       K       L       M       N       O       P       Q       R       S       T       U       V       W       X       Y       Z
        float weights[26] = { 43.31f, 10.56f, 23.13f, 17.25f, 56.88f,  9.24f, 12.59f, 15.31f, 38.45f,  1.00f,  5.61f, 27.98f, 15.36f, 33.92f, 36.51f, 16.14f,  1.00f, 38.64f, 29.23f, 35.43f, 18.51f,  5.13f,  6.57f,  1.48f,  9.06f,  1.39f };
        float penalties[26] = { 1.00f, 5.61f, 10.56f, 17.25f, 23.13f, 27.98f, 35.43f, 38.64f, 43.31f, 56.88f };
        for (int i = 0; i < 10; i++) {
            timeAnalysis(weights, penalties[i]);
        }
        reset();
    }
    else badResponse();
}



