#include "FunctionsPrototypes.h"

#include <chrono>

//////////////////////////
// Functions Prototypes //
//////////////////////////

vector<int> checkUnreachableNodes();
void pathGraphAnimation(vector<Vertex*> path);
void resetGraphState();

void dijkstraSource(Vertex *origin, Vertex *destination) {
	// Perform algorithm
	chrono::duration<double> time;
	graph->dijkstraDist(origin, &time);
	cout << "Elapsed time: " << (1000 * time.count()) << "ms." << endl;

	// Get shortest path and animate
	vector<Vertex*> path = graph->getPath(origin, destination);
	pathGraphAnimation(path);
}


void dijkstraSourceDest(Vertex *origin, Vertex *destination) {
	// Perform algorithm
	chrono::duration<double> time;
	graph->dijkstraDist(origin, destination, &time);
	cout << "Elapsed time: " << (1000 * time.count()) << "ms." << endl;

	// Get shortest path and animate
	vector<Vertex*> path = graph->getPath(origin, destination);
	pathGraphAnimation(path);
}


void Astar(Vertex *origin, Vertex *destination) {
	// Perform algorithm
	chrono::duration<double> time;
	graph->AstarDist(origin, destination, &time);
	cout << "Elapsed time: " << (1000 * time.count()) << "ms." << endl;

	// Get shortest path and animate
	vector<Vertex*> path = graph->getPath(origin, destination);
	pathGraphAnimation(path);
}


void dijkstraSimulation(Vertex *origin, Vertex *destination) {

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
	return;
}


void AstarSimulation(Vertex *origin, Vertex *destination) {
	return;
}


void getShortestPath() {

	int option;
	chrono::duration<double> timeSpent;
	Vertex* origin = nullptr;
	Vertex* destination= nullptr;

	cout << "Get Shortest Path" << endl << endl;

	cout << "Available algorithms:" << endl;
	cout << "1 - Dijkstra <source>" << endl;
	cout << "2 - Dijkstra <source,destination>" << endl;
	cout << "3 - A* <source,destination>" << endl;
	cout << "4 - Dijkstra <source,destination> with simulation" << endl;
	cout << "5 - A* <source,destination> with simulation" << endl;
	cout << "6 - return" << endl;

	// Choose Algorithm
	int option = getOption(6);
	if (option == 6) return;

	// Choose origin
	origin = getOriginVertex(false);
	if (origin == nullptr) return;

	// Show unreachable nodes
	graph->bfs(origin);
	vector<Vertex*> unreachableNodes = checkUnreachableNodes();

	if((unreachableNodes.size() + 1 + graph->getAccidentedVertexSet().size()) >= (unsigned int)graph->getTotalVertices())
		cout << "There are no reachable nodes at the moment from " << origin->getID() << "." << endl << endl;
	else {

		// Select destination
		destination = getDestinationVertex(false, origin, true, false);
		if (destination == nullptr) {
			//...
			return;
		}

		// Ok: so here we separate each algorithm
		switch (option) {
		case 1: // Dijkstra <source>
			dijkstraSource(origin, destination);
			break;
		case 2: // Dijkstra <source,destination>
			dijkstraSourceDest(origin, destination);
			break;
		case 3: // A* <source,destination>
			Astar(origin, destination);
			break;
		case 4: // Dijkstra <source,destination> with simulation
			dijkstraSimulation(origin, destination);
			break;
		case 5: // A* <source,destination> with simulation
			AstarSimulation(origin, destination);
			break;
		}
	}

	// Reset
	system("pause");
	resetGraphState();
}


vector<Vertex*> checkUnreachableNodes() {
	vector<Vertex*> unreachableNodes;

	cout << endl << "Checking for unreachable nodes ..." << endl << endl;

	for(auto v : graph->getVertexSet()) { // Non-accidented vertices
		if(v->path == nullptr) {
			graph->setVertexColor(v,VERTEX_UNREACHABLE_COLOR);
			unreachableNodes.push_back(v);
		}
	}
	graph->rearrange();
	return unreachableNodes;
}

void pathGraphAnimation(vector<Vertex*> path) {
	for(unsigned int i = 0; i < path.size(); i++) {
		graph->setVertexColor(path.at(i),VERTEX_SELECTED_COLOR);
		graph->rearrange();
		Sleep(100);
	}

	Sleep(100);
	graph->setVertexColor(path.at(0),"magenta");
	graph->rearrange();
}

void resetGraphState() {
	for (auto v : graph->getAllVertexSet())
		graph->setVertexDefaultColor(v);
	graph->rearrange();
}





vector<int> checkUnreachableNodes() {
	vector<int> unreachableNodes;

	cout << endl << "Checking for unreachable nodes ..." << endl << endl;

	for(auto v : graph->getVertexSet()) {
		if(v->path == nullptr) {
			graph->setVertexColor(v,VERTEX_UNREACHABLE_COLOR);
			unreachableNodes.push_back(v);
		}
	}
	graph->rearrange();
	return unreachableNodes;
}






// Este vector<int> path podia muito bem ser um vector<Vertex*> ...
vector<int> pathGraphAnimation(vector<int> path) {
	vector<int> pathIndex;

	for(unsigned int i = 0; i < path.size(); i++) {
		if(i != 0)
			pathIndex.push_back(path.at(i));

		graph->setVertexColor(graph->findVertex(path.at(i)),VERTEX_SELECTED_COLOR);
		graph->rearrange();
		Sleep(100);
	}

	Sleep(100);
	graph->setVertexColor(graph->findVertex(path.at(0)),"magenta");
	graph->rearrange();

	return pathIndex;
}
