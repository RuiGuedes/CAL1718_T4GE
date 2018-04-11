#include "LoadMap.h"
#include "Graph.h"
#include "Interface.h"

#include <stdlib.h>

#define FILENAME_PREFIX "./resource/"

string files = "Select map file\n\n"
"Map files available:\n"
"NAME         SUBROADS     NODES\n"
"fep          64           52\n"	// Minor check
"newyork      125          100\n"	// Minor check
"madrid       300          270\n"	// Minor check
"vilareal     1914         1756\n"	// Minor check
"graciosa     6278         6123\n"	// Minor check
"bignewyork   11261        9917\n"	// Good
"\n"
"faro         500          500\n"		// Decent
"coimbra      1K           1K\n"		// Good
"porto        2K           2K\n"		// Good
"sydney       4K           4K\n"		// Good
"tokyo        10K          8K\n"		// Good
"paris        18K          16K\n";		// Good

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
