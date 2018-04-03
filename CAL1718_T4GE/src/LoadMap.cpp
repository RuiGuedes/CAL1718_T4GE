#include <regex>
#include <fstream>
#include <limits>
#include <exception>
#include <stdexcept>
#include <math.h>

#include "LoadMap.h"

static map<long long, int> nodeIdMap;
static map<long long, int> roadIdMap;
static map<int, string> roadNameMap;
static map<int, bool> roadDirectionMap; 	// True means bidirectional


/*
 * For use by load_nodes
 * @param longitude The node's longitude.
 * @return The node's assigned X coordinate on the GraphViewer map.
 */
static int getX(long double longitude, MapMetaData &meta) {
	// X grows from left to right
	return floor(meta.width * ((longitude - meta.min_longitude) / (meta.max_longitude - meta.min_longitude)));
}

/*
 * For use by load_nodes
 * @param latitude The node's longitude.
 * @return The node's assigned Y coordinate on the GraphViewer map.
 */
static int getY(long double latitude, MapMetaData &meta) {
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
static void estimateMeta(MapMetaData &meta) {
	static constexpr long double latitude_const = 110.574;		// 1 degree Latitude ~ 110.574 km
	static constexpr long double longitude_const = 111.320;		// 1 degree Longitude ~ 111.320 * cos(latitude) km

	if (meta.width != 0 && meta.height != 0) {
		// Assume full recalculation of width and height in this case
		meta.width = 0;
		meta.height = 0;
	}

	long double avg_latitude = (meta.min_latitude + meta.max_latitude) / 2;
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

	if (meta.width == 0 && meta.height == 0) {
		meta.width = ceil(Xd);
		meta.height = ceil(Yd);
	} else {
		if (meta.width != 0 && meta.height == 0) {
			Yd = static_cast<long double>(meta.width) * ratio;
			meta.height = ceil(Yd);
		} else if (meta.width == 0 && meta.height != 0) {
			Xd = static_cast<long double>(meta.height) / ratio;
			meta.width = ceil(Xd);
		}
	}
}

/*
 * For use by load_map and test functions
 * Adds dummy boundary vertices around the map (around 20 per side)
 * @param ...
 */
static void showBoundaries(MapMetaData &meta, Graph* &graph) {
	int ID = graph->getTotalVertices() + 1;

	// Corners
	graph->addVertex(ID++, 0, 0);
	graph->addVertex(ID++, meta.width, 0);
	graph->addVertex(ID++, 0, meta.height);
	graph->addVertex(ID++, meta.width, meta.height);

	int increment;

	// Top and bottom
	increment = meta.width / ((meta.width + 1000) / 200);
	for (int i = increment; i < meta.width; i += increment) {
		graph->addVertex(ID++, i, 0);
		graph->addVertex(ID++, i, meta.height);
	}

	// Left and right
	increment = meta.height / ((meta.height + 1000) / 200);
	for (int j = increment; j < meta.height; j += increment) {
		graph->addVertex(ID++, 0, j);
		graph->addVertex(ID++, meta.width, j);
	}
}





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
	string meta_filename = filename + meta_suffix;
	string nodes_filename = filename + nodes_suffix;
	string roads_filename = filename + roads_suffix;
	string subroads_filename = filename + subroads_suffix;

	// Load the Map's metadata: latitudes, longitudes, number of nodes and edges...
	MapMetaData meta;
	loadMeta(meta_filename, meta);

	// Initialize the map with appropriate sizes, but do nothing else.
	// For some reason, creating the Window immediately is necessary...
	graph = new Graph(meta.width, meta.height);

	// Load the map's nodes...
	// The nodes' filed id is mapped to a new id in nodeIdMap.
	loadNodes(nodes_filename, meta, graph);

	// Load the map's road names...
	// The roads' id is mapped to a new id in roadIdMap, the name is stored in
	// roadNameMap and the direction is stored in roadDirectionMap.
	loadRoads(roads_filename, meta, graph);

	// Load the map's edges...
	loadSubroads(subroads_filename, meta, graph);

	// All good and loaded...
	if (boundaries) showBoundaries(meta, graph);

	// Release auxiliary memory... while preparing for a new invocation later...
	nodeIdMap.clear();
	roadIdMap.clear();
	roadNameMap.clear();
	roadDirectionMap.clear();

	graph->update();
	return 0;
}

// ** Meta: attr=val;
//    attr ?= ?(-?\d+\.?\d*)[.;,]       for long doubles
//    attr ?= ?(\d+)[.;,]               for ints
// -> ECMAScript (icase)
//    [0] : Text Line
//    [1] : Attribute value
int loadMeta(string filename, MapMetaData &meta) {
	static const regex reg_min_lon("min_longitude ?= ?(-?\\d+\\.?\\d*)[.;,]", regex::icase);
	static const regex reg_max_lon("max_longitude ?= ?(-?\\d+\\.?\\d*)[.;,]", regex::icase);
	static const regex reg_min_lat("min_latitude ?= ?(-?\\d+\\.?\\d*)[.;,]", regex::icase);
	static const regex reg_max_lat("max_latitude ?= ?(-?\\d+\\.?\\d*)[.;,]", regex::icase);
	static const regex reg_nodes("nodes ?= ?(\\d+)[.;,]", regex::icase);
	static const regex reg_edges("edges ?= ?(\\d+)[.;,]", regex::icase);
	static const regex reg_density("density ?= ?(-?\\d+\\.?\\d*)[.;,]", regex::icase);
	static const regex reg_width("width ?= ?(\\d+)[.;,]", regex::icase);
	static const regex reg_height("height ?= ?(\\d+)[.;,]", regex::icase);
	static const regex reg_test("test ?= ?(\\d)[.;,]", regex::icase);

	ifstream file(filename);
	if (!file.is_open())
		return -1;

	// Load the entire textfile into text
	string text((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
	file.close();

	smatch match;

	// Mandatory
	if (regex_search(text, match, reg_min_lon))
		meta.min_longitude = stold(match[1]);
	else
		return -1;

	// Mandatory
	if (regex_search(text, match, reg_max_lon))
		meta.max_longitude = stold(match[1]);
	else
		return -1;

	// Mandatory
	if (regex_search(text, match, reg_min_lat))
		meta.min_latitude = stold(match[1]);
	else
		return -1;

	// Mandatory
	if (regex_search(text, match, reg_max_lat))
		meta.max_latitude = stold(match[1]);
	else
		return -1;

	// Mandatory
	if (regex_search(text, match, reg_nodes))
		meta.nodes = stoi(match[1]);
	else
		return -1;

	// Mandatory
	if (regex_search(text, match, reg_edges))
		meta.edges = stoi(match[1]);
	else
		return -1;

	// Optional
	if (regex_search(text, match, reg_density))
		meta.density = stold(match[1]);

	// Optional
	if (regex_search(text, match, reg_width))
		meta.width = stoi(match[1]);

	// Optional
	if (regex_search(text, match, reg_height))
		meta.height = stoi(match[1]);

	// Optional
	if (regex_search(text, match, reg_test))
		meta.test = static_cast<bool>(stoi(match[1]));

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
int loadNodes(string filename, MapMetaData &meta, Graph* &graph) {
	static const regex reg("^(\\d+);(-?\\d+.?\\d*);(-?\\d+.?\\d*);(?:-?\\d+.?\\d*);(?:-?\\d+.?\\d*);?$");

	ifstream file(filename);
	if (!file.is_open())
		return -1;

	string line;
	getline(file, line);

	int newNodes = 0, mappedID = 1;

	while (!file.eof() && !file.fail()) {
		smatch match;

		if (regex_match(line, match, reg)) {
			// Get Node ID
			long long id = stoll(match[1]);
			nodeIdMap[id] = mappedID;

			// Get Node Latitude
			double long latitude = stold(match[2]);
			int y = getY(latitude, meta);

			// Get Node Longitude
			double long longitude = stold(match[3]);
			int x = getX(longitude, meta);

			// Add Node
			graph->addVertex(mappedID, x, y);
			++newNodes;
			++mappedID;
		}

		getline(file, line);
	}

	file.close();

	if(newNodes == meta.nodes)
		return 0;

	return -1;
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
int loadRoads(string filename, MapMetaData &meta, Graph* &graph) {
	static const regex reg("^(\\d+);((?:[-0-9a-zA-ZÀ-ÿ,\\.]| )*);(False|True);?$");
	static const string FalseStr = "False", TrueStr = "True";

	ifstream file(filename);
	if (!file.is_open())
		return -1;

	string line;
	getline(file, line);

	int newRoads = 0, mappedID = 1;

	while (!file.eof() && !file.fail()) {
		smatch match;

		if (regex_match(line, match, reg)) {
			// Get Road ID
			long long id = stoll(match[1]);
			roadIdMap[id] = mappedID;

			// Get Road name
			string name = match[2];
			roadNameMap[mappedID] = name;

			// Get Road Direction
			if(match[3] == TrueStr)
				roadDirectionMap[mappedID] = true;
			else
				roadDirectionMap[mappedID] = false;

			// Register Road
			++newRoads;
			++mappedID;
		}

		getline(file, line);
	}

	file.close();
	return newRoads;
}

// ** Subroads: road_id;node1_id;node2_id;
//    ^(\d+);(\d+);(\d+);?$
//     ( 1 ) ( 2 ) ( 3 )
// -> ECMAScript
//    [0] : Text Line
//    [1] : Road id
//    [2] : Node 1 id
//    [3] : Node 2 id
int loadSubroads(string filename, MapMetaData &meta, Graph* &graph) {
	static const regex reg("^(\\d+);(\\d+);(\\d+);?$");

	ifstream file(filename);
	if (!file.is_open())
		return -1;

	string line;
	getline(file, line);

	int newSubroads = 0, subRoadID = 1;

	while (!file.eof() && !file.fail()) {
		smatch match;
		long long id;

		if (regex_match(line, match, reg)) {
			// Get Road ID
			id = stoll(match[1]);
			int roadid = roadIdMap[id];

			// Get Node 1
			id = stoll(match[2]);
			int node1id = nodeIdMap[id];

			// Get Node 2
			id = stoll(match[3]);
			int node2id = nodeIdMap[id];

			// Register Edges
			Vertex* v1 = graph->getVertex(node1id);
			Vertex* v2 = graph->getVertex(node2id);
			double weight = graph->distance(v1, v2);

			graph->addEdge(subRoadID, node1id, node2id, weight);
			++newSubroads;
			++subRoadID;
			if (roadDirectionMap[roadid]) {
				// Add reverse edge
				graph->addEdge(subRoadID, node2id, node1id, weight);
				++newSubroads;
				++subRoadID;
			}
		}

		getline(file, line);
	}

	file.close();
	return newSubroads;
}


////////////////////
// Test Functions //
////////////////////

int testLoadMeta(string path) {
	try {
		MapMetaData meta;
		loadMeta(path + meta_suffix, meta);
		cout << "Min Longitude: " << meta.min_longitude << endl;
		cout << "Max Longitude: " << meta.max_longitude << endl;
		cout << "Min Latitude: " << meta.min_latitude << endl;
		cout << "Max Latitude: " << meta.max_latitude << endl;
		cout << "Nodes: " << meta.nodes << endl;
		cout << "Edges: " << meta.edges << endl;
		cout << "Estimated X: " << meta.width << endl;
		cout << "Estimated Y: " << meta.height << endl;
		cout << "Load Meta Successful" << endl;

		getchar();
		return 0;
	} catch (exception &e) {
		cout << "Load Meta Failed\n" << e.what() << endl;
		getchar();
		return -1;
	}
}

int testLoadNodes(string path) {
	try {
		MapMetaData meta;
		loadMeta(path + meta_suffix, meta);

		Graph* graph = new Graph(meta.width, meta.height);

		loadNodes(path + nodes_suffix, meta, graph);
		showBoundaries(meta, graph);
		graph->update();

		// *** Test
		auto vertexSet = graph->getAllVertexSet();
		for (auto v : vertexSet) {
			cout << v << endl;
			if (graph->findVertex(v->getId()) == nullptr)
				throw logic_error("Node not found in Graph");
		}

		getchar();
		delete graph;
		return 0;
	} catch (std::exception &e) {
		cout << "Load Nodes Failed\n" << e.what() << std::endl;
		getchar();
		return -1;
	}
}

int testLoadRoads(string path) {
	try {
		MapMetaData meta;
		loadMeta(path + meta_suffix, meta);

		Graph* graph = new Graph(meta.width, meta.height);

		loadNodes(path + nodes_suffix, meta, graph);
		showBoundaries(meta, graph);
		graph->update();

		loadRoads(path + roads_suffix, meta, graph);

		for (auto road : roadIdMap) {
			int id = road.second;
			cout << "ID=" << id << " ; ";
			cout << "NAME=" << roadNameMap[id] << " ; ";
			cout << "TWOWAY=" << (roadDirectionMap[id] ? "true" : "false");
			cout << " ; " << endl;
		}

		getchar();
		delete graph;
		return 0;
	} catch (exception &e) {
		cout << "Load Roads Failed\n" << e.what() << endl;
		getchar();
		return -1;
	}
}

int testLoadSubroads(string path) {
	try {
		MapMetaData meta;
		loadMeta(path + meta_suffix, meta);

		Graph* graph = new Graph(meta.width, meta.height);

		loadNodes(path + nodes_suffix, meta, graph);
		showBoundaries(meta, graph);
		graph->update();

		loadRoads(path + roads_suffix, meta, graph);

		loadSubroads(path + subroads_suffix, meta, graph);

		auto vertexSet = graph->getAllVertexSet();
		for (auto v : vertexSet) {
			for (auto edge : v->getAdj()) {
				cout << edge << endl;
			}
		}

		getchar();
		delete graph;
		return 0;
	} catch (exception &e) {
		cout << "Load Roads Failed\n" << e.what() << endl;
		getchar();
		return -1;
	}
}

int testLoadMap(string path) {
	try {
		Graph* graph;

		loadMap(path, graph, true);

		getchar();
		delete graph;
		return 0;
	} catch (exception &e) {
		cout << "Load Meta Failed\n" << e.what() << endl;
		getchar();
		return -1;
	}
}
