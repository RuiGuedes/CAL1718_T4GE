#include "FunctionsPrototypes.h"

#include <chrono>

//////////////////////////
// Functions Prototypes //
//////////////////////////

bool checkUnreachableNodes(Vertex *origin);
void pathGraphAnimation(vector<Vertex*> path, bool oneRoad = false);
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
	while(1) {
		// Perform algorithm
		chrono::duration<double> time;
		graph->dijkstraSimulation(origin, destination, &time);
		cout << "Elapsed time: " << (1000 * time.count()) << "ms." << endl;

		// Get shortest path and animate (TODO)
		vector<Vertex*> path = graph->getPath(origin, destination);
		pathGraphAnimation(path, true);

		system("pause");

		if (path.back() == destination) return;

		// Else move cars
		graph->generateGraphNewStatus();

		// Recompute unreachable nodes
		checkUnreachableNodes(path.back());
	};
}

void getShortestPath() {

	int option;
	chrono::duration<double> timeSpent;
	Vertex* origin = nullptr;
	Vertex* destination = nullptr;

	cout << "Get Shortest Path" << endl << endl;

	cout << "Available algorithms:" << endl;
	cout << "1 - Dijkstra <source>" << endl;
	cout << "2 - Dijkstra <source,destination>" << endl;
	cout << "3 - A* <source,destination>" << endl;
	cout << "4 - Dijkstra <source,destination> with simulation" << endl;
	cout << "5 - return" << endl;

	// Choose Algorithm
	option = getOption(5);
	if (option == 5) return;

	// Choose origin
	origin = getOriginVertex(false);
	if (origin == nullptr) return;
	graph->setVertexColor(origin, VERTEX_SELECTED_COLOR);

	// Find unreachable nodes
	bool reachable = checkUnreachableNodes(origin);

	if(!reachable) {
		cout << "There are no reachable nodes at the moment from " << origin->getID() << "." << endl << endl;
	}
	else {

		// Select destination
		destination = getDestinationVertex(false, true);
		if (destination == nullptr) {
			//...
			return;
		}
		//graph->setVertexColor(destination, VERTEX_SELECTED_COLOR);

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
		}
	}

	// Reset
	cout << "Done. ";
	system("pause");
	resetGraphState();
}

bool checkUnreachableNodes(Vertex* origin) {
	graph->bfs(origin);
	vector<Vertex*> unreachableNodes;
	bool reachable = false;

	cout << endl << "Checking for unreachable nodes ..." << endl << endl;

	for(auto v : graph->getVertexSet()) { // Non-accidented vertices
		if(v->getPath() == nullptr) {
			graph->setVertexColor(v,VERTEX_UNREACHABLE_COLOR);
			unreachableNodes.push_back(v);
		} else {
			reachable = true;
		}
	}

	// Color unreachable nodes
	for (auto v : unreachableNodes) {
		graph->setVertexColor(v, VERTEX_UNREACHABLE_COLOR);
	}

	graph->rearrange();
	return reachable;
}

void pathGraphAnimation(vector<Vertex*> path, bool oneRoad) {
	Road* firstRoad = path.at(0)->findEdge(path.at(1))->getRoad();
	for(unsigned int i = 0; i < path.size(); i++) {
		graph->setVertexColor(path.at(i),VERTEX_SELECTED_COLOR);
		graph->rearrange();
		if (oneRoad && i > 0) {
			Road* road = path.at(i - 1)->findEdge(path.at(i))->getRoad();
			if (road != firstRoad) break;
		}
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

