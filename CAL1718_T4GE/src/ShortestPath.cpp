#include "FunctionsPrototypes.h"

#include <chrono>

//////////////////////////
// Functions Prototypes //
//////////////////////////

bool checkUnreachableNodes(Vertex *origin);
void animateOneRoad(vector<Vertex*> path, Vertex* &current);
void animateOneSubroad(vector<Vertex*> path, Vertex* &current);
void resetGraphState();

void gbfs(Vertex *origin, Vertex *destination) {
	// Perform algorithm
	microtime time;
	graph->gbfsDist(origin, destination, &time);
	cout << "Elapsed time: " << time << " microseconds." << endl;

	// Get shortest path and animate
	vector<Vertex*> path = graph->getPath(origin, destination);
	graph->animatePath(path, 100, VERTEX_PATH_COLOR, true);

	// Perform A* to verify it is the correct path
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
	graph->animatePath(path, 100, VERTEX_PATH_COLOR, true);
}

void dijkstraSourceDest(Vertex *origin, Vertex *destination) {
	// Perform algorithm
	microtime time;
	graph->dijkstraDist(origin, destination, &time);
	cout << "Elapsed time: " << time << " microseconds." << endl;

	// Get shortest path and animate
	vector<Vertex*> path = graph->getPath(origin, destination);
	graph->animatePath(path, 100, VERTEX_PATH_COLOR, true);
}

void Astar(Vertex *origin, Vertex *destination) {
	// Perform algorithm
	microtime time;
	graph->AstarDist(origin, destination, &time);
	cout << "Elapsed time: " << time << " microseconds." << endl;

	// Get shortest path and animate
	vector<Vertex*> path = graph->getPath(origin, destination);
	graph->animatePath(path, 100, VERTEX_PATH_COLOR, true);
}

void dijkstraSimulation(Vertex *origin, Vertex *destination) {
	Vertex* current = origin;
	while (true) {
		// Perform algorithm
		microtime time;
		graph->dijkstraSimulation(current, destination, &time);
		cout << "Elapsed time: " << time << " microseconds." << endl;

		// Get shortest path ...
		vector<Vertex*> path = graph->getPath(current, destination);

		// ... and animate
		// by Road
		animateOneRoad(path, current);

		// by Subroad
		//animateOneSubroad(path, current);

		system("pause");
		if (current == destination) return;

		// Clear path prediction colors
		path = graph->getPath(current, destination);
		graph->clearPath(path, 0, true);

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
	cout << "5 - Dijkstra with simulation <source,destination>" << endl;
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

void animateOneRoad(vector<Vertex*> path, Vertex* &current) {
	graph->setVertexColor(current, VERTEX_PATH_COLOR);

	// This is the road we should stay on
	Road* firstRoad = path.at(0)->findEdge(path.at(1))->getRoad();

	// Display the road we travel
	cout << "Traveling road " << firstRoad->getName() << "." << endl;

	// Find the vertex in which we switch Roads
	// If the entire path is contained in one Road, i.e. this is the
	// last road and the for cycle ends naturally, then
	// threshold = path.size() - 1
	// and the new current is the last (destination) vertex.
	current = path.back();
	for (size_t i = 1; i < path.size(); ++i) {
		Road* road = path.at(i - 1)->findEdge(path.at(i))->getRoad();
		if (road != firstRoad) {
			current = path.at(i - 1);
			break;
		}
	}

	// Animate these with PATH color
	vector<Vertex*> first = graph->getPath(path.front(), current);
	// Quick-animate these with NEXT PATH color
	vector<Vertex*> second = graph->getPath(current, path.back());

	graph->animatePath(first, 100, VERTEX_PATH_COLOR, true);
	graph->animatePath(second, 10, VERTEX_NEXT_PATH_COLOR, true);
}

void animateOneSubroad(vector<Vertex*> path, Vertex* &current) {
	graph->setVertexColor(current, VERTEX_PATH_COLOR);

	// The new current is the next vertex in the path
	current = path.at(1);

	// Animate these with PATH color
	vector<Vertex*> first = graph->getPath(path.front(), path.at(1));
	// Quick-animate these with NEXT PATH color
	vector<Vertex*> second = graph->getPath(path.at(1), path.back());

	graph->animatePath(first, 100, VERTEX_PATH_COLOR, true);
	graph->animatePath(second, 10, VERTEX_NEXT_PATH_COLOR, true);
}

void resetGraphState() {
	graph->resetVertexColors();
	graph->resetEdgeColors();
}

