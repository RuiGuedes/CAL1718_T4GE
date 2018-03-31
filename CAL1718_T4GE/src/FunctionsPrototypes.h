#include <iostream>
#include <cstdio>
#include "Graphviewer.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "Graph.h"

//////////////////////
// Global Variables //
//////////////////////

extern GraphViewer *gv;
extern Graph<int> myGraph;

//////////////////////////
// Functions Prototypes //
//////////////////////////

void mainMenu();
void causeAccident();
void fixAccident();

void getShortestPath();
void systemInformation();

bool validNumberInput(string input, int upperLimit);
