#include "Interface.h"
#include "SearchAlgorithms.h"

//////////////////////
// Global variables //
//////////////////////

Road * startRoad;
Road * endRoad;
int algorithm;

//////////////////////////
// Functions Prototypes //
//////////////////////////

void evacuateClient();
void exactSearch();
void approximateSearch();


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

	option = selectOption(6);
	if (option == 7) return;

	algorithm = option;
	evacuateClient();

	return emergencyLine();

}

void evacuateClient() {

	if(algorithm == 5 || algorithm == 6)
		approximateSearch();
	else
		exactSearch();


}

void exactSearch() {

	system("cls");
	cout << "Emergency Line" << endl << endl;

	//Local variables
	int option, posMeters, direction;
	string pattern  = "";
	vector<string> availableRoads;
	map<string,Road *>::iterator it = graph->getRoadsInfo().begin();
	availableRoads.push_back("");

	cout << "Location name: ";
	cin.ignore(1000,'\n');
	getline(cin, pattern);

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

		if(result && ((startRoad == NULL) || (startRoad != endRoad)))
			availableRoads.push_back(it->first);

		it++;
	}

	for(unsigned int i = 1; i < availableRoads.size(); i++)
		cout << i << " - " << availableRoads.at(i) << endl;

	cout << endl;
	if(availableRoads.size() == 1) {
		cout << "Dont exist roads with containing exactly " << pattern << endl << endl;
		system("pause"); return;
	}

	option = selectOption(availableRoads.size() - 1);

	startRoad = graph->getRoadsInfo().find(availableRoads.at(option))->second;

	cout << endl << "Road selected: " << startRoad->getName() << endl << endl;

	approximateSearch();

	cout << "Position in meters: ";
	posMeters = selectOption(9999);

	cout << endl << endl << "Direction:" << endl;
	cout << "1 - Begin to End" << endl;
	cout << "2 - End to Begin" << endl << endl;
	direction =  selectOption(2);

	system("pause");
}

void approximateSearch() {

	int nextID = 1;
	map<int, Edge *>::iterator it = graph->getSubRoadsInfo().begin();
	vector<int> edgeIDs;
	int distance = 100;
	int sum = 0;

	cout << startRoad->getTotalDistance() << endl;

	while(it != graph->getSubRoadsInfo().end()) {


		if(startRoad->getName() == it->second->getRoad()->getName())
			edgeIDs.push_back(it->second->getID());

		it++;
	}

	sort(edgeIDs.begin(), edgeIDs.end());

	if(startRoad->isBidirectional())
		nextID = 2;

	//begin to end
	/*for(int i = 0; i < edgeIDs.size(); i += nextID) {
		sum += graph->findEdge(edgeIDs.at(i))->getDistance();

		if(distance <= sum)
		{
			graph->setVertexColor(graph->findEdge(edgeIDs.at(i))->getDest(), RED);
			graph->accidentEdge(graph->findEdge(edgeIDs.at(i + nextID)));
			break;
		}
	}*/


	//End to begin
	for(int i = edgeIDs.size() - 1; i >= 0; i -= nextID)
	{
		sum += graph->findEdge(edgeIDs.at(i))->getDistance();

		if(distance <= sum)
		{
			graph->setVertexColor(graph->findEdge(edgeIDs.at(i))->getDest(), RED);
			graph->accidentEdge(graph->findEdge(edgeIDs.at(i - nextID)));
			break;
		}
	}


	graph->update();
	cout << endl;
	system("pause");
}
