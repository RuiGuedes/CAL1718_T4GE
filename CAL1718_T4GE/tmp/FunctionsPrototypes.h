#include <iostream>
#include <cstdio>
#include <fstream>
#include <iterator>
#include <iostream>
#include <sstream>
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

bool validNumberInput(string input, int upperLimit);
