#include "LoadMap.h"
#include "Graph.h"

Graph* graph;

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
*/
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

	//loadMap("../resource/small", graph);
	loadMap("./resource/small", graph);

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
	catch(...){// out_of_range ou invalid_argument
		return false;
	}
}



