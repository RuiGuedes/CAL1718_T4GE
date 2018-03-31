#include "FunctionsPrototypes.h"
#include <regex>
#include <fstream>

extern GraphViewer *gv;
extern Graph<int> myGraph;

// ** Roads: road_id;road_name;two_way
//    ^(\d+);((?:[-0-9a-zA-ZÀ-ÿ,\.]| )+);(False|True);?$
// -> ECMAScript, icase
//    [0] : Text Line
//    [1] : Road id
//    [2] : Road name
//    [3] : Two way (false, true)

// ** Subroads: road_id;node1_id;node2_id;
//    ^(\d+);(\d+);(\d+);?$
// -> ECMAScript
//    [0] : Text Line
//    [1] : Road id
//    [2] : Node 1 id
//    [3] : Node 2 id


// ** Node: node_id;lat_deg;long_deg;long_rad;lat_rad
//    ^(\d+);(-?\d+.?\d*);(-?\d+.?\d*);(-?\d+.?\d*);(-?\d+.?\d*);?$
// -> ECMAScript
//    [0] : Text Line
//    [1] : Node id
//    [2] : Latitude in degrees
//    [3] : Longitude in degrees
//    [4] : Longitude in radians
//    [5] : Latitude in radians


int load_nodes(string filename) {
	constexpr std::regex reg("^(\\d+);(-?\\d+.?\\d*);(-?\\d+.?\\d*);(-?\\d+.?\\d*);(-?\\d+.?\\d*);?$");

	std::ifstream file(filename);
	if (!file.is_open()) return -1;

	std::string line;
	std::getline(file, line);

	while (!file.eof() && !file.fail()) {


		std::getline(file, line);
	}


	return 0;
}


