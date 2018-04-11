#include "LoadMap.h"
#include "Graph.h"
#include "Interface.h"

#include <stdlib.h>

#define FILENAME_PREFIX "./resource/"

Graph* graph;

static void getFilename(string &filename) {
	while (1) {
		cout << "Select map file" << endl;
		cout << "Available by default:" << endl;
		cout << "     small" << endl;
		cout << "     corvo" << endl;
		cout << "     gporto" << endl;
		cout << "     newyork_large" << endl;
		cout << "     sydney" << endl;
		cout << "     vilareal" << endl << endl;;

		cout << "Filename: ";
		cin >> filename;

		filename = FILENAME_PREFIX + filename;

		if (checkFilename(filename))
			return;
		else
			cout << "Invalid filename. Try again !" << endl << endl;
	}
}

int main(int argc, char* argv[]) {
	srand(time(0));

	string filename;

	// Process args
	if (argc > 1) {
		filename = argv[1];
		if (filename == "--test") {
			getFilename(filename);
			return testNewMap(filename);
		}
		filename = FILENAME_PREFIX + filename;
		if (!checkFilename(filename)) {
			getFilename(filename);
		}
	} else {
		getFilename(filename);
	}

	cout << endl << "Loading map information ..." << endl << endl;

	if (loadMap(filename, graph) != 0) {
		system("pause");
		return 1;
	}
	mainMenu();

	delete graph;
	return 0;
}
