#include <iostream>
#include <fstream>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <regex>

#include "Graph.h"

//////////////////////////
// Functions Prototypes //
//////////////////////////

void mainMenu();
void causeAccident();
void fixAccident();
void editRoadInfo();
void shortestPathUI();
void emergencyLine();
void systemInformation();


bool validNumberInput(string input, int max = 0);

int selectIterations();

int selectOption(int max);

Vertex* selectVertex(bool maybeAccidented = true);

Vertex* selectOriginVertex(bool maybeAccidented = true);

Vertex* selectDestinationVertex(Vertex* origin, bool maybeAccidented = true, bool mustBeReachable = false);

Edge* selectEdge(bool maybeAccidented = true);
