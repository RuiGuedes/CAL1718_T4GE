#include <stdlib.h>
#include <time.h>

#include "FunctionsPrototypes.h"
#include "LoadMap.h"
#include "Graph.h"

Graph* graph;


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

			if (validIDInput(option,6)) {
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

int main(int argc, char* argv[]) {
	srand(time(0));

	ifstream file;
	//string filename;
	string filename = "./resource/small";

	cout << "Select map file" << endl;
	cout << "Available maps: gporto, newyork, newyork_large, small" << endl << endl;

//	while(1) {
//		cout << "Filename: ";
//		cin >> filename;
//
//		filename = "./resource/" + filename;
//
//		if (checkFilename(filename))
//			break;
//		else
//			cout << "Invalid filename. Try again !" << endl << endl;
//	}
//
//	cout << endl << "Loading map information ..." << endl << endl;

	if (argc == 1) {
		loadMap(filename, graph);
		mainMenu();
	} else {
		testNewMap(filename);
	}

	return 0;
}
