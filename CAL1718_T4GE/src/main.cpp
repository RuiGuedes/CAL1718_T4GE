#include "LoadMap.h"
#include "Graph.h"
#include "Interface.h"

#include <stdlib.h>

#define FILENAME_PREFIX "./resource/"

string files = "Select map file"
"Available by default:"
" NODES  SUBROADS     Name"
"                     gporto"
"    1K     1.2K      coimbra"
"    2K     2K        porto"
"    4K     4K        sydney"
"    8K    10K        tokyo"
"   16K    18K        paris";

Graph* graph;

static void getFilename(string &filename) {
	cout << files;

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

	cout << endl << "Loading map information ..." << endl << endl;

	if (loadMap(filename, graph) != 0) {
		system("pause");
		return 1;
	}
	mainMenu();

	delete graph;
	return 0;
}
