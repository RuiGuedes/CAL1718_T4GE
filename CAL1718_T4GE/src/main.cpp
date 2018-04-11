#include <stdlib.h>
#include <time.h>

#include "LoadMap.h"
#include "Graph.h"
#include "Interface.h"

Graph* graph;

int main(int argc, char* argv[]) {
	srand(time(0));

	ifstream file;
	string filename;

	cout << "Select map file" << endl;
	cout << "Available by default: small, corvo, gporto, newyork, newyork_large" << endl << endl;

	while(1) {
		cout << "Filename: ";
		cin >> filename;

		filename = "./resource/" + filename;

		if (checkFilename(filename))
			break;
		else
			cout << "Invalid filename. Try again !" << endl << endl;
	}

	cout << endl << "Loading map information ..." << endl << endl;

	if (argc == 1) {
		loadMap(filename, graph);
		mainMenu();
	} else {
		testNewMap(filename);
	}

	delete graph;

	return 0;
}
