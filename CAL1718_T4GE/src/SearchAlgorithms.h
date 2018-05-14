#include <iostream>
#include <string>

using namespace std;

#define ALPHABET 256

//////////////////////////////////////
// EXACT STRING MATCHING ALGORITHMS //
//////////////////////////////////////

bool naiveAlgorithm(string text, string pattern);
void computeFiniteAutomata(string pattern, int FT[][ALPHABET]);
bool finiteAutomataAlgorithm(string text, string pattern);
bool rabinKarpAlgorithm(string text, string pattern, int primeNumber = 101);
void computePrefixFunction(string pattern, int * prefix);
bool knuthMorrisPrattAlgorithm(string text, string pattern);

////////////////////////////////////////////
// APPROXIMATE STRING MATCHING ALGORITHMS //
////////////////////////////////////////////

int editDistanceAlgorithm(string text, string pattern);
int hammingDistanceAlgorithm(string text, string pattern);



