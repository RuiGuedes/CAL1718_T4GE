#include "FunctionsPrototypes.h"

//////////////////////////
// Functions Prototypes //
//////////////////////////

void editRoadInformationInterface(Edge* edge);
void addCars(Edge* edge);
void removeCars(Edge* edge);
void printEdgeInfo(Edge* edge);



//////////////////////
// Global Variables //
//////////////////////

int startingNode { };
int destinationNode { };

void editRoadInfo() {

	system("cls");

	string option;
	Vertex *origin { };
	Vertex *destination { };

	cout << "Edit road information " << endl << endl;

	// Get origin
	while(1) {

		cout << "Select starting node: ";
		cin >> option;

		if (validIDInput(option)) {
			int node = stoi(option);
			origin = graph->findVertex(node);
			if (origin == nullptr)
				cout << "Invalid node (" << option << "). Try again !" << endl << endl;
			else
				break;
		}
		else
			cout << "Invalid node (" << option << "). Try again !" << endl << endl;
	}

	// Get destination
	while(1) {

		cout << endl << "Select destination node: ";
		cin >> option;

		if(validIDInput(option)) {
			int node = stoi(option);
			destination = graph->findVertex(node);
			if (destination == nullptr) {
				cout << "Invalid node (" << option << "). Try again !" << endl << endl;
			}
			else if (!origin->isAdjacentTo(destination)) {
				cout << "Node " << option << " is not adjacent to " << origin->getID() << endl << endl;
			}
			else {
				break;
			}
		}
		else
			cout << "Invalid node (" << option << ") Try again !" << endl;
	}

	Edge *edge = origin->findEdge(destination);
	graph->setEdgeColor(edge, EDGE_SELECTED_COLOR);
	graph->rearrange();
	editRoadInformationInterface(edge);
	graph->setEdgeDefaultColor(edge);
	graph->rearrange();
}


void editRoadInformationInterface(Edge* edge) {
	system("cls");

	string option;
	int value {};

	do {
		cout << "Edit road information " << endl << endl;

		cout << "1 - Add cars" << endl;
		cout << "2 - Remove cars" << endl;
		cout << "3 - Return" << endl;

		while(1)
		{
			cout << endl << "Enter an option (1-3): ";
			cin >> option;

			if(validIDInput(option,3)) {
				value = stoi(option);
				break;
			}
			else
				cout << "Invalid option (" << option << "). Try again !" << endl << endl;

		};

		system("cls");

		switch (value)
		{
		case 1:
			if(edge->getActualCapacity() == edge->getMaxCapacity())
				cout << "There are no cars to be added (the edge is saturated)" << endl;
			else
				addCars(edge);
			break;
		case 2:
			if(edge->getActualCapacity() == 0)
				cout << "There are no cars to be removed (the edge is empty)" << endl;
			else
				removeCars(edge);
			break;
		}
		if(value != 3) {
			graph->rearrange();
			cout << endl;
			system("pause");
		}
		system("cls");

	}while(value != 3);
}


void addCars(Edge* edge) {

	system("cls");
	cout << "Adding Cars " << endl << endl;
	printEdgeInfo(edge);

	string option;
	int actualCap = edge->getActualCapacity();
	int maxCap = edge->getMaxCapacity();

	while(1) {

		cout << "Number of cars to be added: ";
		cin >> option;

		if(validIDInput(option, maxCap - actualCap)) {
			edge->setActualCapacity(actualCap + stoi(option)); // this should relabel the edge
			break;
		}
		else
			cout << "Invalid number (" << option << "). Try again !" << endl << endl;
	}
}

void removeCars(Edge* edge) {

	system("cls");
	cout << "Removing Cars" << endl << endl;
	printEdgeInfo(edge);

	string option;
	int actualCap = edge->getActualCapacity();

	while(1) {

		cout << "Number of cars to be removed: ";
		cin >> option;

		if(validIDInput(option,actualCap)) {
			edge->setActualCapacity(actualCap - stoi(option)); // this should relabel the edge
			break;
		}
		else
			cout << "Invalid number (" << option << "). Try again !" << endl << endl;
	}
}

void printEdgeInfo(Edge* edge) {
	Road* road = edge->getRoad();
	cout << "Edge information:" << endl;
	cout << "ID -> " << edge->getID() << endl;
	if(road->getName().empty())
		cout << "Name -> -----" << endl;
	else
		cout << "Name -> " << road->getName() << endl;
	cout << "Average speed -> " << edge->calculateAverageSpeed() << endl;
	cout << "Distance -> " << edge->getDistance() << " meters" << endl;
	cout << "Amount of cars -> " << edge->getActualCapacity() << " cars" << endl;
	cout << "Maximum capacity -> " << edge->getMaxCapacity() << " cars" << endl << endl;
}
