#include "FunctionsPrototypes.h"
#include <regex>
#include <fstream>
#include <limits>
#include <exception>
#include "load_map.h"

static std::map<long long, int> nodeIdMap;
static std::map<long long, int> roadIdMap;
static std::map<int, string> roadNameMap;
static std::map<int, bool> roadDirectionMap; // true means bidirectional



/*
 * For use by load_nodes
 * @param longitude The node's longitude.
 * @return The node's assigned X coordinate on the GraphViewer map.
 */
static int getX(long double longitude, MapMetaData &meta) {
	// X grows from left to right
	return std::floor(meta.width * (longitude - meta.min_longitude)
			/ (meta.max_longitude - meta.min_longitude));
}

/*
 * For use by load_nodes
 * @param latitude The node's longitude.
 * @return The node's assigned Y coordinate on the GraphViewer map.
 */
static int getY(long double latitude, MapMetaData &meta) {
	// Y grows from top to bottom
	return std::floor(meta.height * (meta.max_latitude - latitude)
			/ (meta.max_latitude - meta.min_latitude));
}

/*
 * For use by whatever
 * @param degrees Angle in degrees.
 * @return Angle in radians.
 */
static long double to_radians(long double degrees) {
	static constexpr long double pi = std::acos(-1);
	return degrees * pi / 180;
}

/*
 * For user by load_map
 * @param meta The MapMetaData struct.
 * @return
 */
static void estimate_meta(MapMetaData &meta) {
	// 1 degree Latitude ~ 110.574 km
	// 1 degree Longitude ~ 111.320 * cos(latitude) km
	static constexpr long double latitude_const = 110.574;
	static constexpr long double longitude_const = 111.320;

	if (meta.width != 0 && meta.height != 0) {
		// Assume full recalculation
		meta.width = 0; meta.height = 0;
	}

	long double avg_latitude = (meta.min_latitude + meta.max_latitude) / 2;
	long double delta_latitude = meta.max_latitude - meta.min_latitude;
	long double delta_longitude = meta.max_longitude - meta.min_longitude;

	// Good approximations of physical distance
	long double distanceYkm = std::abs(latitude_const * delta_latitude);
	long double distanceXkm = std::abs(longitude_const * delta_longitude
			* std::cos(to_radians(avg_latitude)));

	// The X,Y ratio of the resulting GraphViewer map:
	long double ratio = distanceYkm / distanceXkm;

	// In a geometry of size X x Y there are X*Y possible node positions.
	// Assume the nodes are well spread out throughout the map we are
	// going to create. If we expect node density ~ K (meta.density)
	// then the number of nodes (meta.nodes) should be approximately
	// equal to X*Y*K = X*X*K*ratio. Therefore X = sqrt(nodes / K*ratio).

	long double Xd = std::sqrt(meta.nodes / (meta.density * ratio));
	long double Yd = Xd * ratio;

	if (meta.width == 0 && meta.height == 0) {
		meta.width = std::ceil(Xd);
		meta.height = std::ceil(Yd);
	} else {
		if (meta.width != 0 && meta.height == 0) {
			Yd = static_cast<long double>(meta.width) * ratio;
			meta.height = std::ceil(Yd);
		} else if (meta.width == 0 && meta.height != 0) {
			Xd = static_cast<long double>(meta.height) / ratio;
			meta.width = std::ceil(Xd);
		}
	}
}





int load_map(std::string filename, GraphViewer* &gv, Graph<int> &myGraph) {
	std::string meta_filename = filename + meta_suffix;
	std::string nodes_filename = filename + nodes_suffix;
	std::string roads_filename = filename + roads_suffix;
	std::string subroads_filename = filename + subroads_suffix;

	// Load the Map's metadata: latitudes, longitudes, number of nodes and edges.
	MapMetaData meta;
	load_meta(meta_filename, meta);

	// Initialize the map with appropriate sizes, but do nothing else.
	gv = new GraphViewer(meta.width, meta.height, false);

	// Load the map's nodes.
	// The nodes' filed id is mapped to a new id in nodeIdMap.
	load_nodes(nodes_filename, meta, gv, myGraph);

	// Load the map's roads.
	// The roads' id is mapped to a new id in roadIdMap, the name is stored in
	// roadNameMap and the direction is stored in roadDirectionMap.
	load_roads(roads_filename, meta, gv, myGraph);

	// Load the map's edges.
	load_subroads(subroads_filename, meta, gv, myGraph);

	return 0;
}





int load_meta(std::string filename, MapMetaData &meta) {
	static const std::regex reg_min_lon("min_longitude ?= ?(-?\\d+.?\\d*)[.;,]", std::regex::icase);
	static const std::regex reg_max_lon("max_longitude ?= ?(-?\\d+.?\\d*)[.;,]", std::regex::icase);
	static const std::regex reg_min_lat("min_latitude ?= ?(-?\\d+.?\\d*)[.;,]", std::regex::icase);
	static const std::regex reg_max_lat("max_latitude ?= ?(-?\\d+.?\\d*)[.;,]", std::regex::icase);
	static const std::regex reg_nodes("nodes ?= ?(\\d+)[.;,]", std::regex::icase);
	static const std::regex reg_edges("edges ?= ?(\\d+)[.;,]", std::regex::icase);
	static const std::regex reg_density("density ?= ?(-?\\d+.?\\d*)[.;,]", std::regex::icase);

	std::ifstream file(filename);
	if (!file.is_open()) return -1;

	std::string text((std::istreambuf_iterator<char>(file)),
					std::istreambuf_iterator<char>());
	file.close();

	std::smatch match;

	// Mandatory
	if (std::regex_search(text, match, reg_min_lon))
		meta.min_longitude = std::stold(match[1]);
	else return -1;

	// Mandatory
	if (std::regex_search(text, match, reg_max_lon))
		meta.max_longitude = std::stold(match[1]);
	else return -1;

	// Mandatory
	if (std::regex_search(text, match, reg_min_lat))
		meta.min_latitude = std::stold(match[1]);
	else return -1;

	// Mandatory
	if (std::regex_search(text, match, reg_max_lat))
		meta.max_latitude = std::stold(match[1]);
	else return -1;

	// Mandatory
	if (std::regex_search(text, match, reg_nodes))
		meta.nodes = std::stoi(match[1]);
	else return -1;

	// Mandatory
	if (std::regex_search(text, match, reg_edges))
		meta.edges = std::stoi(match[1]);
	else return -1;

	// Optional
	if (std::regex_search(text, match, reg_density))
		meta.density = std::stold(match[1]);

	estimate_meta(meta);
	return 0;
}





// **
// ** Node: node_id;lat_deg;long_deg;long_rad;lat_rad
//    ^(\d+);(-?\d+.?\d*);(-?\d+.?\d*);(?:-?\d+.?\d*);(?:-?\d+.?\d*);?$
// -> ECMAScript
//    [0] : Text Line
//    [1] : Node id (long long)
//    [2] : Latitude in degrees (long double)
//    [3] : Longitude in degrees (long double)
//    [4]*: Longitude in radians (long double)
//    [5]*: Latitude in radians (long double)
int load_nodes(std::string filename, MapMetaData &meta, GraphViewer* &gv, Graph<int> &myGraph) {
	static const std::regex reg("^(\\d+);(-?\\d+.?\\d*);(-?\\d+.?\\d*);(?:-?\\d+.?\\d*);(?:-?\\d+.?\\d*);?$");

	std::ifstream file(filename);
	if (!file.is_open()) return -1;

	std::string line;
	std::getline(file, line);

	int new_nodes = 0, mappedid = 0;

	while (!file.eof() && !file.fail()) {
		std::smatch match;

		if (std::regex_match(line, match, reg)) {
			// Get Node ID
			long long id = std::stoll(match[1]);
			nodeIdMap[id] = mappedid;

			// Get Node Latitude
			double long latitude = std::stold(match[2]);
			int y = getY(latitude, meta);

			// Get Node Longitude
			double long longitude = std::stold(match[3]);
			int x = getX(longitude, meta);

			// Add Node
			myGraph.addVertex(mappedid);
			gv->addNode(mappedid, x, y);
			++new_nodes; ++mappedid;
		}
		// else skip line

		std::getline(file, line);
	}

	file.close();
	return new_nodes == meta.nodes ? 0 : -1;
}





// ** Roads: road_id;road_name;two_way
//    ^(\d+);((?:[-0-9a-zA-ZÀ-ÿ,\.]| )+);(False|True);?$
// -> ECMAScript (icase)
//    [0] : Text Line
//    [1] : Road id (long long)
//    [2] : Road name (string)
//    [3] : Two way (bool)
int load_roads(std::string filename, MapMetaData &meta, GraphViewer* &gv, Graph<int> &myGraph) {
	static const std::regex reg("^(\\d+);((?:[-0-9a-zA-ZÀ-ÿ,\\.]| )+);(False|True);?$");
	static const std::string FalseStr = "False", TrueStr = "True";

	std::ifstream file(filename);
	if (!file.is_open()) return -1;

	std::string line;
	std::getline(file, line);

	int new_roads = 0, mappedid = 0;

	while (!file.eof() && !file.fail()) {
		std::smatch match;

		if (std::regex_match(line, match, reg)) {
			// Get Road ID
			long long id = std::stoll(match[1]);
			roadIdMap[id] = mappedid;

			// Get Road name
			std::string name = match[2];
			roadNameMap[mappedid] = name;

			// Get Road Direction
			bool twoway = match[3] == TrueStr ? true : false;
			roadDirectionMap[mappedid] = twoway;

			// Register Road
			++new_roads; ++mappedid;
		}
		// else skip line

		std::getline(file, line);
	}

	file.close();
	return new_roads;
}





// ** Subroads: road_id;node1_id;node2_id;
//    ^(\d+);(\d+);(\d+);?$
// -> ECMAScript
//    [0] : Text Line
//    [1] : Road id
//    [2] : Node 1 id
//    [3] : Node 2 id
int load_subroads(std::string filename, MapMetaData &meta, GraphViewer* &gv, Graph<int> &myGraph) {
	static const std::regex reg("^(\\d+);(\\d+);(\\d+);?$");

	std::ifstream file(filename);
	if (!file.is_open()) return -1;

	std::string line;
	std::getline(file, line);

	int new_subroads = 0, subroadid = 0;

	while (!file.eof() && !file.fail()) {
		std::smatch match;
		long long id;

		if (std::regex_match(line, match, reg)) {
			// Get Road ID
			id = std::stoll(match[1]);
			int roadid = roadIdMap[id];

			// Get Node 1
			id = std::stoll(match[2]);
			int node1id = nodeIdMap[id];

			// Get Node 2
			id = std::stoll(match[2]);
			int node2id = nodeIdMap[id];

			// Register Edges
			std::cout << roadid << " ; " << node1id << " ; " << node2id << std::endl;
			myGraph.addEdge(node1id, node2id, 0);
			gv->addEdge(subroadid, node1id, node2id, EdgeType::UNDIRECTED);
			if (roadDirectionMap[roadid]) {
				// Add reverse edges
				myGraph.addEdge(node2id, node1id, 0);
				gv->addEdge(subroadid, node2id, node1id, EdgeType::UNDIRECTED);
			}
			++new_subroads; ++subroadid;
		}
		// else skip line

		std::getline(file, line);
	}

	file.close();
	return new_subroads;
}










static void show_boundaries(MapMetaData &meta, GraphViewer* gv, Graph<int> myGraph) {
	int ID = myGraph.getNumVertex() + 1;

	// Corners
	myGraph.addVertex(ID);
	gv->addNode(ID++, 0, 0);

	myGraph.addVertex(ID);
	gv->addNode(ID++, meta.width, 0);

	myGraph.addVertex(ID);
	gv->addNode(ID++, 0, meta.height);

	myGraph.addVertex(ID);
	gv->addNode(ID++, meta.width, meta.height);

	int increment;

	// Top and bottom
	increment = (meta.width + 30) / 20;
	for (int i = 1; i < meta.width; i += increment) {
		myGraph.addVertex(ID);
		gv->addNode(ID++, i, 0);

		myGraph.addVertex(ID);
		gv->addNode(ID++, i, meta.height);
	}

	// Left and right
	increment = (meta.height + 30) / 20;
	for (int j = 1; j < meta.height; j += increment) {
		myGraph.addVertex(ID);
		gv->addNode(ID++, 0, j);

		myGraph.addVertex(ID);
		gv->addNode(ID++, meta.width, j);
	}
}



//void test_load_map(std::string path);



void test_load_meta(std::string path, MapMetaData &meta) {
	try {
		load_meta(path, meta);
		std::cout << "Min Longitude: " << meta.min_longitude << std::endl;
		std::cout << "Max Longitude: " << meta.max_longitude << std::endl;
		std::cout << "Min Latitude: " << meta.min_latitude << std::endl;
		std::cout << "Max Latitude: " << meta.max_latitude << std::endl;
		std::cout << "Nodes: " << meta.nodes << std::endl;
		std::cout << "Edges: " << meta.edges << std::endl;
		std::cout << "Load Meta Successful" << std::endl;
	} catch (std::exception &e) {
		std::cout << "Load Meta Failed\n" << e.what() << std::endl;
	}
}



void test_load_nodes(std::string path, MapMetaData &meta) {
	try {
		GraphViewer* gv;
		Graph<int> myGraph;
		gv = new GraphViewer(meta.width, meta.height, false);
		std::cout << "Estimated X: " << meta.width << std::endl;
		std::cout << "Estimated Y: " << meta.height << std::endl;
		gv->createWindow(600, 600);
		gv->defineVertexColor("blue");
		gv->defineEdgeColor("black");

		int e = load_nodes(path, meta, gv, myGraph);

		std::cout << "Status load nodes: " << e << std::endl;

		auto vertexSet = myGraph.getVertexSet();
		std::cout << "Number of Nodes: " << vertexSet.size() << std::endl;

		show_boundaries(meta, gv, myGraph);
		gv->rearrange();
	} catch (std::exception &e) {
		std::cout << "Load Nodes Failed\n" << e.what() << std::endl;
	}
}



void test_load_roads(std::string path, MapMetaData &meta) {
	try {

	} catch (std::exception &e) {
		std::cout << "Load Roads Failed\n" << e.what() << std::endl;
	}
}

//void test_load_subroads(std::string path, MapMetaData &meta);


