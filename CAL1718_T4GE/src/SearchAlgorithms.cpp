#include "SearchAlgorithms.h"

//////////////////////////////////////
// EXACT STRING MATCHING ALGORITHMS //
//////////////////////////////////////

/**
 * @brief Checks pattern existence on text using optimized naive algorithm
 *
 * @return True if pattern exists on text string, otherwise returns false
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

/**
 * @brief Builds the TF table, passed as parameter, which represents Finite Automata for a
 * given pattern
 *
 */
void computeFiniteAutomata(string pattern, int TF[][ALPHABET]) {

	//Local Variables
	int prefixFunction = 0;

	// Fill entries in first row
	for (int x =0; x < ALPHABET; x++)
		TF[0][x] = 0;


	TF[0][(int)pattern[0]] = 1;

	// Fill entries in other rows
	for (unsigned int i = 1; i <= pattern.length(); i++) {

		// Copy values from row at index lps
		for (int x = 0; x < ALPHABET; x++)
			TF[i][x] = TF[prefixFunction][x];

		// Update the entry corresponding to this character
		TF[i][(int)pattern[i]] = i + 1;


		// Update lps for next row to be filled
		if (i < pattern.length())
			prefixFunction = TF[prefixFunction][(int)pattern[i]];
	}

	return;
}

/**
 * @brief Through a computation of FA for a certain pattern searches all text
 * string and checks if pattern exists by comparing state position
 *
 * @return True if pattern exists on text string, otherwise returns false
 */
bool finiteAutomataAlgorithm(string text, string pattern) {

	//Local variables
	int N = text.length();
	int M = pattern.length();
	int j = 0;

	int TF[M+1][ALPHABET];

	computeFiniteAutomata(pattern, TF);

	//Process text over FA <TF>.
	for(int i = 0; i < N; i++) {

		j = TF[j][(int)text[i]];

		if (j == M)
			return true;
	}

	return false;
}

/**
 * @brief Checks if pattern exists on text string using hash function to determine if
 * a window of characters with same length as pattern have the same hash value. If it has
 * it compares character one by one to see if pattern really exists on text
 *
 * @return True if pattern exists on text string, otherwise returns false
 */
bool rabinKarpAlgorithm(string text, string pattern, int primeNumber) {

	//Local variables
	int N = text.length();
	int M = pattern.length();
	int j;
	int p = 0; // hash value for pattern
	int t = 0; // hash value for text
	int h = 1; // ALPHABET^(M-1) % primeNumber

	for (int i = 0; i < M-1; i++)
		h = (h*ALPHABET) % primeNumber;

	// Calculate the hash value of pattern and first window of text
	for (int i = 0; i < M; i++)	{
		t = (ALPHABET*t + text[i]) % primeNumber;
		p = (ALPHABET*p + pattern[i]) % primeNumber;
	}

	// Slide the pattern over text one by one
	for (int i = 0; i <= N - M; i++) {

		// Check the hash values of current window of text and pattern. If the hash values match then only check for characters on by one
		if (p == t) {

			for (j = 0; j < M; j++) {
				if (text[i+j] != pattern[j])
					break;
			}

			//Pattern was found on text string
			if (j == M)
				return true;
		}

		// Calculate hash value for next window of text: Remove leading digit, add trailing digit
		if ( i < N-M ) {
			t = (ALPHABET*(t - text[i]*h) + text[i+M])%primeNumber;

			// We might get negative value of t, converting it to positive
			if (t < 0)
				t = (t + primeNumber);
		}
	}

	return false;
}

/**
 * @brief Computes the prefix function, passed as parameter, by filling prefix array
 * in order to auxiliate KMP algorithm
 *
 */
void computePrefixFunction(string pattern, int * prefix) {

	//Local variables
	int M = pattern.length();
	int i = 1;
	int len = 0; // length of the previous longest prefix suffix

	//prefix[0] is always 0
	prefix[0] = 0;

	//The following loop calculates prefix[i] for i = 1 to M-1
	while (i < M)
	{
		if (pattern[i] == pattern[len]) {
			len++;
			prefix[i] = len;
			i++;
		}
		else {
			if (len != 0)	// Needs to reset len value due to different characters
				len = prefix[len-1];
			else {
				prefix[i] = 0;
				i++;
			}
		}
	}

	return;
}

/**
 * @brief Checks whether pattern exists or not on text by calculating prefix function
 * and use it´s result to search text string
 *
 * @return True if pattern exists on text string, otherwise returns false
 */
bool knuthMorrisPrattAlgorithm(string text, string pattern) {

	//Local variables
	int N = text.length();
	int M = pattern.length();
	int i = 0;  // index for txt[]
	int j = 0;  // index for pat[]


	//Create prefix array that will hold the longest prefix suffix values for pattern
	int prefix[pattern.length()];

	//Preprocess the pattern
	computePrefixFunction(pattern, prefix);

	//While loop to check if pattern exists or not on text string
	while (i < N)
	{
		if (pattern[j] == text[i]) {
			j++;
			i++;
		}

		if (j == M)
			return true;
		else if (i < N && pattern[j] != text[i])  //Mismatch after j matches
		{
			// Do not match prefix[0..prefix[j-1]] characters, they will match anyway
			if (j != 0)
				j = prefix[j-1];
			else
				i = i+1;
		}
	}

	return false;
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
