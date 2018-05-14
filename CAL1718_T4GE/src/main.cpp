#include "LoadMap.h"
#include "Graph.h"
#include "Interface.h"
#include <windows.h>

#include <stdlib.h>

#define FILENAME_PREFIX "./resource/"

string files = "Select map file\n\n"
"Map files available:\n"
"NAME         SUBROADS     NODES\n"
"fep          64           52\n"
"newyork      125          100\n"
"madrid       300          270\n"
"vilareal     1914         1756\n"
"graciosa     6278         6123\n"
"bignewyork   11261        9917\n"
"\n"
"faro         500          500\n"
"coimbra      1K           1K\n"
"porto        2K           2K\n"
"sydney       4K           4K\n"
"tokyo        10K          8K\n"
"paris        18K          16K\n";

Graph* graph;

static void getFilename(string &filename) {
	cout << files << endl;

	while (1) {
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

	SetConsoleCP(1252);
	SetConsoleOutputCP(1252);

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

	cout << endl << "Loading map information ..";

	if (loadMap(filename, graph) != 0) {
		system("pause");
		return 1;
	}
	cout << ".";
	mainMenu();

	delete graph;
	return 0;
}
