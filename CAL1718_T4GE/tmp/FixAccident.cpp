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

			if (validNumberInput(option,3)) {
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

void fixIntersectionAccident() {

	string option;
	int node {};
	Vertex *origin { };

	cout << "Fix an accident" << endl << endl;

	while(1) {

		cout << "Select a node: ";
		cin >> option;

		// Se usares um regex para interrogar option em validNumberInput, garantindo que é um
		// int, esta porção de código e outras semelhantes ficam muito mais simples.
		// O regex que queres é algo do tipo: \s*(\d+)\.?\s*, ou até outros mais fortes e capazes
		// de interpretar input com pontos, virgulas etc. Se quiseres eu explico detalhadamente...
		// Um stoi à volta de um capture group (\d+) só falha por out_of_range porque o input
		// é garantidamente uma string só com algarismos 0-9.
		if(validNumberInput(option,graph->getTotalVertices() + 1000)) {
			node = stoi(option);
			//gv->setVertexColor(node,"blue");
			//origin = myGraph.findVertex(node);
			//origin->removeAccidentedEdges();
			Vertex* v = graph->getVertex(node);
			if (v/* != nullptr*/) {
				v->fix();
			} else {
				cout << "Invalid node (" << option << "). Try again !" << endl << endl;
			}
			// falta remove accidented edges, tenho de tratar disso...
			break;
		}
		else
			cout << "Invalid node (" << option << "). Try again !" << endl << endl;
	}

//	for(unsigned int i = 0; i < origin->getAdj().size(); i++) {
//		int destinationNode = origin->getAdj().at(i).getDest()->getInfo();
//		string edgeID = "" + to_string(node) + "0" + to_string(destinationNode);
//		gv->setEdgeColor(stoi(edgeID),"black");
//	}
}

void fixRoadAccident() {

	string option;
	int startingNode { }, destinationNode { };

	cout << "Cause an accident" << endl << endl;

	while(1) {

		cout << "Select starting node: ";
		cin >> option;

		if(validNumberInput(option,graph->getTotalVertices() + 1000)) {
			startingNode = stoi(option);
			break;
		}
		else
			cout << "Invalid node (" << option << "). Try again !" << endl << endl;
	}

	while(1) {

		cout << endl << "Select destination node: ";
		cin >> option;

		if(validNumberInput(option,graph->getTotalVertices() + 1000)) {
			destinationNode = stoi(option);
			break;
		}
		else
			cout << "Invalid node (" << option << ") Try again !" << endl << endl;
	}

	Vertex *origin = graph->findVertex(startingNode);

//	if(origin == nullptr)
//		origin = myGraph.findAccidentedVertex(startingNode);

	origin->fixEdge(graph->findVertex(destinationNode));
//	string edgeID = "" + to_string(startingNode) + "0" + to_string(destinationNode);
//	gv->setEdgeColor(stoi(edgeID),"black");

}
