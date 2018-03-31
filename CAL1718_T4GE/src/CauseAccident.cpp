#include "FunctionsPrototypes.h"

//////////////////////////
// Functions Prototypes //
//////////////////////////

void causeIntersectionAccident();
void causeRoadAccident();

void causeAccident() {

	system("cls");

	string option;
	int value {};

	do {
		cout << "Cause accident" << endl << endl;

		cout << "1 - Intersection" << endl;
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

	for(unsigned int i = 0; i < myGraph.findVertex(node)->getAdj().size(); i++) {
		int destinationNode = myGraph.findVertex(node)->getAdj().at(i).getDest()->getInfo();
		cout << destinationNode << endl;
		string edgeID = "" + to_string(node) + "0" + to_string(destinationNode);
		myGraph.findVertex(node)->removeEdge(myGraph.findVertex(destinationNode));
		i--;
		gv->setEdgeColor(stoi(edgeID),"red");
	}

	gv->rearrange();
	cout << endl;
	system("pause");
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

		cout << "Select destination node: ";
		cin >> option;

		if(validNumberInput(option,myGraph.getNumVertex())) {
			destinationNode = stoi(option);
			break;
		}
		else
			cout << "Invalid node (" << option << ") Try again !" << endl << endl;
	}

	string edgeID = "" + to_string(startingNode) + "0" + to_string(destinationNode);

	myGraph.findVertex(startingNode)->removeEdge(myGraph.findVertex(destinationNode));
	gv->setEdgeColor(stoi(edgeID),"red");
	gv->rearrange();

	cout << endl;
	system("pause");
}
