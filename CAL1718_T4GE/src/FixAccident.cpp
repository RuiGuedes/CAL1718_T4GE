#include "FunctionsPrototypes.h"

//////////////////////////
// Functions Prototypes //
//////////////////////////

void fixIntersectionAccident();
void fixRoadAccident();

void fixAccident() {

	system("cls");

	string option;
	int value {};

	do {
		cout << "Fix accident" << endl << endl;

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
			fixIntersectionAccident();
			break;
		case 2:
			fixRoadAccident();
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

void fixIntersectionAccident() {

	string option;
	int node {};
	Vertex<int> *origin { };

	cout << "Fix an accident" << endl << endl;

	while(1) {

		cout << "Select a node: ";
		cin >> option;

		if(validNumberInput(option,myGraph.getNumVertex())) {
			node = stoi(option);
			gv->setVertexColor(node,"blue");
			myGraph.removeAccidentedVertex(node);
			origin = myGraph.findVertex(node);
			origin->removeAccidentedEdges();
			break;
		}
		else
			cout << "Invalid node (" << option << "). Try again !" << endl << endl;
	}

	for(unsigned int i = 0; i < origin->getAdj().size(); i++) {
		int destinationNode = origin->getAdj().at(i).getDest()->getInfo();
		string edgeID = "" + to_string(node) + "0" + to_string(destinationNode);
		gv->setEdgeColor(stoi(edgeID),"black");
	}
}

void fixRoadAccident() {

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

	Vertex<int> *origin = myGraph.findVertex(startingNode);

	if(origin == NULL)
		origin = myGraph.findAccidentedVertex(startingNode);

	origin->removeAccidentedEdge(destinationNode);
	string edgeID = "" + to_string(startingNode) + "0" + to_string(destinationNode);
	gv->setEdgeColor(stoi(edgeID),"black");

}
