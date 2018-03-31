#include "FunctionsPrototypes.h"
#include <regex>
#include <fstream>
#include <numeric_limits>

extern GraphViewer *gv;
extern Graph<int> myGraph;

class GraphMetaData {
public:
	long double min_latitude, max_latitude;
	long double min_longitude, max_longitude;
	int nodes, edges;

	int width, height;

	int getX(long double longitude) {
		return width * (longitude - min_longitude) / (max_longitude - min_longitude);
	}
	int getY(long double latitude) {
		return height * (latitude - min_latitude) / (max_latitude - min_latitude);
	}
};

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
//    [4]*: Longitude in radians
//    [5]*: Latitude in radians
int load_nodes(string filename, GraphMetaData meta) {
	const std::regex reg("^(\\d+);(-?\\d+.?\\d*);(-?\\d+.?\\d*);(-?\\d+.?\\d*);(-?\\d+.?\\d*);?$");

	std::ifstream file(filename);
	if (!file.is_open()) return -1;

	std::string line;
	std::getline(file, line);

	while (!file.eof() && !file.fail()) {
		std::smatch match;
		if (std::regex_match(line, match, reg)) {
			// Get Node ID
			long long id = std::stoll(match[1]);
			if (id > std::numeric_limits<int>::max()) {
				// TODO
			}
			// Get Node Latitude
			double long latitude = std::stold(match[2]);
			int y = meta.getY(latitude);
			// Get Node Longitude
			double long longitude = std::stold(match[3]);
			int x = meta.getX(longitude);

			// Add Node
			myGraph.addVertex()
		}

		std::getline(file, line);
	}


	return 0;
}


