#include "Interface.h"

//////////////////////////
// Functions Prototypes //
//////////////////////////

void editRoadInformationInterface(Edge* edge);
void addCars(Edge* edge);
void removeCars(Edge* edge);
void printEdgeInfo(Edge* edge);

void editRoadInfo() {
	Edge *edge = nullptr;

	cout << "Edit road information " << endl << endl;

	// Get origin
	edge = selectEdge(true);
	if (edge == nullptr) return;

	graph->setEdgeColor(edge, SELECTED_COLOR);
	graph->rearrange();
	editRoadInformationInterface(edge);
	graph->setEdgeDefaultColor(edge);
	graph->rearrange();
}


void editRoadInformationInterface(Edge* edge) {
	int option;

	while (1) {
		cout << "Edit road information " << endl << endl;

		cout << "1 - Add cars" << endl;
		cout << "2 - Remove cars" << endl;
		cout << "3 - Return" << endl;

		option = getOption(3);
		if (option == 3) return;

		system("cls");

		switch (option)
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

		system("cls");
	}
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

		if(validNumberInput(option, maxCap - actualCap)) {
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

		if(validNumberInput(option,actualCap)) {
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
		cout << "Road Name -> -----" << endl;
	else
		cout << "Road Name -> " << road->getName() << endl;
	cout << "Average speed -> " << edge->calculateAverageSpeed() << endl;
	cout << "Distance -> " << edge->getDistance() << " meters" << endl;
	cout << "Amount of cars -> " << edge->getActualCapacity() << " cars" << endl;
	cout << "Maximum capacity -> " << edge->getMaxCapacity() << " cars" << endl << endl;
}
