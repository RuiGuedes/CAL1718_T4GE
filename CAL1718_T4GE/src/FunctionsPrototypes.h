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
 * > The "max" optional parameter is inclusive
 * @param input The user's input sequence
 * @param max Verify that stoi(input) <= max
 * @return true if valid and stoi(input) does not throw, false otherwise
 */
bool validIDInput(string input, int max = 0);

int getOption(int MAX);

Vertex* getOriginVertex(bool maybeAccidented = true);

Vertex* getDestinationVertex(bool maybeAccidented = true, bool mustBeReachable = false);
