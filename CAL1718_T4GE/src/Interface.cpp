#include "FunctionsPrototypes.h"

GraphViewer *gv { };
Graph<int> myGraph { };

void mainMenu() {

	system("cls");

	string option;
	int value {};

	do {
		cout << "Main Menu" << endl << endl;

		cout << "1 - Cause an accident" << endl;
		cout << "2 - Fix accident" << endl;
		cout << "3 - Get shortest path" << endl;
		cout << "4 - System information" << endl;
		cout << "5 - Exit" << endl;

		while(1)
		{
			cout << endl << "Enter an option (1-5): ";
			cin >> option;

			if(validNumberInput(option,5)) {
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
			causeAccident();
			break;
		case 2:
			fixAccident();
			break;
		case 3:
			getShortestPath();
			break;
		case 4:
			systemInformation();
			break;
		}
		system("cls");

	}while(value != 5);
}

void systemInformation() {

	cout << "System information" << endl << endl;

	cout << "Node colors meaning:" << endl;
	cout << "Blue    -> Available nodes" << endl;
	cout << "Green   -> Chosen path" << endl;
	cout << "Yellow  -> Unreachable nodes from a specific starting node" << endl;
	cout << "Red     -> Accidented nodes" << endl;

	system("pause");
}

int main() {

	gv = new GraphViewer(600, 600, false);
	gv->createWindow(600, 600);
	gv->defineVertexColor("blue");
	gv->defineEdgeColor("black");

	myGraph.addVertex(1);	gv->addNode(1,100,300);
	myGraph.addVertex(2);	gv->addNode(2,100,500);
	myGraph.addVertex(3);	gv->addNode(3,100,100);
	myGraph.addVertex(4);	gv->addNode(4,300,300);
	myGraph.addVertex(5);	gv->addNode(5,500,500);
	myGraph.addVertex(6);	gv->addNode(6,500,100);
	myGraph.addVertex(7);	gv->addNode(7,500,300);

	myGraph.addEdge(1, 2, 2);	gv->addEdge(102,1,2,EdgeType::DIRECTED);	gv->setEdgeLabel(102, "2");
	myGraph.addEdge(1, 4, 7);	gv->addEdge(104,1,4,EdgeType::DIRECTED);	gv->setEdgeLabel(104, "7");
	myGraph.addEdge(2, 4, 3);	gv->addEdge(204,2,4,EdgeType::DIRECTED);	gv->setEdgeLabel(204, "3");
	myGraph.addEdge(2, 5, 5);	gv->addEdge(205,2,5,EdgeType::DIRECTED);	gv->setEdgeLabel(205, "5");
	myGraph.addEdge(3, 1, 2);	gv->addEdge(301,3,1,EdgeType::DIRECTED);	gv->setEdgeLabel(301, "2");
	myGraph.addEdge(3, 6, 5);	gv->addEdge(306,3,6,EdgeType::DIRECTED);	gv->setEdgeLabel(306, "5");
	myGraph.addEdge(4, 3, 1);	gv->addEdge(403,4,3,EdgeType::DIRECTED);	gv->setEdgeLabel(403, "1");
	myGraph.addEdge(4, 5, 1);	gv->addEdge(405,4,5,EdgeType::DIRECTED);	gv->setEdgeLabel(405, "1");
	myGraph.addEdge(4, 6, 6);	gv->addEdge(406,4,6,EdgeType::DIRECTED);	gv->setEdgeLabel(406, "6");
	myGraph.addEdge(4, 7, 4);	gv->addEdge(407,4,7,EdgeType::DIRECTED);	gv->setEdgeLabel(407, "4");
	myGraph.addEdge(5, 7, 2);	gv->addEdge(507,5,7,EdgeType::DIRECTED);	gv->setEdgeLabel(507, "2");
	myGraph.addEdge(6, 4, 3);	gv->addEdge(604,6,4,EdgeType::DIRECTED);	gv->setEdgeLabel(604, "3");
	myGraph.addEdge(7, 6, 4);	gv->addEdge(706,7,6,EdgeType::DIRECTED);	gv->setEdgeLabel(706, "4");

	mainMenu();
	return 0;
}

/**
 * Verifica se a string number contem apenas digitos ou se contem simbolos ou caracteres nao
 * aceites para a situacao.
 * @param number string extraida da stream de input
 * @return Retorna true se number e um numero e false se number contem alguma coisa para alem de digitos.
 */
bool validNumberInput(string input, int upperLimit) {
	for (unsigned int i = 0; i < input.size(); i++){
		if (!(isdigit(input.at(i))))
			return false;
	}

	try {
		int value = stoi(input);

		if( (value < 1) || (value > upperLimit))
			throw exception();

		return true;
	}
	catch(...){
		return false;
	}
}
