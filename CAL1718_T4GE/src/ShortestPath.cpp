#include "FunctionsPrototypes.h"

//////////////////////////
// Functions Prototypes //
//////////////////////////

vector<int> checkUnreachableNodes();
vector<int> pathGraphAnimation(vector<int> path);
void resetGraphState(vector<int> unreachableNodes,vector<int> pathIndex);

void getShortestPath() {

	string option, algorithm;
	int startingNode { }, destinationNode { };
	Vertex* origin = nullptr;
	Vertex* destination= nullptr;

	cout << "Get Shortest Path" << endl << endl;

	cout << "Available algorithms:" << endl;
	cout << "1 - Dijkstra <source>" << endl;
	cout << "2 - Dijkstra <source,destination>" << endl;
	cout << "3 - A star" << endl << endl;

	while(1) {

		cout << "Select algorithm: ";
		cin >> option;

		if(validIDInput(option,3)) {
			if(stoi(option) == 1)
				algorithm = "DijkstraSource";
			else if(stoi(option) == 2)
				algorithm = "DijkstraSourceDist";
			else
				algorithm = "A*";
			cout << endl;
			break;
		}
		else
			cout << "Unavailable algorithm (" << option << "). Try again !" << endl << endl;
	}

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

	if(algorithm == "DijkstraSource")
		graph->dijkstraDist(origin);

	vector<int> unreachableNodes = checkUnreachableNodes();
	vector<int> pathIndex;

	if((unreachableNodes.size() + 1 + graph->getAccidentedVertexSet().size()) >= (unsigned int)graph->getTotalVertices())
		cout << "There are no reachable nodes at the moment from " << origin->getID() << "." << endl << endl;
	else {

		while(1) {

			cout << "Select destination node: ";
			cin >> option;

			if(validIDInput(option,graph->getTotalVertices())) {
				destinationNode = stoi(option);
				destination = graph->findVertex(destinationNode);

				if(destination == nullptr)
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

		if(algorithm == "DijkstraSourceDist")
			graph->dijkstraDist(origin,destination);
		else if(algorithm == "A*")
			graph->AstarDist(origin,destination);

		pathIndex = graph->getPath(origin,destination);
		pathGraphAnimation(pathIndex);

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

vector<int> pathGraphAnimation(vector<int> path) {
	vector<int> pathIndex;

	for(unsigned int i = 0; i < path.size(); i++) {
		pathIndex.push_back(path.at(i));
		graph->setVertexColor(graph->findVertex(path.at(i)),"green");
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

