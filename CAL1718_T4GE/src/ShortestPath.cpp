#include "FunctionsPrototypes.h"

#include <chrono>

//////////////////////////
// Functions Prototypes //
//////////////////////////

bool checkUnreachableNodes(Vertex *origin);
void pathGraphAnimation(vector<Vertex*> path);
void pathGraphAnimationOneRoad(vector<Vertex*> path, Vertex* &current, string &name);
void pathGraphAnimationOneSubroad(vector<Vertex*> path, Vertex* &current);
void resetGraphState();

void gbfs(Vertex *origin, Vertex *destination) {
	// Perform algorithm
	microtime time;
	graph->gbfsDist(origin, destination, &time);
	cout << "Elapsed time: " << time << " microseconds." << endl;

	// Perform A* to verify it is correct path
	graph->AstarDist(origin, destination);

	// Get shortest path and animate
	vector<Vertex*> path = graph->getPath(origin, destination);
	pathGraphAnimation(path);

	// Perform A* to verify it is correct path
	graph->AstarDist(origin, destination);
	vector<Vertex*> astarpath = graph->getPath(origin, destination);

	if (equal(path.begin(), path.end(), astarpath.begin(), astarpath.end())) {
		cout << "The path found is the best path." << endl;
	} else {
		cout << "The path found is not the best path." << endl;
	}
}

void dijkstraSource(Vertex *origin, Vertex *destination) {
	// Perform algorithm
	microtime time;
	graph->dijkstraDist(origin, &time);
	cout << "Elapsed time: " << time << " microseconds." << endl;

	// Get shortest path and animate
	vector<Vertex*> path = graph->getPath(origin, destination);
	pathGraphAnimation(path);
}

void dijkstraSourceDest(Vertex *origin, Vertex *destination) {
	// Perform algorithm
	microtime time;
	graph->dijkstraDist(origin, destination, &time);
	cout << "Elapsed time: " << time << " microseconds." << endl;

	// Get shortest path and animate
	vector<Vertex*> path = graph->getPath(origin, destination);
	pathGraphAnimation(path);
}

void Astar(Vertex *origin, Vertex *destination) {
	// Perform algorithm
	microtime time;
	graph->AstarDist(origin, destination, &time);
	cout << "Elapsed time: " << time << " microseconds." << endl;

	// Get shortest path and animate
	vector<Vertex*> path = graph->getPath(origin, destination);
	pathGraphAnimation(path);
}

void dijkstraSimulation(Vertex *origin, Vertex *destination) {
	Vertex* current = origin;
	while(1) {
		// Perform algorithm
		microtime time;
		graph->dijkstraSimulation(current, destination, &time);
		cout << "Elapsed time: " << time << " microseconds." << endl;

		// Get shortest path
		vector<Vertex*> path = graph->getPath(current, destination);

		// Animate
		// Just a Road
		//pathGraphAnimationOneRoad(path, current, name);

		// Just a Subroad
		pathGraphAnimationOneSubroad(path, current);

		system("pause");

		if (current == destination) return;

		// Else move cars
		graph->generateGraphNewStatus();
	};
}

void getShortestPath() {

	int option;
	Vertex* origin = nullptr;
	Vertex* destination = nullptr;

	cout << "Get Shortest Path" << endl << endl;

	cout << "Available algorithms:" << endl;
	cout << "1 - Greedy Best-First Search <source,destination>" << endl;
	cout << "2 - Dijkstra <source>" << endl;
	cout << "3 - Dijkstra <source,destination>" << endl;
	cout << "4 - A* <source,destination>" << endl;
	cout << "5 - Dijkstra <source,destination> with simulation" << endl;
	cout << "6 - return" << endl;

	// Choose Algorithm
	option = getOption(6);
	if (option == 6) return;

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
			resetGraphState();
			return;
		}

		graph->setVertexColor(destination, "magenta");

		// Ok: so here we separate each algorithm
		switch (option) {
		case 1: // Greedy Best-First Search <source,destination>
			gbfs(origin, destination);
			break;
		case 2: // Dijkstra <source>
			dijkstraSource(origin, destination);
			break;
		case 3: // Dijkstra <source,destination>
			dijkstraSourceDest(origin, destination);
			break;
		case 4: // A* <source,destination>
			Astar(origin, destination);
			break;
		case 5: // Dijkstra <source,destination> with simulation
			dijkstraSimulation(origin, destination);
			break;
		}
	}

	// Reset and go up
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

void pathGraphAnimation(vector<Vertex*> path) {
	for(unsigned int i = 0; i < path.size(); i++) {
		graph->setVertexColor(path.at(i),VERTEX_SELECTED_COLOR);
		graph->rearrange();
		Sleep(100);
	}

	graph->setVertexColor(path.at(0),"magenta");
	graph->rearrange();
}

// A small hack to draw one road at a time. We need to return the last vertex read
void pathGraphAnimationOneRoad(vector<Vertex*> path, Vertex* &current, string &name) {
	// path.size() > 1, current == path.at(0)
	// Set previous current back to PATH color
	graph->setVertexColor(current, VERTEX_PATH_COLOR);

	// This is the road we should stay on
	Road* firstRoad = path.at(0)->findEdge(path.at(1))->getRoad();

	// Find the vertex in which we switch roads
	unsigned int threshold = path.size() - 1;
	for (unsigned int i = 1; i < path.size(); ++i) {
		Road* road = path.at(i - 1)->findEdge(path.at(i))->getRoad();
		if (road != firstRoad) {
			current = path.at(i - 1);
			threshold = i - 1;
			break;
		}
	}

	// Assign the colors to PATH vertices
	for (unsigned int i = 1; i < threshold; ++i) {
		graph->setVertexColor(path.at(i), VERTEX_PATH_COLOR);
		graph->setEdgeColor(path.at(i - 1)->findEdge(path.at(i)), EDGE_PATH_COLOR);
		graph->rearrange();
		Sleep(100);
	}

	// Assign the colors to NEXT PATH vertices
	for (unsigned int i = threshold + 1; i < path.size(); ++i) {
		graph->setVertexColor(path.at(i), VERTEX_NEXT_PATH_COLOR);
		graph->setEdgeColor(path.at(i - 1)->findEdge(path.at(i)), EDGE_NEXT_PATH_COLOR);
	}
	graph->setVertexColor(path.back(), VERTEX_SELECTED_COLOR);
	graph->rearrange();

	//cout << "Traveled road " << firstRoad->getName() << "." << endl;
}

void pathGraphAnimationOneSubroad(vector<Vertex*> path, Vertex* &current) {
	// path.size() > 1, current == path.at(0)
	// Set previous current back to PATH color
	graph->setVertexColor(current, VERTEX_PATH_COLOR);

	// Set edge from previous to new current to PATH color
	graph->setEdgeColor(current->findEdge(path.at(1)), EDGE_PATH_COLOR);

	// Set new current to path.at(1) and SELECTED color
	current = path.at(1);
	graph->setVertexColor(current, VERTEX_SELECTED_COLOR);
	Sleep(300);

	for (unsigned int i = 2; i < path.size(); ++i) {
		graph->setVertexColor(path.at(i), VERTEX_NEXT_PATH_COLOR);
		graph->setEdgeColor(path.at(i - 1)->findEdge(path.at(i)), EDGE_NEXT_PATH_COLOR);
	}
	graph->setVertexColor(path.back(), VERTEX_SELECTED_COLOR);
	graph->rearrange();
}

void resetGraphState() {
	for (auto v : graph->getAllVertexSet())
		graph->setVertexDefaultColor(v);
	graph->rearrange();
}

