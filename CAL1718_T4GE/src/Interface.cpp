#include "LoadMap.h"

Graph* graph;

int testNewMap(string path);
/*
void mainMenu() {

	system("cls");

	string option;
	int value {};

	do {
		cout << "Main Menu" << endl << endl;

		cout << "1 - Cause an accident" << endl;
		cout << "2 - Fix accident" << endl;
		cout << "3 - Edit road information" << endl;
		cout << "4 - Get shortest path" << endl;
		cout << "5 - System information" << endl;
		cout << "6 - Exit" << endl;

		while(1)
		{
			cout << endl << "Enter an option (1-6): ";
			cin >> option;

			if (validNumberInput(option,6)) {
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
			editRoadInfo();
			break;
		case 4:
			getShortestPath();
			break;
		case 5:
			systemInformation();
			break;
		}
		system("cls");

	} while(value != 6);
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
*/
int main() {

//	gv = new GraphViewer(600, 600, false);
//	gv->createWindow(600, 600);
//	gv->defineVertexColor("blue");
//	gv->defineEdgeColor("black");
//
//	myGraph.addVertex(1);	gv->addNode(1,100,300);
//	myGraph.addVertex(2);	gv->addNode(2,100,500);
//	myGraph.addVertex(3);	gv->addNode(3,100,100);
//	myGraph.addVertex(4);	gv->addNode(4,300,300);
//	myGraph.addVertex(5);	gv->addNode(5,500,500);
//	myGraph.addVertex(6);	gv->addNode(6,500,100);
//	myGraph.addVertex(7);	gv->addNode(7,500,300);
//
//	myGraph.addEdge(1, 2, new Road(1,"",2.3,3,0,20));	gv->addEdge(102,1,2,EdgeType::DIRECTED);	gv->setEdgeLabel(102, "2.3 - 0/20 - 3");
//	myGraph.addEdge(1, 4, new Road(2,"",7.6,2,0,20));	gv->addEdge(104,1,4,EdgeType::DIRECTED);	gv->setEdgeLabel(104, "7.6 - 0/20 - 2");
//	myGraph.addEdge(2, 4, new Road(3,"",3.0,1,0,20));	gv->addEdge(204,2,4,EdgeType::DIRECTED);	gv->setEdgeLabel(204, "3.0 - 0/20 - 1");
//	myGraph.addEdge(2, 5, new Road(4,"",5.4,1,0,20));	gv->addEdge(205,2,5,EdgeType::DIRECTED);	gv->setEdgeLabel(205, "5.4 - 0/20 - 1");
//	myGraph.addEdge(3, 1, new Road(5,"",2.2,3,0,20));	gv->addEdge(301,3,1,EdgeType::DIRECTED);	gv->setEdgeLabel(301, "2.2 - 0/20 - 3");
//	myGraph.addEdge(3, 6, new Road(6,"",5.1,1,0,20));	gv->addEdge(306,3,6,EdgeType::DIRECTED);	gv->setEdgeLabel(306, "5.1 - 0/20 - 1");
//	myGraph.addEdge(4, 3, new Road(7,"",1.9,2,0,20));	gv->addEdge(403,4,3,EdgeType::DIRECTED);	gv->setEdgeLabel(403, "1.9 - 0/20 - 2");
//	myGraph.addEdge(4, 5, new Road(8,"",1.6,4,0,20));	gv->addEdge(405,4,5,EdgeType::DIRECTED);	gv->setEdgeLabel(405, "1.6 - 0/20 - 4");
//	myGraph.addEdge(4, 6, new Road(9,"",6.2,1,0,20));	gv->addEdge(406,4,6,EdgeType::DIRECTED);	gv->setEdgeLabel(406, "6.2 - 0/20 - 1");
//	myGraph.addEdge(4, 7, new Road(10,"",4.3,2,0,20));	gv->addEdge(407,4,7,EdgeType::DIRECTED);	gv->setEdgeLabel(407, "4.3 - 0/20 - 2");
//	myGraph.addEdge(5, 7, new Road(11,"",2.1,3,0,20));	gv->addEdge(507,5,7,EdgeType::DIRECTED);	gv->setEdgeLabel(507, "2.1 - 0/20 - 3");
//	myGraph.addEdge(6, 4, new Road(12,"",3.7,2,0,20));	gv->addEdge(604,6,4,EdgeType::DIRECTED);	gv->setEdgeLabel(604, "3.7 - 0/20 - 2");
//	myGraph.addEdge(7, 6, new Road(13,"",4.0,2,0,20));	gv->addEdge(706,7,6,EdgeType::DIRECTED);	gv->setEdgeLabel(706, "4.0 - 0/20 - 2");
//
//	mainMenu();

//	ifstream file;
//	string filename;
//
//	cout << "Select map file" << endl << endl;
//
//	while(1) {
//
//		cout << "Filename: ";
//		cin >> filename;
//
//		file.open("../resource/" + filename + "_meta.txt");
//
//		if (file.is_open()) {
//			file.close();
//			break;
//		}
//		else
//			cout << "Invalid filename. Try again !" << endl << endl;
//
//	}
//
//	cout << endl << "Loading map information ..." << endl << endl;

	testNewMap("./resource/small");
	return 0;
}

/**
 * Verifica se a string number contem apenas digitos ou se contem simbolos ou caracteres nao
 * aceites para a situacao.
 * @param number string extraida da stream de input
 * @return Retorna true se number e um numero e false se number contem alguma coisa para alem de digitos.
 */
/*
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

*/


int testNewMap(string path) {
	int status;

	status = testLoadMeta(path);
	if (status != 0)
		return -1;

	cout << "Next test " << endl << endl;

	status = testLoadNodes(path);
	if (status != 0)
		return -1;

	cout << "Next test " << endl << endl;

	status = testLoadRoads(path);
	if (status != 0)
		return -1;

	cout << "Next test " << endl << endl;

	status = testLoadSubroads(path);
	if (status != 0)
		return -1;

	cout << "Next test " << endl << endl;

	status = testLoadMap(path);
	if (status != 0)
		return -1;

	getchar();
	exit(0);
}
