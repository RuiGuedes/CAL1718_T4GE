#include "FunctionsPrototypes.h"

//////////////////////////
// Functions Prototypes //
//////////////////////////

void editRoadInformationInterface(Edge<int> edge);
void addCars(Edge<int> edge);
void removeCars(Edge<int> edge);
void printEdgeInfo(Edge<int> edge);

//////////////////////
// Global Variables //
//////////////////////

int startingNode { };
int destinationNode { };

void editRoadInfo() {

	system("cls");

	string option;

	cout << "Edit road information " << endl << endl;

	while(1) {

		cout << "Select starting node: ";
		cin >> option;

		if(validNumberInput(option,myGraph.getNumVertex())) {
			startingNode = stoi(option);
			break;
		}
		else
			cout << "Invalid node (" << option << "). Try again !" << endl << endl;
	}

	while(1) {

		cout << endl << "Select destination node: ";
		cin >> option;

		if(validNumberInput(option,myGraph.getNumVertex())) {
			destinationNode = stoi(option);
			break;
		}
		else
			cout << "Invalid node (" << option << ") Try again !" << endl << endl;
	}

	Vertex<int> * origin = myGraph.findVertex(startingNode);
	Vertex<int> * destination = myGraph.findVertex(destinationNode);

	if(origin == NULL)
		origin = myGraph.findAccidentedVertex(startingNode);

	if(destination == NULL)
		destination =myGraph.findAccidentedVertex(destinationNode);

	Edge<int> edge = origin->getEdge(destination);

	editRoadInformationInterface(edge);
}


void editRoadInformationInterface(Edge<int> edge) {
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

			if(validNumberInput(option,3)) {
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
			addCars(edge);
			break;
		case 2:
			removeCars(edge);
			break;
		}
		if(value != 3) {
			gv->rearrange();
			cout << endl;
			system("pause");
		}
		system("cls");

	}while(value != 3);
}


void addCars(Edge<int> edge) {

	system("cls");
	cout << "Adding Cars " << endl << endl;
	printEdgeInfo(edge);

	string option;
	int actualCap = edge.getRoad()->getActualCapacity();
	int maxCap = edge.getRoad()->getMaxCapacity();

	while(1) {

		cout << "Number of cars to be added: ";
		cin >> option;

		if(validNumberInput(option, maxCap - actualCap)) {
			edge.getRoad()->setAtualCapacity(actualCap + stoi(option));
			break;
		}
		else
			cout << "Invalid number (" << option << "). Try again !" << endl << endl;
	}

	string edgeID = "" + to_string(startingNode) + "0" + to_string(destinationNode);

	string edgeLabel = "" + to_string(edge.getRoad()->getDistance()).substr(0,4);
	edgeLabel += " - " + to_string(edge.getRoad()->getActualCapacity()) + "/" + to_string(edge.getRoad()->getMaxCapacity());
	edgeLabel += " - " + to_string(edge.getRoad()->getAverageSpeed());

	gv->setEdgeLabel(stoi(edgeID),edgeLabel);
}

void removeCars(Edge<int> edge) {

	system("cls");
	cout << "Removing Cars" << endl << endl;
	printEdgeInfo(edge);

	string option;
	int actualCap = edge.getRoad()->getActualCapacity();

	while(1) {

		cout << "Number of cars to be removed: ";
		cin >> option;

		if(validNumberInput(option,actualCap)) {
			edge.getRoad()->setAtualCapacity(actualCap - stoi(option));
			break;
		}
		else
			cout << "Invalid number (" << option << "). Try again !" << endl << endl;
	}

	string edgeID = "" + to_string(startingNode) + "0" + to_string(destinationNode);

	string edgeLabel = "" + to_string(edge.getRoad()->getDistance()).substr(0,4);
	edgeLabel += " - " + to_string(edge.getRoad()->getActualCapacity()) + "/" + to_string(edge.getRoad()->getMaxCapacity());
	edgeLabel += " - " + to_string(edge.getRoad()->getAverageSpeed());

	gv->setEdgeLabel(stoi(edgeID),edgeLabel);
}

void printEdgeInfo(Edge<int> edge) {
	cout << "Road information:" << endl;
	cout << "ID -> " << edge.getRoad()->getID() << endl;
	if(edge.getRoad()->getName().empty())
		cout << "Name -> -----" << endl;
	else
		cout << "Name -> " << edge.getRoad()->getName() << endl;
	cout << "Average speed -> " << edge.getRoad()->getAverageSpeed() << endl;
	cout << "Distance -> " << edge.getRoad()->getDistance() << " meters"<< endl;
	cout << "Amount of cars -> " << edge.getRoad()->getActualCapacity() << " cars" << endl;
	cout << "Maximum capacity -> " << edge.getRoad()->getMaxCapacity() << " cars" << endl << endl;
}
