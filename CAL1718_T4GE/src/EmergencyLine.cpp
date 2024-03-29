#include "Interface.h"
#include "SearchAlgorithms.h"

using namespace std::chrono;

//////////////////////
// Global variables //
//////////////////////

int algorithm;
int accidentedEdgeID = 1;
Road * startRoad = NULL;
Road * endRoad = NULL;

//////////////////////////
// Functions Prototypes //
//////////////////////////

void evacuateClient();
Road * exactSearch(string pattern);
bool orderLessDiff(pair<int,string> pair1, pair<int,string> pair2);
Road * approximateSearch(string pattern);
Vertex * selectNode(Road * road, int position, int direction);
Vertex * selectRoad(Road *& road);
void pathFinder(Vertex *origin, Vertex *destination);
void checkUnreachableNodes(Vertex* origin);
void benchmarking(int N);

/**
 * @brief Emergency line interface that allows user to choose algorithm to be used
 * in order to help me to be evacuated
 */
void emergencyLine() {

	system("cls");
	cout << "Emergency Line" << endl << endl;

	cout << "###########################" << endl;
	cout << "## Exact string matching ##" << endl;
	cout << "###########################" << endl << endl;
	cout << "1 - Na�ve Algorithm" << endl;
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

	//Reset graph state
	startRoad = NULL; endRoad = NULL;
	graph->fixEdge(graph->findEdge(accidentedEdgeID));
	graph->resetEdgeColors();
	graph->resetVertexColors();
	graph->rearrange();

	//Stores option in global variable to be used to determine algorithm to be used in road search
	algorithm = selectOption(7);

	if (algorithm == 8)
		return;
	else if(algorithm == 7) {
		int iterations = selectIterations();
		cout << endl << endl;
		if (iterations == 0) return emergencyLine();
		benchmarking(iterations);
	}
	else
		evacuateClient();

	return emergencyLine();
}

/**
 * @brief Determines both location where user is going to be evacuated and where he
 * will be evacuated
 */
void evacuateClient() {

	//Local variables
	Vertex * startingNode;
	Vertex * reachingNode;


	//Determines evacuation source
	system("cls");
	cout << "Emergency Line" << endl << endl;
	cout << "Evacuate From: " << endl << endl;

	startingNode = selectRoad(startRoad);

	if(startingNode == NULL)
		return;
	else
		checkUnreachableNodes(startingNode);


	//Determines evacuation destiny
	system("cls");
	cout << "Emergency Line" << endl << endl;
	cout << "Evacuate To" << endl << endl;

	reachingNode = selectRoad(endRoad);

	if(reachingNode == NULL)
		return;

	system("cls");
	cout << "Emergency Line" << endl << endl;
	cout << "Evacuate From: " << startRoad->getName() << endl << endl;
	cout << "Evacuate To: " << endRoad->getName() << endl << endl;

	//Animation relative to evacuation route
	pathFinder(startingNode, reachingNode);

	system("pause");
}

/**
 * @brief Uses chosen algorithm to search pattern in all roads available
 * for the loaded map. This function uses only exact string maching algorithms
 *
 * @param pattern User input relative to his location content
 *
 * @return Returns pointer to the selected road
 */
Road * exactSearch(string pattern) {

	//Local variables
	int option;
	vector<string> availableRoads;
	map<string,Road *>::iterator it = graph->getRoadsInfo().begin();

	//Check pattern existence on all possible roads
	while(it != graph->getRoadsInfo().end()) {
		bool result = false;

		//Algorithm to be used
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
			if(!it->first.empty()) {
				availableRoads.push_back(it->first);
				if(availableRoads.size() >= 20)
					break;
			}
		}

		it++;
	}

	if(availableRoads.empty()) {
		cout << endl << "Road <" << pattern << "> is unknown" << endl << endl;
		system("pause");
		return NULL;
	}

	for(unsigned int i = 0; i < availableRoads.size(); i++)
		cout << i + 1 << " - " << availableRoads.at(i) << endl;

	cout << endl;
	option = selectOption(availableRoads.size());

	if(option > (int)(availableRoads.size()))
		return NULL;
	else
		return graph->getRoadsInfo().find(availableRoads.at(option - 1))->second;
}

/**
 * @brief Function used to sort roads display on approximateSearch function.
 * This function makes an ascendent sort using the result given by approximate
 * string matching algorithms
 *
 * @return True if pair1 < pair2. Otherwise returns false
 */
bool orderLessDiff(pair<int,string> pair1, pair<int,string> pair2) {
	return pair1.first < pair2.first;
}

/**
 * @brief Uses chosen algorithm to search pattern in all roads available
 * for the loaded map. This function uses only approximate string maching algorithms
 *
 * @param pattern User input relative to his location content
 *
 * @return Returns pointer to the selected road
 */
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

			if((result >= 0) && ((startRoad == NULL) || (startRoad != endRoad))) {
				if(!it->first.empty())
					availableRoads.push_back(pair<int,string>(result,it->first));

			}
		}

		it++;
	}

	if(availableRoads.empty()) {
		cout << endl << "Hamming Distance algorithm requires equal string lenghts which was not verified" << endl << endl;
		system("pause");
		return NULL;
	}
	else
		sort(availableRoads.begin(), availableRoads.end(), orderLessDiff);

	for(unsigned int i = 0; i < 20; i++)
		cout << i + 1 << " - " << availableRoads.at(i).second << endl;

	cout << endl;
	option = selectOption(20);

	if(option > 20)
		return NULL;
	else
		return graph->getRoadsInfo().find(availableRoads.at(option - 1).second)->second;
}

/**
 * @brief Function responsible to determine user vertex position throught
 * selected road information
 *
 * @param road Selected road
 * @param position Selected position in meters
 * @param direction Selected direction
 *
 * @return Return user vertex position
 */
Vertex * selectNode(Road * road, int position, int direction) {

	//Local variables
	vector<int> edgeIDs;
	double totalDistance = 0;
	unsigned int nextID = 1;
	int i = 0;
	map<int, Edge *>::iterator it = graph->getSubRoadsInfo().begin();

	//Stores on edgeIDS the edge id of every edge that belongs to selected road
	while(it != graph->getSubRoadsInfo().end()) {
		if(road->getName() == it->second->getRoad()->getName())
			edgeIDs.push_back(it->second->getID());

		it++;
	}

	//Ascendent sort of the road edge ids
	sort(edgeIDs.begin(), edgeIDs.end());

	//Checks if it is necessary to increment road by 2 or just 1 (default nextID)
	if(road->isBidirectional())
		nextID = 2;


	//Change graph state according to options chosen
	if(direction == 1) //Begin to end
	{
		for(i = 0; i < (int)edgeIDs.size(); i += nextID) {
			totalDistance += graph->findEdge(edgeIDs.at(i))->getDistance();

			if((position <= totalDistance)){
				graph->setVertexColor(graph->findEdge(edgeIDs.at(i))->getDest(), SELECTED_COLOR);
				if(endRoad == NULL) {
					if((i + nextID) < edgeIDs.size()) {
						accidentedEdgeID = graph->findEdge(edgeIDs.at(i + nextID))->getID();
						graph->accidentEdge(graph->findEdge(edgeIDs.at(i + nextID)));
					}
				}
				break;
			}
			else if((i + nextID) >= edgeIDs.size()) {
				i = edgeIDs.size() - 1;
				graph->setVertexColor(graph->findEdge(edgeIDs.at(i))->getDest(), SELECTED_COLOR);
				break;
			}
		}
	}
	else if(direction == 2) //End to begin
	{
		for(i = (edgeIDs.size() - 1); i >= 0; i -= nextID) {
			totalDistance += graph->findEdge(edgeIDs.at(i))->getDistance();

			if(position <= totalDistance) {
				graph->setVertexColor(graph->findEdge(edgeIDs.at(i))->getDest(), SELECTED_COLOR);
				if(endRoad == NULL) {
					if((i - nextID) >= 0) {
						accidentedEdgeID = graph->findEdge(edgeIDs.at(i - nextID))->getID();
						graph->accidentEdge(graph->findEdge(edgeIDs.at(i - nextID)));
					}
				}
				break;
			}
			else if((i - nextID) < 0) {
				i = 0;
				graph->setVertexColor(graph->findEdge(edgeIDs.at(i))->getDest(), SELECTED_COLOR);
				break;
			}
		}
	}

	graph->update();
	system("pause");
	return graph->findEdge(edgeIDs.at(i))->getDest();
}

/**
 * @brief Function responsible to determine user location and store it
 * on variable passed as parameter
 *
 * @param road Variable where will be stored road information
 *
 * @return Return the vertex where user is at
 */
Vertex * selectRoad(Road *& road) {

	//Local variables
	int posMeters, direction;
	string pattern;

	//Clear input buffer
	cin.ignore(1000,'\n');

	//Select road name
	cout << "Location name: ";
	getline(cin, pattern);

	//Perform chosen algorithm
	if(algorithm == 5 || algorithm == 6)
		road = approximateSearch(pattern);
	else
		road = exactSearch(pattern);

	//Check road validity
	if(road == NULL)
		return NULL;
	else
		cout << endl << "Road <" << road->getName() << "> total distance: " << road->getTotalDistance() << " meters" << endl << endl;


	//Select position in meters
	while (1) {
		string input;
		cout << "Position in meters: ";
		cin >> input;

		if (validNumberInput(input, (int)road->getTotalDistance())) {
			posMeters = stoi(input);
			break;
		}
		else
			cout << "Unavailable position (" << input << "). Try again !" << endl << endl;
	}

	//Select user direction
	cout << endl << "Direction:" << endl;
	cout << "1 - Begin to End" << endl;
	cout << "2 - End to Begin" << endl << endl;
	direction =  selectOption(2);

	if(direction == 3)
		return NULL;

	cout << endl;
	return selectNode(road, posMeters, direction);
}


/**
 * @brief Function responsible to execute evacuation from
 * origin to destination and animate it
 *
 * @param origin Origin vertex
 *
 * @param destination Destination vertex
 */
void pathFinder(Vertex *origin, Vertex *destination) {

	//Local variables
	microtime time;

	// Perform A* algorithm (better performance)
	graph->AstarDist(origin, destination, &time);

	// Get shortest path and animate
	vector<Vertex*> path = graph->getPath(origin, destination);

	if(path.size() == 0)
		cout << "Not possible to evacuate to the location specified" << endl << endl;
	else {
		cout << "Evacuating to the location specified ";
		graph->animatePath(path, 200, PATH_COLOR, true);
		cout << endl << endl << "Done." << endl << endl;
	}
}

/**
 * @brief After origin vertex is established this function determines
 * unreachable nodes from origin
 *
 * @param origin Starting vertex (node)
 */

void checkUnreachableNodes(Vertex* origin) {

	//Calculate possible paths
	graph->bfs(origin);

	for (auto v : graph->getVertexSet()) { // Non-accidented vertices
		if (v->getPath() == nullptr) {
			graph->setVertexColor(v, UNREACHABLE_COLOR);
		}
	}

	graph->rearrange();
	graph->clear();
}

/**
 * @brief Function used to check algorithms performance
 */
void benchmarking(int N) {

	system("cls");
	cout << "##################" << endl;
	cout << "## Benchmarking ##" << endl;
	cout << "##################" << endl << endl;

	//Local variables
	string text = "";
	string pattern;
	map<string,Road *>::iterator it = graph->getRoadsInfo().begin();

	//Select location name
	cout << "Location name: ";
	cin.ignore(1000,'\n');
	getline(cin, pattern);

	while(it != graph->getRoadsInfo().end()) {
		text += it->first + " ";
		it++;
	}

	cout << "===== Benchmark " << N << " Iterations =====" << endl;

	// Na�ve Algorithm
	{
		auto start = chrono::high_resolution_clock::now();
		for(int i = 0; i < N; i++)
			naiveAlgorithm(text,pattern);
		auto end = chrono::high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(end - start);

		cout << "--- (2) Naive Algorithm ---" << endl;
		cout << "External Average Time: " << duration.count()/N << " microseconds." << endl;
	}

	// Rabin Karp Algorithm
	{
		auto start = chrono::high_resolution_clock::now();
		for(int i = 0; i < N; i++)
			rabinKarpAlgorithm(text,pattern);
		auto end = chrono::high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(end - start);

		cout << "--- (2) Rabin Karp Algorithm ---" << endl;
		cout << "External Average Time: " << duration.count()/N << " microseconds." << endl;
	}

	// Finite Automata Algorithm
	{
		auto start = chrono::high_resolution_clock::now();
		for(int i = 0; i < N; i++)
			finiteAutomataAlgorithm(text,pattern);
		auto end = chrono::high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(end - start);

		cout << "--- (3) Finit Automata Algorithm ---" << endl;
		cout << "External Average Time: " << duration.count()/N << " microseconds." << endl;
	}

	// Knuth Morris Pratt Algorithm
	{
		auto start = chrono::high_resolution_clock::now();
		for(int i = 0; i < N; i++)
			knuthMorrisPrattAlgorithm(text,pattern);
		auto end = chrono::high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(end - start);

		cout << "--- (4) Knuth Morris Pratt Algorithm ---" << endl;
		cout << "External Average Time: " << duration.count()/N << " microseconds." << endl;
	}

	// Edit Distance Algorithm
	{
		auto start = chrono::high_resolution_clock::now();
		for(int i = 0; i < N; i++)
			editDistanceAlgorithm(text,pattern);
		auto end = chrono::high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(end - start);

		cout << "--- (5) Edit Distance Algorithm ---" << endl;
		cout << "External Average Time: " << duration.count()/N << " microseconds." << endl;
	}

	// Hamming Distance Algorithm
	{
		auto start = chrono::high_resolution_clock::now();
		for(int i = 0; i < N; i++)
			hammingDistanceAlgorithm(text,pattern);
		auto end = chrono::high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(end - start);

		cout << "--- (6) Hamming Distance Algorithm ---" << endl;
		cout << "External Average Time: " << duration.count()/N << " microseconds." << endl;
	}

	cout << endl << endl;
	system("pause");
}
