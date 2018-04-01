#include "FunctionsPrototypes.h"
#include "load_map.h"

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

int main(int argc, char* argv[]) {
	string which = argc > 1 ? argv[1] : "gporto";

	int s = load_map("./resource/" + which, gv, myGraph, false);
	if (s != 0) return -1;

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


int testNewMap(string path) {
	int status;

	status = test_load_meta("./resource/" + path);
	if (status != 0) return -1;

	status = test_load_nodes("./resource/" + path);
	if (status != 0) return -1;

	status = test_load_roads("./resource/" + path);
	if (status != 0) return -1;

	status = test_load_subroads("./resource/" + path);
	if (status != 0) return -1;

	status = test_load_map("./resource/" + path);
	if (status != 0) return -1;

	std::getchar();
	return 0;
}
