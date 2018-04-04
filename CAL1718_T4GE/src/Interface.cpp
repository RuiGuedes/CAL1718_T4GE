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
	loadMap("./resource/newyork_large", graph);

	//mainMenu();
	getchar();
	return 0;
}
