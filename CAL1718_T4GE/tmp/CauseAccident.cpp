#include "FunctionsPrototypes.h"

//////////////////////////
// Functions Prototypes //
//////////////////////////

void causeIntersectionAccident();
void causeRoadAccident();
void updateSystem(int startingNode, int destinationNode);

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
			causeIntersectionAccident();
			break;
		case 2:
			causeRoadAccident();
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

void causeIntersectionAccident() {

	string option;
	int node {};

	cout << "Cause an accident" << endl << endl;

	while(1) {

		cout << "Select a node: ";
		cin >> option;

		if(validNumberInput(option,myGraph.getNumVertex())) {
			node = stoi(option);
			gv->setVertexColor(node,"red");
			break;
		}
		else
			cout << "Invalid node (" << option << "). Try again !" << endl << endl;
	}

	if(myGraph.findVertex(node) == NULL)
		return;

	for(unsigned int i = 0; i < myGraph.findVertex(node)->getAdj().size(); i++) {
		updateSystem(node,myGraph.findVertex(node)->getAdj().at(i).getDest()->getInfo());
		i--;
	}

	myGraph.removeVertex(node);
}

void causeRoadAccident() {

	string option;
	int startingNode { }, destinationNode { };

	cout << "Cause an accident" << endl << endl;

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

	updateSystem(startingNode,destinationNode);
}

void updateSystem(int startingNode, int destinationNode) {
	string edgeID = "" + to_string(startingNode) + "0" + to_string(destinationNode);

	Vertex<int> *origin = myGraph.findVertex(startingNode);
	Vertex<int> *destination = myGraph.findVertex(destinationNode);

	if(origin == NULL)
		origin = myGraph.findAccidentedVertex(startingNode);

	if(destination == NULL)
		destination = myGraph.findAccidentedVertex(destinationNode);

	origin->removeEdge(destination);
	gv->setEdgeColor(stoi(edgeID),"red");
}
