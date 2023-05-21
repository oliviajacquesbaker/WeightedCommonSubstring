PROBLEM STATEMENT:
Given two input strings, not necesarily of the same length, based on alphanumeric characters [A-Z], determine the best common sub-sequence.
The score that determines the best sub-sequence is defined as the sum of the weights w_l for each char in the sequence
(i.e. w_A is the weight of matching letter A) and a penalty -delta for each mismatch. Gaps are NOT allowed in the solution.

HOW TO RUN:
Either open the precreated .exe in this same folder, or rebuild the solution yourself if you wish. If so, the command is
g++ StringComparator.cpp Weighted_Common_Subsequence.cpp -o Weighted_Common_Subsequence.exe

EXTRA INFO:
The real meat and potatoes of this code is the StringComparator.cpp file; Weighted_Common_Subsequence.cpp is just 
the setup for the user to interact with the rest of the code. StringComparator.cpp is where the problem statement is solved.
