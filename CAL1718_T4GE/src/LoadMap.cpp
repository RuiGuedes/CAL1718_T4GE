#include <fstream>
#include <limits>
#include <exception>
#include <stdexcept>
#include <regex>
#include <math.h>
#include <stdlib.h>

#include "LoadMap.h"

static map<long long, int> nodeIdMap;
static map<long long, int> roadIdMap;
static map<int, Road*> roadMap;

/////////////////////////
// Auxiliary Functions //
/////////////////////////

/*
 * For use by load_nodes
 * @param longitude The node's longitude.
 * @return The node's assigned X coordinate on the GraphViewer map.
 */
static int getX(long double longitude, MetaData &meta) {
	// X grows from left to right
	return floor(meta.width * ((longitude - meta.min_longitude) / (meta.max_longitude - meta.min_longitude)));
}

/*
 * For use by load_nodes
 * @param latitude The node's longitude.
 * @return The node's assigned Y coordinate on the GraphViewer map.
 */
static int getY(long double latitude, MetaData &meta) {
	// Y grows from top to bottom
	return floor(meta.height * ((meta.max_latitude - latitude) / (meta.max_latitude - meta.min_latitude)));
}

/*
 * For use by whatever
 * @param degrees Angle in degrees.
 * @return Angle in radians.
 */
static long double toRadians(long double degrees) {
	static constexpr long double pi = acos(-1);
	return degrees * pi / 180.0;
}

/*
 * For use by MapMetaData
 * Estimates the map width and height based on map density and the
 * nodes's geographic position.
 * @param meta The MapMetaData struct.
 */
static void estimateMeta(MetaData &meta) {
	static constexpr long double latitude_const = 110.574;		// 1 degree Latitude ~ 110.574 km
	static constexpr long double longitude_const = 111.320;		// 1 degree Longitude ~ 111.320 * cos(latitude) km

	if (meta.width != 0 && meta.height != 0) {
		// Assume full recalculation of width and height in this case
		meta.width = 0;
		meta.height = 0;
	}

	long double avg_latitude = (meta.min_latitude + meta.max_latitude) / 2.0;
	long double delta_latitude = meta.max_latitude - meta.min_latitude;
	long double delta_longitude = meta.max_longitude - meta.min_longitude;

	// Good approximations of physical distance in kilometers
	long double distanceYkm = abs(latitude_const * delta_latitude);
	long double distanceXkm = abs(longitude_const * delta_longitude * cos(toRadians(avg_latitude)));

	// The X,Y ratio of the resulting GraphViewer map:
	long double ratio = distanceYkm / distanceXkm;

	// In a geometry of size X x Y there are X*Y possible node positions.
	// Assume the nodes are well spread out throughout the map we are
	// going to create. If we expect node density ~ K (meta.density)
	// then the number of nodes (meta.nodes) should be approximately
	// equal to X*Y*K = X*X*K*ratio. Therefore X = sqrt(nodes / K*ratio).

	long double Xd = sqrt(meta.nodes / (meta.density * ratio));
	long double Yd = Xd * ratio;

	// Case 1: Use density
	if (meta.width == 0 && meta.height == 0) {
		meta.width = ceil(Xd);
		meta.height = ceil(Yd);
		meta.scale = 1000.0 * distanceXkm / Xd;
	} else {
		// Case 2: Use width
		if (meta.width != 0 && meta.height == 0) {
			Yd = static_cast<long double>(meta.width) * ratio;
			meta.height = ceil(Yd);
			meta.scale = 1000.0 * distanceYkm / Yd;
			meta.density = static_cast<long double>(meta.nodes) / (Xd * Yd);
		}
		// Case 3: Use height
		else if (meta.width == 0 && meta.height != 0) {
			Xd = static_cast<long double>(meta.height) / ratio;
			meta.width = ceil(Xd);
			meta.scale = 1000.0 * distanceXkm / Xd;
			meta.density = static_cast<long double>(meta.nodes) / (Xd * Yd);
		}
	}
}

/*
 * For use by loadMap
 * Checks if all files are present and openable
 * Extremely rigid -- exits if some file is not found
 */
bool checkFilename(string filename) {
	ifstream file;

	file.open(filename + meta_suffix);
	if (!file.is_open() || !file.good()) {
		cout << "Meta file not found (" << filename + meta_suffix << ")" << endl;
		//system("pause");
		return false;
	}
	file.close();

	file.open(filename + nodes_suffix);
	if (!file.is_open() || !file.good()) {
		cout << "Nodes file not found (" << filename + nodes_suffix << ")" << endl;
		//system("pause");
		return false;
	}
	file.close();

	file.open(filename + roads_suffix);
	if (!file.is_open() || !file.good()) {
		cout << "Roads file not found (" << filename + roads_suffix << ")" << endl;
		//system("pause");
		return false;
	}
	file.close();

	file.open(filename + subroads_suffix);
	if (!file.is_open() || !file.good()) {
		cout << "Subroads file not found (" << filename + subroads_suffix << ")" << endl;
		//system("pause");
		return false;
	}
	file.close();

	return true;
}



////////////////////
// Load Functions //
////////////////////

/*
 * @brief Initializes the Graph (only barely).
 * Reads all information from the collection
 * of files of format "filename + suffix",
 * where suffix is meta, nodes, roads, subroads by default...
 * @param filename The files' indicative name (e.g. gporto for the gporto_*.txt files)
 * @param gv GraphViewer
 * @param myGraph Graph
 * @param boundaries Whether to add dummy boundary vertices around the generated map (defaults to false)
 * @return Standard Success/Error
 */
int loadMap(string filename, Graph* &graph, bool boundaries) {
	// Exit if any of the 4 files is not found
	if (!checkFilename(filename)) {
		return -1;
	}

	// Load meta data
	MetaData meta;
	if (loadMeta(filename + meta_suffix, meta) != 0) {
		return -1;
	}

	// Initialize the graph
	graph = new Graph(meta.width, meta.height, meta.scale);

	// Load Nodes, Roads and Subroads
	if (loadNodes(filename + nodes_suffix, meta, graph) != 0) {
		return -1;
	}
	if (loadRoads(filename + roads_suffix, meta, graph) != 0) {
		return -1;
	}
	if (loadSubroads(filename + subroads_suffix, meta, graph) != 0) {
		return -1;
	}

	if (meta.boundaries) graph->showBoundaries();

	// Release auxiliary memory
	nodeIdMap.clear();
	roadIdMap.clear();
	roadMap.clear();

	graph->update();
	return 0;
}

// ** Meta: attr=val;
//    attr ?= ?(-?\d+\.?\d*)[.;,]       for long doubles
//    attr ?= ?(\d+)[.;,]               for ints
// -> ECMAScript (icase)
//    [0] : Text Line
//    [1] : Attribute value
int loadMeta(string filename, MetaData &meta) {
	static const regex reg_min_lon("min_longitude ?= ?(-?\\d+\\.?\\d*)[.;,]", regex::icase);
	static const regex reg_max_lon("max_longitude ?= ?(-?\\d+\\.?\\d*)[.;,]", regex::icase);
	static const regex reg_min_lat("min_latitude ?= ?(-?\\d+\\.?\\d*)[.;,]", regex::icase);
	static const regex reg_max_lat("max_latitude ?= ?(-?\\d+\\.?\\d*)[.;,]", regex::icase);
	static const regex reg_nodes("nodes ?= ?(\\d+)[.;,]", regex::icase);
	static const regex reg_edges("edges ?= ?(\\d+)[.;,]", regex::icase);
	static const regex reg_density("density ?= ?(-?\\d+\\.?\\d*)[.;,]", regex::icase);
	static const regex reg_width("width ?= ?(\\d+)[.;,]", regex::icase);
	static const regex reg_height("height ?= ?(\\d+)[.;,]", regex::icase);
	static const regex reg_boundaries("boundaries ?= ?([01])[.;,]", regex::icase);

	ifstream file(filename);
	if (!file.is_open())
		return -1;

	// Load the entire textfile into text
	string text((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
	file.close();

	smatch match;

	try {
		// Mandatory: MIN_LONGITUDE
		if (regex_search(text, match, reg_min_lon))
			meta.min_longitude = stold(match[1]);
		else
			return -1;

		// Mandatory: MAX_LONGITUDE
		if (regex_search(text, match, reg_max_lon))
			meta.max_longitude = stold(match[1]);
		else
			return -1;

		// Mandatory: MIN_LATITUDE
		if (regex_search(text, match, reg_min_lat))
			meta.min_latitude = stold(match[1]);
		else
			return -1;

		// Mandatory: MAX_LATITUDE
		if (regex_search(text, match, reg_max_lat))
			meta.max_latitude = stold(match[1]);
		else
			return -1;

		// Mandatory: NODES
		if (regex_search(text, match, reg_nodes))
			meta.nodes = stoi(match[1]);
		else
			return -1;

		// Mandatory: EDGES
		if (regex_search(text, match, reg_edges))
			meta.edges = stoi(match[1]);
		else
			return -1;

		// Optional: DENSITY
		if (regex_search(text, match, reg_density))
			meta.density = stold(match[1]);

		// Optional: WIDTH
		if (regex_search(text, match, reg_width))
			meta.width = stoi(match[1]);

		// Optional: HEIGHT
		if (regex_search(text, match, reg_height))
			meta.height = stoi(match[1]);

		// Optional: BOUNDARIES
		if (regex_search(text, match, reg_boundaries))
			meta.boundaries = static_cast<bool>(stoi(match[1]));
	} catch (runtime_error &e) {
		cerr << e.what() << endl;
		cerr << "Found numeric field not representable in " << filename << endl;
		return -2;
	}

	estimateMeta(meta);
	return 0;
}

// ** Node: node_id;lat_deg;long_deg;long_rad;lat_rad
//    ^(\d+);(-?\d+.?\d*);(-?\d+.?\d*);(?:-?\d+.?\d*);(?:-?\d+.?\d*);?$
//     ( 1 ) (    2     ) (    3     ) (?:    4     ) (?:    5     )
// -> ECMAScript
//    [0] : Text Line
//    [1] : Node id (long long)
//    [2] : Latitude in degrees (long double)
//    [3] : Longitude in degrees (long double)
//    [4]*: Longitude in radians (long double)
//    [5]*: Latitude in radians (long double)
int loadNodes(string filename, MetaData &meta, Graph* graph) {
	static const regex reg("^(\\d+);(-?\\d+.?\\d*);(-?\\d+.?\\d*);(?:-?\\d+.?\\d*);(?:-?\\d+.?\\d*);?$");

	ifstream file(filename);
	if (!file.is_open())
		return -1;

	string line;
	getline(file, line);

	int newNodes = 0, lineID = 1;

	while (!file.eof() && !file.fail()) {
		smatch match;

		if (regex_match(line, match, reg)) {
			try {
				// Get Node ID
				long long id = stoll(match[1]);
				nodeIdMap[id] = lineID;

				// Get Node Latitude
				double long latitude = stold(match[2]);
				int y = getY(latitude, meta);

				// Get Node Longitude
				double long longitude = stold(match[3]);
				int x = getX(longitude, meta);

				// Add Node
				graph->addVertex(lineID, x, y);
				++newNodes;
			} catch (runtime_error &e) {
				cerr << e.what() << endl;
				cerr << "Found numeric field not representable in " << filename << endl;
				cerr << "Line: " << lineID << endl;
				return -2;
			}
		}

		++lineID;
		getline(file, line);
	}

	file.close();
	return 0;
}

// ** Roads: road_id;road_name;two_way
//    ^(\d+);((?:[-0-9a-zA-ZÀ-ÿ,\.]| )*);(False|True);?$
//     ( 1 ) (            2            ) (    3     )
// -> ECMAScript (icase)
// Nota: A rua pode não ter nome!...
//    [0] : Text Line
//    [1] : Road id (long long)
//    [2] : Road name (string)
//    [3] : Two way (bool)
int loadRoads(string filename, MetaData &meta, Graph* graph) {
	static const regex reg("^(\\d+);(.*?);(False|True);?$");
	static const string FalseStr = "False", TrueStr = "True";

	ifstream file(filename);
	if (!file.is_open())
		return -1;

	string line;
	getline(file, line);

	int newRoads = 0, lineID = 1;

	while (!file.eof() && !file.fail()) {
		smatch match;

		if (regex_match(line, match, reg)) {
			try {
				// Get Road ID
				long long id = stoll(match[1]);
				roadIdMap[id] = lineID;

				// Get Road Name
				string name = match[2];

				// Get Road Direction
				bool bothways;
				if (match[3] == TrueStr)
					bothways = true;
				else
					bothways = false;

				// Register Road
				Road* road = new Road(lineID, name, bothways);
				roadMap[lineID] = road;
				++newRoads;
			} catch (runtime_error &e) {
				cerr << e.what() << endl;
				cerr << "Found numeric field not representable in " << filename << endl;
				cerr << "Line: " << lineID << endl;
				return -2;
			}
		}

		++lineID;
		getline(file, line);
	}

	file.close();
	return 0;
}

// ** Subroads: road_id;node1_id;node2_id;
//    ^(\d+);(\d+);(\d+);?$
//     ( 1 ) ( 2 ) ( 3 )
// -> ECMAScript
//    [0] : Text Line
//    [1] : Road id
//    [2] : Node 1 id
//    [3] : Node 2 id
int loadSubroads(string filename, MetaData &meta, Graph* graph) {
	static const regex reg("^(\\d+);(\\d+);(\\d+);?$");

	ifstream file(filename);
	if (!file.is_open())
		return -1;

	string line;
	getline(file, line);

	int newSubroads = 0, subRoadID = 1, lineID = 1;
	Road* currentRoad = nullptr;
	vector<Subroad*> subroads;

	while (!file.eof() && !file.fail()) {
		smatch match;
		long long id;

		if (regex_match(line, match, reg)) {
			try {
				// Get Road ID
				id = stoll(match[1]);
				int roadid = roadIdMap[id];

				// Finish setting up the previous Road's data
				// if this subroad belongs to a different Road
				if (!currentRoad) {
					currentRoad = roadMap[roadid];
				}
				else if (roadMap[roadid] != currentRoad) {
					double totalDistance = 0;
					for (auto subroad : subroads)
						totalDistance += subroad->getDistance();

					currentRoad->setTotalDistance(totalDistance);
					subroads.clear();

					currentRoad = roadMap[roadid];
				}

				// Get Node 1
				id = stoll(match[2]);
				int node1id = nodeIdMap[id];

				// Get Node 2
				id = stoll(match[3]);
				int node2id = nodeIdMap[id];

				// Get vertices and distance
				Vertex* v1 = graph->getVertex(node1id);
				Vertex* v2 = graph->getVertex(node2id);
				double distance = graph->distance(v1, v2);

				// Load subroad
				Subroad* subroad = new Subroad(distance, currentRoad);
				subroads.push_back(subroad);
				graph->addEdge(subRoadID, v1, v2, subroad);
				++newSubroads;
				++subRoadID;
				if (currentRoad->isBidirectional()) {
					// Add reverse edge
					graph->addEdge(subRoadID, v2, v1, subroad);
					++newSubroads;
					++subRoadID;
				}
			} catch (runtime_error &e) {
				cerr << e.what() << endl;
				cerr << "Found numeric field not representable in " << filename << endl;
				cerr << "Line: " << lineID << endl;
				return -2;
			}
		}

		++lineID;
		getline(file, line);
	}

	if (currentRoad != nullptr) {
		double totalDistance = 0;
		for (auto subroad : subroads)
			totalDistance += subroad->getDistance();

		currentRoad->setTotalDistance(totalDistance);
		subroads.clear();
	}

	file.close();
	return 0;
}


////////////////////
// Test Functions //
////////////////////

int testLoadMeta(string path) {
	try {
		// Exit if any of the 4 files is not found
		if (!checkFilename(path)) {
			return -1;
		}

		// Load meta data
		MetaData meta;
		if (loadMeta(path + meta_suffix, meta) != 0) {
			return -1;
		}

		cout << "Min Longitude: " << meta.min_longitude << endl;
		cout << "Max Longitude: " << meta.max_longitude << endl;
		cout << "Min Latitude: " << meta.min_latitude << endl;
		cout << "Max Latitude: " << meta.max_latitude << endl;
		cout << "Nodes: " << meta.nodes << endl;
		cout << "Edges: " << meta.edges << endl;
		cout << "Density: " << meta.density << endl;
		cout << "Boundaries: " << meta.boundaries << endl;

		cout << "Computed Scale: " << meta.scale << endl;
		cout << "Estimated X: " << meta.width << endl;
		cout << "Estimated Y: " << meta.height << endl;

		system("pause");
		return 0;
	} catch (exception &e) {
		cout << "Load Meta Failed\n" << e.what() << endl;
		system("pause");
		return -1;
	}
}

int testLoadNodes(string path) {
	try {
		Graph* graph;

		// Exit if any of the 4 files is not found
		if (!checkFilename(path)) {
			return -1;
		}

		// Load meta data
		MetaData meta;
		if (loadMeta(path + meta_suffix, meta) != 0) {
			return -1;
		}

		// Initialize the graph
		graph = new Graph(meta.width, meta.height);

		// Load Nodes, Roads and Subroads
		if (loadNodes(path + nodes_suffix, meta, graph) != 0) {
			return -1;
		}

		if (meta.boundaries) graph->showBoundaries();

		graph->update();

		// *** Test
		auto vertexSet = graph->getAllVertexSet();
		for (auto v : vertexSet) {
			cout << v << endl;
		}

		system("pause");
		delete graph;
		return 0;
	} catch (std::exception &e) {
		cout << "Load Nodes Failed\n" << e.what() << std::endl;
		system("pause");
		return -1;
	}
}

int testLoadRoads(string path) {
	try {
		Graph* graph;

		// Exit if any of the 4 files is not found
		if (!checkFilename(path)) {
			return -1;
		}

		// Load meta data
		MetaData meta;
		if (loadMeta(path + meta_suffix, meta) != 0) {
			return -1;
		}

		// Initialize the graph
		graph = new Graph(meta.width, meta.height);

		// Load Nodes, Roads and Subroads
		if (loadNodes(path + nodes_suffix, meta, graph) != 0) {
			return -1;
		}
		if (loadRoads(path + roads_suffix, meta, graph) != 0) {
			return -1;
		}

		if (meta.boundaries) graph->showBoundaries();

		graph->update();

		// *** Test
		for (auto const& pair : roadMap) {
			cout << pair.second << endl;
		}

		system("pause");
		delete graph;
		return 0;
	} catch (exception &e) {
		cout << "Load Roads Failed\n" << e.what() << endl;
		system("pause");
		return -1;
	}
}

int testLoadSubroads(string path) {
	try {
		Graph* graph;

		// Exit if any of the 4 files is not found
		if (!checkFilename(path)) {
			return -1;
		}

		// Load meta data
		MetaData meta;
		if (loadMeta(path + meta_suffix, meta) != 0) {
			return -1;
		}

		// Initialize the graph
		graph = new Graph(meta.width, meta.height);

		// Load Nodes, Roads and Subroads
		if (loadNodes(path + nodes_suffix, meta, graph) != 0) {
			return -1;
		}
		if (loadRoads(path + roads_suffix, meta, graph) != 0) {
			return -1;
		}
		if (loadSubroads(path + subroads_suffix, meta, graph) != 0) {
			return -1;
		}

		if (meta.boundaries) graph->showBoundaries();

		graph->update();

		// *** Test
		auto vertexSet = graph->getAllVertexSet();
		for (auto v : vertexSet) {
			for (auto edge : v->getAdj()) {
				cout << edge << endl;
			}
		}

		system("pause");
		delete graph;
		return 0;
	} catch (exception &e) {
		cout << "Load Roads Failed\n" << e.what() << endl;
		system("pause");
		return -1;
	}
}

int testLoadMap(string path) {
	try {
		Graph* graph;

		loadMap(path, graph, true);

		system("pause");
		delete graph;
		return 0;
	} catch (exception &e) {
		cout << "Load Meta Failed\n" << e.what() << endl;
		system("pause");
		return -1;
	}
}

int testNewMap(string path) {
	int status;

	if (!checkFilename(path)) {
		return -1;
	}

	status = testLoadMeta(path);
	if (status != 0)
		return -1;

	cout << "Next test " << endl << endl;

	status = testLoadNodes(path);
	if (status != 0)
		return -1;

	cout << "Next test " << endl << endl;

	status = testLoadRoads(path);
	if (status != 0)
		return -1;

	cout << "Next test " << endl << endl;

	status = testLoadSubroads(path);
	if (status != 0)
		return -1;

	cout << "Next test " << endl << endl;

	status = testLoadMap(path);
	if (status != 0)
		return -1;

	system("pause");
	exit(0);
}
