#include "FunctionsPrototypes.h"

//////////////////////////
// Functions Prototypes //
//////////////////////////

vector<int> checkUnreachableNodes();
vector<int> pathGraphAnimation(vector<int> path);
void resetGraphState(vector<int> unreachableNodes,vector<int> pathIndex);

void getShortestPath() {

	string option;
	int startingNode { }, destinationNode { };

	cout << "Get Shortest Path" << endl << endl;

	while(1) {

		cout << "Select starting node: ";
		cin >> option;

		if(validNumberInput(option,myGraph.getNumVertex())) {
			startingNode = stoi(option);

			if(myGraph.findVertex(startingNode) != NULL) {
				gv->setVertexColor(startingNode,"green");
				break;
			}
			else {
				cout << "Accidented node (" << option << "). Try again !" << endl << endl;
			}
		}
		else
			cout << "Invalid node (" << option << "). Try again !" << endl << endl;
	}

	myGraph.dijkstraShortestPath(startingNode);

	vector<int> unreachableNodes = checkUnreachableNodes();
	vector<int> pathIndex;

	if((unreachableNodes.size() + myGraph.getAccidentedVertexSet().size()) >= (unsigned int)(myGraph.getNumVertex() - 1))
		cout << "There are no reachable nodes at the moment. " << endl << endl;
	else {

		while(1) {

			cout << "Select destination node: ";
			cin >> option;

			if(validNumberInput(option,myGraph.getNumVertex())) {
				destinationNode = stoi(option);

				if(myGraph.findVertex(destinationNode) == NULL)
					cout << "Accidented node (" << option << ") Try again !" << endl << endl;
				else if(myGraph.findVertex(destinationNode)->getDist() == INF)
					cout << "Unreachable node (" << option << ") Try again !" << endl << endl;
				else if(startingNode == destinationNode)
					cout << "Same node as the one chosen to be the starting point. Try again !" << endl << endl;
				else
					break;
			}
			else
				cout << "Invalid node (" << option << ") Try again !" << endl << endl;
		}

		pathIndex = pathGraphAnimation(myGraph.getPath(startingNode,destinationNode));
		cout << endl << "Time spent: " << myGraph.findVertex(destinationNode)->getDist() << " seconds" << endl << endl;
	}

	system("pause");

	resetGraphState(unreachableNodes,pathIndex);
}

vector<int> checkUnreachableNodes() {
	vector<int> unreachableNodes;

	cout << endl << "Checking for unreachable nodes ..." << endl << endl;

	for(int i = 1; i <= myGraph.getNumVertex(); i++) {
		if((myGraph.findVertex(i) != NULL) && (myGraph.findVertex(i)->getDist() == INF)) {
			gv->setVertexColor(i,"yellow");
			unreachableNodes.push_back(i);
		}
	}
	gv->rearrange();
	return unreachableNodes;
}

vector<int> pathGraphAnimation(vector<int> path) {
	vector<int> pathIndex;

	for(unsigned int i = 0; i < path.size(); i++) {
		pathIndex.push_back(path.at(i));
		gv->setVertexColor(path.at(i),"green");
		gv->rearrange();
		Sleep(500);
	}
	return pathIndex;
}

void resetGraphState(vector<int> unreachableNodes,vector<int> pathIndex) {
	for(unsigned int i = 0; i < unreachableNodes.size(); i++)
		gv->setVertexColor(unreachableNodes.at(i),"blue");

	for(unsigned int i = 0; i < pathIndex.size(); i++)
		gv->setVertexColor(pathIndex.at(i),"blue");

	gv->rearrange();
}
