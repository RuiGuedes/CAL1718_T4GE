#ifndef SRC_LOAD_MAP_H_
#define SRC_LOAD_MAP_H_

static const std::string meta_suffix = "_meta.txt";
static const std::string nodes_suffix = "_nodes.txt";
static const std::string roads_suffix = "_roads.txt";
static const std::string subroads_suffix = "_subroads.txt";
static constexpr long double default_density = 0.000250;

struct MapMetaData {
	long double min_longitude, max_longitude;
	long double min_latitude, max_latitude;
	int nodes, edges;
	int width = 0, height = 0;
	long double density = default_density;
};

int load_map(std::string filename, GraphViewer* &gv, Graph<int> &myGraph, bool boundaries = false);

int load_meta(std::string filename, MapMetaData &meta);

int load_nodes(std::string filename, MapMetaData &meta, GraphViewer* &gv, Graph<int> &myGraph);

int load_roads(std::string filename, MapMetaData &meta, GraphViewer* &gv, Graph<int> &myGraph);

int load_subroads(std::string filename, MapMetaData &meta, GraphViewer* &gv, Graph<int> &myGraph);

int test_load_map(std::string path);

int test_load_meta(std::string path);

int test_load_nodes(std::string path);

int test_load_roads(std::string path);

int test_load_subroads(std::string path);


#endif /* SRC_LOAD_MAP_H_ */
