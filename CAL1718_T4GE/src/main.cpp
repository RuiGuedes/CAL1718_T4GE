#include "LoadMap.h"
#include "Graph.h"
#include "Interface.h"

#include <stdlib.h>

#define FILENAME_PREFIX "./resource/"

string files = "Select map file\n"
"Available by default:\n"
" NODES   ~SUBROADS   NAME\n"
"    52        64      fep\n"             // Minor check
"   100       125      newyork\n"         // Minor check
"   270       300      madrid\n"          // Minor check
"  1756      1914      vilareal\n"        // Minor check
"  6123      6278      graciosa\n"        // Minor check
"  9917     11261      bignewyork\n"      // Good
"\n"
"   500       500      faro\n"            // Decent
"    1K        1K      coimbra\n"         // Good
"    2K        2K      porto\n"           // Good
"    4K        4K      sydney\n"          // Good
"    8K       10K      tokyo\n"           // Good
"   16K       18K      paris\n";          // Good

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

	cout << endl << "Loading map information ..." << endl << endl;

	if (loadMap(filename, graph) != 0) {
		system("pause");
		return 1;
	}
	mainMenu();

	delete graph;
	return 0;
}
