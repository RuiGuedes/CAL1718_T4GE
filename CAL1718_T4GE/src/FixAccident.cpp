#include "FunctionsPrototypes.h"

//////////////////////////
// Functions Prototypes //
//////////////////////////

void fixIntersectionAccident();
void fixRoadAccident();

/*
 * Fix Accident Menu UI
 */
void fixAccident() {

	system("cls");

	string option;
	int value {};

	do {
		cout << "Fix accident" << endl << endl;

		cout << "1 - Intersection accident" << endl;
		cout << "2 - Road accident" << endl;
		cout << "3 - Return" << endl;

		while (1)
		{
			cout << endl << "Enter an option (1-3): ";
			cin >> option;

			if (validIDInput(option,3)) {
				value = stoi(option);
				break;
			}
			else
				cout << "Invalid option (" << option << "). Try again !" << endl << endl;

		};

		system("cls");

		switch(value)
		{
		case 1:
			fixIntersectionAccident();
			break;
		case 2:
			fixRoadAccident();
			break;
		}
		if (value != 3) {
			graph->rearrange();
			cout << endl;
			system("pause");
		}
		system("cls");

	}while(value != 3);
}

/*
 * Fix Node accident UI
 */
void fixIntersectionAccident() {

	string option;
	Vertex *v = nullptr;

	cout << "Fix an accident" << endl << endl;

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

	// Fix node
	v->fix();
}

/*
 * Fix Edge accident UI
 */
void fixRoadAccident() {

	string option;
	Vertex *origin = nullptr;
	Vertex *destination = nullptr;

	cout << "Fix an accident" << endl << endl;
	// cout << "Cause an accident" << endl << endl;  ??? gralha?

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

	// Fix origin-destination
	origin->fixEdge(destination);
	graph->setVertexDefaultColor(origin);
	graph->rearrange();
}
