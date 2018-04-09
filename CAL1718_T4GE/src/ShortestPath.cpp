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
	double timeSpent { 0 };
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
		graph->dijkstraShortestPath(origin);

	vector<int> unreachableNodes = checkUnreachableNodes();
	vector<int> pathIndex;
	vector<int> reset;

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

//		if(algorithm == "DijkstraSourceDist")
//			graph->dijkstraDist(origin,destination);
		//else if(algorithm == "A*")
		//graph->AstarDist(origin,destination);

		pathIndex = graph->getPath(origin,destination);
		reset.push_back(pathIndex.at(0));
		pathIndex = pathGraphAnimation(pathIndex);
//		while(pathIndex.size() >= 1) {
//			pathIndex = pathGraphAnimation(pathIndex);
//			cout << "PATH ANIME CHECK\n\n";
//
//			if(pathIndex.size() == 0)
//				break;
//			else
//				timeSpent += graph->findVertex(pathIndex.at(0))->getDist();
//
//			cout << "TIME SPENT CHECK\n\n";
//
//			graph->generateGraphNewStatus();
//
//			cout << "NEW STATUS CHECK\n\n";
//
//			graph->dijkstraShortestPath(graph->findVertex(pathIndex.at(0)));
//
//			cout << "DJIKSTRA AGAIN CHECK\n\n";
//
//			pathIndex = graph->getPath(graph->findVertex(pathIndex.at(0)),destination);
//			reset.push_back(pathIndex.at(0));
//
//			cout << "NEXT ITERATION CHECK\n\n";
//		}
//
//		cout << endl << "Time spent: " << timeSpent << " seconds" << endl << endl;
	}

	system("pause");

	resetGraphState(unreachableNodes,reset);
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
		if(i != 0)
			pathIndex.push_back(path.at(i));

		graph->setVertexColor(graph->findVertex(path.at(i)),"green");
		graph->rearrange();
		Sleep(100);
	}

	Sleep(100);
	graph->setVertexColor(graph->findVertex(path.at(0)),"magenta");
	graph->rearrange();

	return pathIndex;
}

void resetGraphState(vector<int> unreachableNodes,vector<int> pathIndex) {
	for(unsigned int i = 0; i < unreachableNodes.size(); i++)
		graph->setVertexColor(graph->findVertex(unreachableNodes.at(i)),"blue");

	for(unsigned int i = 0; i < pathIndex.size(); i++)
		graph->setVertexColor(graph->findVertex(pathIndex.at(i)),"blue");

	graph->rearrange();
}

