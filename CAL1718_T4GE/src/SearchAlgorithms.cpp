#include "SearchAlgorithms.h"

//////////////////////////////////////
// EXACT STRING MATCHING ALGORITHMS //
//////////////////////////////////////

/**
 * @brief Checks pattern existence on text using optimized naive algorithm
 *
 * @return true if exists, otherwise returns false
 */
bool naiveAlgorithm(string text, string pattern) {

	//Local variables
	int m = text.length();
	int n = pattern.length();
	bool equal = true;

	//text lenght must be greater or equal to pattern length
	if(m < n)
		return false;

	//Search text string until length left is lower than pattern length
	for(int i = 0; (m - i) >= n; i++) {

		if(text[i] == pattern[0]) {
			int tmp = i + 1;

			for(int j = 1; j < n; j++) {
				if(text[tmp] != pattern[j]) {
					equal = false;

					if(m == n)
						return 1;
					else
						break;
				}
				tmp++;
			}

			if(equal)
				return true;
		}
		else if(m == n)
			return false;
	}

	return false;
}


void computeFiniteAutomata(string pattern, int TF[][ALPHABET]) {

}

bool finiteAutomataAlgorithm(string text, string pattern) {

}

bool rabinKarpAlgorithm(string text, string pattern, int primeNumber) {

}

void computePrefixFunction(string pattern, int * prefix) {

}

bool knuthMorrisPrattAlgorithm(string text, string pattern) {

}

////////////////////////////////////////////
// APPROXIMATE STRING MATCHING ALGORITHMS //
////////////////////////////////////////////

/**
 * @brief Checks the number of positions at which the corresponding character in both strings are different
 *
 * dpMatrix[i][j-1]    // Insert
 * dpMatrix[i-1][j]    // Remove
 * dpMatrix[i-1][j-1]) // Replace
 *
 * @return number of different characters
 */
int editDistanceAlgorithm(string text, string pattern) {

	// Create a table to store results of subproblems
	int m = pattern.length();
	int n = text.length();
	int dpMatrix[m+1][n+1];		//Dynamic programming matrix


	// Fills dpMatrix in bottom up manner
	for (int i = 0; i <= m; i++) {

		for (int j = 0; j <= n; j++) {

			if (i==0)  //If first string is empty, only option is to insert all characters of second string. Minimum operations = j
				dpMatrix[i][j] = j;
			else if (j==0)	// If second string is empty, only option is to remove all characters of second string. // Minimum operations = i
				dpMatrix[i][j] = i;
			else if (pattern[i-1] == text[j-1])	// If last characters are same, ignore last char and recur for remaining string
				dpMatrix[i][j] = dpMatrix[i-1][j-1];
			else	// If last character are different, consider all possibilities and find minimum
				dpMatrix[i][j] = 1 + min(min(dpMatrix[i][j-1], dpMatrix[i-1][j]), dpMatrix[i-1][j-1]);
		}
	}

	return dpMatrix[m][n];
}


/**
 * @brief Checks the number of positions at which the corresponding character in both strings are different
 * It only works on strings with same length
 * @return number of different characters, -1 if invalid strings were passed as parameters
 */
int hammingDistanceAlgorithm(string text, string pattern) {

	int i = 0, count = 0;
	int m = text.length();

	if(text.length() != pattern.length())
		return -1;

	//Checks whole text string
	while (i < m)
	{
		if (text[i] != pattern[i])
			count++;
		i++;
	}

	return count;
}
