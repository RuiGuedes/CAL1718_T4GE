#include <chrono>
#include "Interface.h"

/////////////////////////
// Auxiliary Functions //
/////////////////////////

void colorUnreachableNodes(Vertex* origin) {
	graph->bfs(origin);

	cout << endl << "Checking for unreachable nodes ..." << endl << endl;

	for (auto v : graph->getVertexSet()) { // Non-accidented vertices
		if (v->getPath() == nullptr) {
			graph->setVertexColor(v, UNREACHABLE_COLOR);
		}
	}

	graph->setVertexColor(origin, SELECTED_COLOR);
	graph->rearrange();
	graph->clear();
}

void animateOneRoad(vector<Vertex*> path, Vertex* &current) {
	graph->setVertexColor(current, PATH_COLOR);

	// This is the road we should stay on
	Road* firstRoad = path.at(0)->findEdge(path.at(1))->getRoad();

	// Display the road we travel
	cout << "Traveling road " << firstRoad->getName() << "." << endl;

	// Find the vertex in which we switch Roads
	// If the entire path is contained in one Road, i.e. this is the
	// last road and the for cycle ends naturally, then
	// the new current is the last (destination) vertex.
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

	graph->animatePath(first, 200, PATH_COLOR, true);
	graph->animatePath(second, 15, NEXT_PATH_COLOR, true);
}

void animateOneSubroad(vector<Vertex*> path, Vertex* &current) {
	graph->setVertexColor(current, PATH_COLOR);

	// The new current is the next vertex in the path
	current = path.at(1);

	// Animate these with PATH color
	vector<Vertex*> first = graph->getPath(path.front(), path.at(1));
	// Quick-animate these with NEXT PATH color
	vector<Vertex*> second = graph->getPath(path.at(1), path.back());

	graph->animatePath(first, 200, PATH_COLOR, true);
	graph->animatePath(second, 15, NEXT_PATH_COLOR, true);
}

void resetGraphState() {
	graph->resetVertexColors();
	graph->resetEdgeColors();
	graph->rearrange();
}



///////////////////////////////////
// GUI for Individual Algorithms //
///////////////////////////////////

void gbfs(Vertex *origin, Vertex *destination) {
	// Perform algorithm
	microtime time;
	graph->gbfsDist(origin, destination, &time);
	cout << endl << "Elapsed time: " << time << " microseconds." << endl << endl;

	// Get shortest path and animate
	vector<Vertex*> path = graph->getPath(origin, destination);
	graph->animatePath(path, 200, PATH_COLOR, true);

	// Perform A* to verify it is the correct path
	graph->AstarDist(origin, destination);
	vector<Vertex*> astarpath = graph->getPath(origin, destination);

	if (equal(path.begin(), path.end(), astarpath.begin(), astarpath.end())) {
		cout << "The path found is the best path. ";
	} else {
		cout << "The path found is NOT the best path. We show the best path next." << endl << endl;
		system("pause");
		cout << endl;
		graph->animatePath(astarpath, 0, NEXT_PATH_COLOR, true);
	}
}

void dijkstraSource(Vertex *origin, Vertex *destination) {
	// Perform algorithm
	microtime time;
	graph->dijkstraDist(origin, &time);
	cout << endl << "Elapsed time: " << time << " microseconds." << endl << endl;

	// Get shortest path and animate
	vector<Vertex*> path = graph->getPath(origin, destination);
	graph->animatePath(path, 200, PATH_COLOR, true);
}

void dijkstraSourceDest(Vertex *origin, Vertex *destination) {
	// Perform algorithm
	microtime time;
	graph->dijkstraDist(origin, destination, &time);
	cout << endl << "Elapsed time: " << time << " microseconds." << endl << endl;

	// Get shortest path and animate
	vector<Vertex*> path = graph->getPath(origin, destination);
	graph->animatePath(path, 200, PATH_COLOR, true);
}

void Astar(Vertex *origin, Vertex *destination) {
	// Perform algorithm
	microtime time;
	graph->AstarDist(origin, destination, &time);
	cout << endl << "Elapsed time: " << time << " microseconds." << endl << endl;

	// Get shortest path and animate
	vector<Vertex*> path = graph->getPath(origin, destination);
	graph->animatePath(path, 200, PATH_COLOR, true);
}

void subroadSimulation(Vertex *origin, Vertex *destination) {
	Vertex* current = origin;

	graph->showEdgeSimulationLabels();
	graph->rearrange();

	while (true) {
		// Perform algorithm
		microtime time;
		graph->dijkstraSimulation(current, destination, &time);
		cout << endl << "Elapsed time: " << time << " microseconds." << endl << endl;

		// Get shortest path ...
		vector<Vertex*> path = graph->getPath(current, destination);

		// ... and animate
		// by Subroad
		animateOneSubroad(path, current);

		system("pause");
		cout << endl;
		if (current == destination) break;

		// Clear path prediction colors
		path = graph->getPath(current, destination);
		graph->clearPath(path, 0, true);

		// Else move cars
		graph->generateGraphNewStatus();
		graph->showEdgeSimulationLabels();
	};

	graph->hideAllEdgeLabels();
	graph->rearrange();
}

void roadSimulation(Vertex *origin, Vertex *destination) {
	Vertex* current = origin;

	graph->showEdgeSimulationLabels();
	graph->rearrange();

	while (true) {
		// Perform algorithm
		microtime time;
		graph->dijkstraSimulation(current, destination, &time);
		cout << endl << "Elapsed time: " << time << " microseconds." << endl << endl;

		// Get shortest path ...
		vector<Vertex*> path = graph->getPath(current, destination);

		// ... and animate
		// by Road
		animateOneRoad(path, current);

		system("pause");
		cout << endl;
		if (current == destination) break;

		// Clear path prediction colors
		path = graph->getPath(current, destination);
		graph->clearPath(path, 0, true);

		// Else move cars
		graph->generateGraphNewStatus();
		graph->showEdgeSimulationLabels();
	};

	graph->hideAllEdgeLabels();
	graph->rearrange();
}

void benchmark(Vertex *origin, Vertex *destination, int N) {
	// Perform A* to find the best path and show it,
	// and only then proceed with benchmarking
	graph->AstarDist(origin, destination);
	vector<Vertex*> path = graph->getPath(origin, destination);
	graph->animatePath(path, 0, PATH_COLOR, true);

	// Time of all N iterations
	microtime internal, external;

	cout << "===== Benchmark " << N << " Iterations =====" << endl;

	// Silent warmup
	{
		for (int i = 0; i < min(20, N / 10); ++i) {
			microtime time;
			graph->gbfsDist(origin, destination, &time);
			graph->dijkstraDist(origin, &time);
			graph->dijkstraDist(origin, destination, &time);
			graph->AstarDist(origin, destination, &time);
		}
	}

	// Benchmark Greedy Best-First Search
	{
		microtime sum = 0;
		auto start = chrono::high_resolution_clock::now();
		for (int i = 0; i < N; ++i) {
			microtime time;
			graph->gbfsDist(origin, destination, &time);
			sum += time;
		}
		auto end = chrono::high_resolution_clock::now();
		internal = sum / N;
		external = (end - start).count() / N;
	}
	cout << "--- (1) Greedy Best First Search ---" << endl;
	cout << "Internal Average Time: " << internal << " microseconds." << endl;
	cout << "External Average Time: " << external << " microseconds." << endl;

	// Benchmark Late Exit Dijkstra
	{
		microtime sum = 0;
		auto start = chrono::high_resolution_clock::now();
		for (int i = 0; i < N; ++i) {
			microtime time;
			graph->dijkstraDist(origin, &time);
			sum += time;
		}
		auto end = chrono::high_resolution_clock::now();
		internal = sum / N;
		external = (end - start).count() / N;
	}
	cout << "--- (2) Late Exit Dijkstra ---" << endl;
	cout << "Internal Average Time: " << internal << " microseconds." << endl;
	cout << "External Average Time: " << external << " microseconds." << endl;

	// Benchmark Early Exit Dijkstra
	{
		microtime sum = 0;
		auto start = chrono::high_resolution_clock::now();
		for (int i = 0; i < N; ++i) {
			microtime time;
			graph->dijkstraDist(origin, destination, &time);
			sum += time;
		}
		auto end = chrono::high_resolution_clock::now();
		internal = sum / N;
		external = (end - start).count() / N;
	}
	cout << "--- (3) Early Exit Dijkstra ---" << endl;
	cout << "Internal Average Time: " << internal << " microseconds." << endl;
	cout << "External Average Time: " << external << " microseconds." << endl;

	// Benchmark A*
	{
		microtime sum = 0;
		auto start = chrono::high_resolution_clock::now();
		for (int i = 0; i < N; ++i) {
			microtime time;
			graph->AstarDist(origin, destination, &time);
			sum += time;
		}
		auto end = chrono::high_resolution_clock::now();
		internal = sum / N;
		external = (end - start).count() / N;
	}
	cout << "--- (4) A* ---" << endl;
	cout << "Internal Average Time: " << internal << " microseconds." << endl;
	cout << "External Average Time: " << external << " microseconds." << endl << endl;
}


/////////////////////
// Algorithms Menu //
/////////////////////

void shortestPathUI() {

	system("cls");

	int option;
	Vertex* origin = nullptr;
	Vertex* destination = nullptr;

	cout << "Get Shortest Path" << endl << endl;

	cout << "Available algorithms:" << endl;
	cout << "1 - Greedy Best-First Search <source,destination>" << endl;
	cout << "2 - Dijkstra <source>" << endl;
	cout << "3 - Dijkstra <source,destination>" << endl;
	cout << "4 - A* <source,destination>" << endl;
	cout << "5 - Simulation [edge - edge]" << endl;
	cout << "6 - Simulation [road - road]" << endl;
	cout << "7 - Benchmark  [1 through 4]" << endl << endl;

	// Choose Algorithm
	option = selectOption(8);
	if (option == 8) return;

	// Choose origin
	origin = selectOriginVertex(false);
	if (origin == nullptr) return;

	// Color unreachable nodes
	colorUnreachableNodes(origin);
	graph->setVertexColor(origin, SELECTED_COLOR);

	// Select destination
	destination = selectDestinationVertex(origin, false, true);
	if (destination == nullptr) {
		resetGraphState();
		return;
	}

	graph->setVertexColor(destination, SELECTED_COLOR);

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
	case 5: // Simulation (edge by edge)
		subroadSimulation(origin, destination);
		break;
	case 6: // Simulation (road by road)
		subroadSimulation(origin, destination);
		break;
	case 7: // Benchmark 1 through 4
		int iterations = selectIterations();
		cout << endl << endl;
		if (iterations == 0) return;
		benchmark(origin, destination, iterations);
	}

	// Reset and go up
	cout << "Done." << endl << endl;
	system("pause");
	resetGraphState();
	return shortestPathUI();
}

