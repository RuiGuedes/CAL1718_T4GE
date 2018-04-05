#include "FunctionsPrototypes.h"

//////////////////////////
// Functions Prototypes //
//////////////////////////

void causeIntersectionAccident();
void causeRoadAccident();

/*
 * Cause Accident UI Menu
 */
void causeAccident() {

	system("cls");

	string option;
	int value {};

	do {
		cout << "Cause accident" << endl << endl;

		cout << "1 - Intersection accident" << endl;
		cout << "2 - Road accident" << endl;
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
			causeIntersectionAccident();
			break;
		case 2:
			causeRoadAccident();
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

/*
 * Cause Node accident UI
 */
void causeIntersectionAccident() {

	string option;
	Vertex *v = nullptr;

	cout << "Cause an accident" << endl << endl;

	// Get node
	while(1) {

		cout << "Select a node: ";
		cin >> option;

		if(validIDInput(option)) {
			int node = stoi(option);
			v = graph->getVertex(node);
			if (v != nullptr) {
				break;
			} else {
				cout << "Invalid node (" << option << "). Try again !" << endl << endl;
			}
		}
		else
			cout << "Invalid node (" << option << "). Try again !" << endl << endl;
	}

	// Cause accident
	v->accident();
}

/*
 * Cause Edge accident UI
 */
void causeRoadAccident() {

	string option;
	Vertex *origin = nullptr;
	Vertex *destination = nullptr;

	cout << "Cause an accident" << endl << endl;

	// Get origin
	while(1) {

		cout << "Select starting node: ";
		cin >> option;

		if(validIDInput(option)) {
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
	// Set color to green:
	graph->setVertexColor(origin, VERTEX_SELECTED_COLOR);
	graph->rearrange();

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
			cout << "Invalid node (" << option << ") Try again !" << endl << endl;
	}

	// Cause accident
	origin->accidentEdge(destination);
	// Set color back to whatever it was:
	graph->setVertexDefaultColor(origin);
	graph->rearrange();
}
