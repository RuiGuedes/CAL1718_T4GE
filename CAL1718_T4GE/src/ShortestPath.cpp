#include "FunctionsPrototypes.h"

//////////////////////////
// Functions Prototypes //
//////////////////////////

vector<int> checkUnreachableNodes();
vector<int> pathGraphAnimation(vector<Vertex*> path);
void resetGraphState(vector<int> unreachableNodes,vector<int> pathIndex);

void getShortestPath() {

	string option;
	int startingNode { }, destinationNode { };
	Vertex* origin = nullptr;
	Vertex* destination= nullptr;

	cout << "Get Shortest Path" << endl << endl;

	while(1) {

		cout << "Select starting node: ";
		cin >> option;

		if(validIDInput(option,graph->getTotalVertices())) {
			startingNode = stoi(option);
			 origin = graph->findVertex(startingNode);

			if(origin != NULL) {
				graph->setVertexColor(origin,"green");
				break;
			}
			else {
				cout << "Accidented node (" << option << "). Try again !" << endl << endl;
			}
		}
		else
			cout << "Invalid node (" << option << "). Try again !" << endl << endl;
	}

	graph->dijkstraDist(origin);

	vector<int> unreachableNodes = checkUnreachableNodes();
	vector<int> pathIndex;

	if((unreachableNodes.size() + graph->getAccidentedVertexSet().size()) > (unsigned int)graph->getTotalVertices())
		cout << "There are no reachable nodes at the moment. " << endl << endl;
	else {

		while(1) {

			cout << "Select destination node: ";
			cin >> option;

			if(validIDInput(option,graph->getTotalVertices())) {
				destinationNode = stoi(option);
				destination = graph->findVertex(destinationNode);

				if(destination == NULL)
					cout << "Accidented node (" << option << ") Try again !" << endl << endl;
				else if(destination->getDist() == INF)
					cout << "Unreachable node (" << option << ") Try again !" << endl << endl;
				else if(startingNode == destinationNode)
					cout << "Same node as the one chosen to be the starting point. Try again !" << endl << endl;
				else
					break;
			}
			else
				cout << "Invalid node (" << option << ") Try again !" << endl << endl;
		}

		pathIndex = pathGraphAnimation(graph->getPath(origin,destination));
		cout << endl << "Time spent: " << destination->getDist() << " seconds" << endl << endl;
	}

	system("pause");

	resetGraphState(unreachableNodes,pathIndex);
}

vector<int> checkUnreachableNodes() {
	vector<int> unreachableNodes;

	cout << endl << "Checking for unreachable nodes ..." << endl << endl;

	for(int i = 1; i <= graph->getTotalVertices(); i++) {
		Vertex* v = graph->findVertex(i);
		if((v != NULL) && (v->getDist() == INF)) {
			graph->setVertexColor(v,"yellow");
			unreachableNodes.push_back(i);
		}
	}
	graph->rearrange();
	return unreachableNodes;
}

vector<int> pathGraphAnimation(vector<Vertex*> path) {
	vector<int> pathIndex;

	for(unsigned int i = 0; i < path.size(); i++) {
		pathIndex.push_back(path.at(i)->getID());
		graph->setVertexColor(path.at(i),"green");
		graph->rearrange();
		Sleep(500);
	}
	return pathIndex;
}

void resetGraphState(vector<int> unreachableNodes,vector<int> pathIndex) {
	for(unsigned int i = 0; i < unreachableNodes.size(); i++)
		graph->setVertexColor(graph->findVertex(unreachableNodes.at(i)),"blue");

	for(unsigned int i = 0; i < pathIndex.size(); i++)
		graph->setVertexColor(graph->findVertex(pathIndex.at(i)),"blue");

	graph->rearrange();
}

