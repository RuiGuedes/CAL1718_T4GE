#include <iostream>
#include <cstdio>
#include "graphviewer.h"
#include <fstream>
#include <iterator>
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
void editRoadInfo();
void getShortestPath();
void systemInformation();

bool validNumberInput(string input, int upperLimit);
