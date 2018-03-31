#include <iostream>
#include <cstdio>
#include "graphviewer.h"
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

vector<int> checkUnreachableNodes();
vector<int> pathGraphAnimation(vector<int> path);
void resetGraphState(vector<int> unreachableNodes,vector<int> pathIndex);
bool validNumberInput(string input, int upperLimit);
