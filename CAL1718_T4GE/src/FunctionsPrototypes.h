#include <iostream>
#include <fstream>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <regex>

#include "Graph.h"

//////////////////////
// Global Variables //
//////////////////////

extern Graph* graph;

//////////////////////////
// Functions Prototypes //
//////////////////////////

void mainMenu();
void causeAccident();
void fixAccident();
void editRoadInfo();
void getShortestPath();
void systemInformation();

/*
 * @brief Checks if input is a string containing
 * a valid Vertex, Edge, or Road id -- a positive integer
 * max optional parameter is inclusive
 * @param input The user's input sequence
 * @param max Verify that stoi(input) <= max
 * @return true if valid and stoi(input) does not throw, false otherwise
 */
static bool validIDInput(string input, int max = 0) {
	static const regex reg_int("^\\s*(\\d+)[.,;]?\\s*$");
	smatch match;

	if (regex_match(input, match, reg_int)) {
		try {
			int value = stoi(input);

			if (value < 1) // reserved ids
				return false;

			if (max > 0 && value > max)
				return false;

			return true;
		}
		catch (runtime_error &e) {
			return false;
		}
	} else {
		return false;
	}
}
