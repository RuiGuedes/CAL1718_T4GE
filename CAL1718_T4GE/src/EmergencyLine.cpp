#include "Interface.h"
#include "SearchAlgorithms.h"

//////////////////////
// Global variables //
//////////////////////

int algorithm;
Road * startRoad = NULL;
Road * endRoad = NULL;

//////////////////////////
// Functions Prototypes //
//////////////////////////

void evacuateClient();
Road * exactSearch(string pattern);
Road * approximateSearch(string pattern);
Vertex * selectRoad(Road *& road);
Vertex * selectNode(Road * road, int position, int direction);
void pathFinder(Vertex *origin, Vertex *destination);
void checkUnreachableNodes(Vertex* origin);
void benchmarking();

void emergencyLine() {

	system("cls");

	int option;

	cout << "Emergency Line" << endl << endl;

	cout << "###########################" << endl;
	cout << "## Exact string matching ##" << endl;
	cout << "###########################" << endl << endl;
	cout << "1 - Naíve Algorithm" << endl;
	cout << "2 - Rabin Karp Algorithm" << endl;
	cout << "3 - Finite Automata Algorithm" << endl;
	cout << "4 - Knuth Morris Pratt Algorithm" << endl << endl;
	cout << "#################################" << endl;
	cout << "## Approximate string matching ##" << endl;
	cout << "#################################" << endl << endl;
	cout << "5 - Edit Distance Algorithm" << endl;
	cout << "6 - Hamming Distance Algorithm" << endl << endl;
	cout << "##################" << endl;
	cout << "## Benchmarking ##" << endl;
	cout << "##################" << endl << endl;
	cout << "7 - Benchmark Algorithms" << endl << endl;

	option = selectOption(7);
	if (option == 8) return;

	algorithm = option;

	if(option == 7)
		benchmarking();
	else
		evacuateClient();

	return emergencyLine();

}

void evacuateClient() {

	system("cls");
	cout << "Emergency Line" << endl << endl;

	//Local variables
	Vertex * startingNode;
	Vertex * reachingNode;

	cout << "Evacuate From: " << endl << endl;

	startingNode = selectRoad(startRoad);

	if(startRoad == NULL || startingNode == NULL)
		return;
	else
		checkUnreachableNodes(startingNode);

	system("cls");
	cout << "Emergency Line" << endl << endl;
	cout << "Evacuate To" << endl << endl;

	reachingNode = selectRoad(endRoad);

	if(endRoad == NULL || reachingNode == NULL)
		return;

	system("cls");
	cout << "Emergency Line" << endl << endl;
	cout << "Evacuate From: " << startRoad->getName() << endl << endl;
	cout << "Evacuate To: " << endRoad->getName() << endl << endl;

	pathFinder(startingNode, reachingNode);

	system("pause");
	graph->resetEdgeColors();
	graph->resetVertexColors();
	graph->rearrange();
}

Road * exactSearch(string pattern) {

	//Local variables
	int option;
	vector<string> availableRoads;
	map<string,Road *>::iterator it = graph->getRoadsInfo().begin();

	//Check pattern existence
	while(it != graph->getRoadsInfo().end()) {
		bool result = false;

		switch(algorithm) {
		case 1:
			result = naiveAlgorithm(it->first, pattern);
			break;
		case 2:
			result = rabinKarpAlgorithm(it->first, pattern);
			break;
		case 3:
			result = finiteAutomataAlgorithm(it->first, pattern);
			break;
		case 4:
			result = knuthMorrisPrattAlgorithm(it->first, pattern);
			break;
		}

		if(result && ((startRoad == NULL) || (startRoad != endRoad))) {
			availableRoads.push_back(it->first);
			if(availableRoads.size() >= 20)
				break;
		}

		it++;
	}

	if(availableRoads.empty()) {
		cout << endl << "Road <" << pattern << "> unknown" << endl << endl;
		system("pause");
		return NULL;
	}

	for(unsigned int i = 0; i < availableRoads.size(); i++)
		cout << i + 1 << " - " << availableRoads.at(i) << endl;

	cout << endl;
	option = selectOption(availableRoads.size());

	if(option < 1 || option > (int)(availableRoads.size()))
		return NULL;
	else
		return graph->getRoadsInfo().find(availableRoads.at(option - 1))->second;
}

bool orderLessDiff(pair<int,string> pair1, pair<int,string> pair2) {
	return pair1.first < pair2.first;
}

Road * approximateSearch(string pattern) {

	//Local variables
	int option;
	vector<pair<int,string>> availableRoads;
	map<string,Road *>::iterator it = graph->getRoadsInfo().begin();

	//Check pattern existence
	while(it != graph->getRoadsInfo().end()) {
		int result = -1;

		if(!it->first.empty()) {
			switch(algorithm) {
			case 5:
				result = editDistanceAlgorithm(it->first, pattern);
				break;
			case 6:
				result = hammingDistanceAlgorithm(it->first, pattern);
				break;
			}

			if((startRoad == NULL) || (startRoad != endRoad)) {
				availableRoads.push_back(pair<int,string>(result,it->first));
				if(availableRoads.size() >= 20)
					break;
			}
		}

		it++;
	}

	sort(availableRoads.begin(), availableRoads.end(), orderLessDiff);

	for(unsigned int i = 0; i < availableRoads.size(); i++)
		cout << i + 1 << " - " << availableRoads.at(i).second << endl;

	cout << endl;
	option = selectOption(availableRoads.size() - 1);

	if(option < 1 || option > (int)(availableRoads.size()))
		return NULL;
	else
		return graph->getRoadsInfo().find(availableRoads.at(option - 1).second)->second;
}

Vertex * selectNode(Road * road, int position, int direction) {

	//Local variables
	int nextID = 1;
	unsigned int i;
	double totalDistance = 0;
	map<int, Edge *>::iterator it = graph->getSubRoadsInfo().begin();
	vector<int> edgeIDs;

	while(it != graph->getSubRoadsInfo().end()) {
		if(road->getName() == it->second->getRoad()->getName())
			edgeIDs.push_back(it->second->getID());

		it++;
	}

	sort(edgeIDs.begin(), edgeIDs.end());

	if(road->isBidirectional())
		nextID = 2;

	if(direction == 1) {
		for(i = 0; i < edgeIDs.size(); i += nextID) {
			totalDistance += graph->findEdge(edgeIDs.at(i))->getDistance();

			if(position <= totalDistance)
				break;
		}
	}
	else {
		for(i = edgeIDs.size() - 1; i >= 0; i -= nextID) {
			totalDistance += graph->findEdge(edgeIDs.at(i))->getDistance();

			if(position <= totalDistance)
				break;
		}
	}

	graph->setVertexColor(graph->findEdge(edgeIDs.at(i))->getDest(), SELECTED_COLOR);
	if(endRoad == NULL)
		graph->accidentEdge(graph->findEdge(edgeIDs.at(i + nextID)));
	graph->update();

	system("pause");
	return graph->findEdge(edgeIDs.at(i))->getDest();
}

Vertex * selectRoad(Road *& road) {

	//Local variables
	int posMeters, direction;
	string pattern;

	//Select road name
	cout << "Location name: ";
	cin.ignore(1000,'\n');
	getline(cin, pattern);

	if(algorithm == 5 || algorithm == 6)
		road = approximateSearch(pattern);
	else
		road = exactSearch(pattern);

	//Check road validity
	if(road == NULL)
		return NULL;

	//Select position in meters
	cout << endl << "Road <" << road->getName() << "> total distance: " << road->getTotalDistance() << " meters" << endl << endl;
	while (1) {
		string input;
		cout << "Position in meters: ";
		cin >> input;

		if (validNumberInput(input, road->getTotalDistance())) {
			posMeters = stoi(input);
			break;
		}
		else
			cout << "Unavailable position (" << input << "). Try again !" << endl << endl;
	}

	cout << endl << "Direction:" << endl;
	cout << "1 - Begin to End" << endl;
	cout << "2 - End to Begin" << endl << endl;
	direction =  selectOption(2);

	if(direction < 1 || direction > 2)
		return NULL;

	cout << endl;

	return selectNode(road, posMeters, direction);
}

void pathFinder(Vertex *origin, Vertex *destination) {
	// Perform algorithm
	microtime time;
	graph->AstarDist(origin, destination, &time);

	// Get shortest path and animate
	vector<Vertex*> path = graph->getPath(origin, destination);

	if(path.size() == 0)
		cout << "Not possible to evacuate to the location specified" << endl << endl;
	else {
		cout << "Evacuating to the location specified" << endl << endl;
		graph->animatePath(path, 200, PATH_COLOR, true);
	}

	return;
}

void checkUnreachableNodes(Vertex* origin) {
	graph->bfs(origin);

	for (auto v : graph->getVertexSet()) { // Non-accidented vertices
		if (v->getPath() == nullptr) {
			graph->setVertexColor(v, UNREACHABLE_COLOR);
		}
	}

	graph->rearrange();
	graph->clear();
}

//TODO
void benchmarking() {

	//Local variables
	int N = 20000;
	string pattern;
	map<string,Road *>::iterator it = graph->getRoadsInfo().begin();

	//Select location name
	cout << "Location name: ";
	cin.ignore(1000,'\n');
	getline(cin, pattern);

	// Time of all N iterations
	microtime external;

	cout << "===== Benchmark " << N << " Iterations =====" << endl;

	// Benchmark Naive Algorithm
	{

		auto start = chrono::high_resolution_clock::now();
		for(int i = 1; i < N; i++) {
			while(it != graph->getRoadsInfo().end()) {
				naiveAlgorithm(it->first, pattern);
				it++;
			}
		}
		auto end = chrono::high_resolution_clock::now();
		external = (end - start).count() / N;
	}

	cout << "--- (1) Naive Algorithm ---" << endl;
	cout << "External Average Time: " << external << " microseconds." << endl;

	// Benchmark Rabin Karp Algorithm
	{
		it = graph->getRoadsInfo().begin();
		auto start = chrono::high_resolution_clock::now();
		while(it != graph->getRoadsInfo().end()) {
			rabinKarpAlgorithm(it->first, pattern);
			it++;
		}
		auto end = chrono::high_resolution_clock::now();
		external = (end - start).count() / N;
	}

	cout << "--- (2) Rabin Karp Algorithm ---" << endl;
	cout << "External Average Time: " << external << " microseconds." << endl;

	// Benchmark Finite Automata Algorithm
	{
		it = graph->getRoadsInfo().begin();
		auto start = chrono::high_resolution_clock::now();
		while(it != graph->getRoadsInfo().end()) {
			finiteAutomataAlgorithm(it->first, pattern);
			it++;
		}
		auto end = chrono::high_resolution_clock::now();
		external = (end - start).count() / N;
	}

	cout << "--- (3) Finite Automata Algorithm ---" << endl;
	cout << "External Average Time: " << external << " microseconds." << endl;

	// Benchmark Knuth Morris Pratt Algorithm Algorithm
	{
		it = graph->getRoadsInfo().begin();
		auto start = chrono::high_resolution_clock::now();
		while(it != graph->getRoadsInfo().end()) {
			knuthMorrisPrattAlgorithm(it->first, pattern);
			it++;
		}
		auto end = chrono::high_resolution_clock::now();
		external = (end - start).count() / N;
	}

	cout << "--- (4) Knuth Morris Pratt Algorithm ---" << endl;
	cout << "External Average Time: " << external << " microseconds." << endl;

	// Benchmark Edit Distance Algorithm
	{
		it = graph->getRoadsInfo().begin();
		auto start = chrono::high_resolution_clock::now();
		while(it != graph->getRoadsInfo().end()) {
			editDistanceAlgorithm(it->first, pattern);
			it++;
		}
		auto end = chrono::high_resolution_clock::now();
		external = (end - start).count() / N;
	}

	cout << "--- (5) Edit Distance Algorithm ---" << endl;
	cout << "External Average Time: " << external << " microseconds." << endl;

	// Benchmark Hamming Distance Algorithm
	{
		it = graph->getRoadsInfo().begin();
		auto start = chrono::high_resolution_clock::now();
		while(it != graph->getRoadsInfo().end()) {
			hammingDistanceAlgorithm(it->first, pattern);
			it++;
		}
		auto end = chrono::high_resolution_clock::now();
		external = (end - start).count() / N;
	}

	cout << "--- (6) Hamming Distance Algorithm ---" << endl;
	cout << "External Average Time: " << external << " microseconds." << endl << endl;

	system("pause");
}
