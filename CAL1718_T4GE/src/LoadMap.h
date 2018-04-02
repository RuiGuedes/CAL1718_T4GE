#ifndef SRC_LOADMAP_H_
#define SRC_LOADMAP_H_

static const string meta_suffix = "_meta.txt";
static const string nodes_suffix = "_nodes.txt";
static const string roads_suffix = "_roads.txt";
static const string subroads_suffix = "_subroads.txt";
static constexpr long double default_density = 0.000250;

struct MapMetaData {
	long double min_longitude, max_longitude;
	long double min_latitude, max_latitude;
	int nodes, edges;
	int width = 0, height = 0;
	long double density = default_density;
};

int loadMap(string filename, GraphViewer* &gv, Graph<int> &myGraph, bool boundaries = false);

int loadMeta(string filename, MapMetaData &meta);

int loadNodes(string filename, MapMetaData &meta, GraphViewer* &gv, Graph<int> &myGraph);

int loadRoads(string filename, MapMetaData &meta, GraphViewer* &gv, Graph<int> &myGraph);

int loadSubroads(string filename, MapMetaData &meta, GraphViewer* &gv, Graph<int> &myGraph);

int testLoadMap(string path);

int testLoadMeta(string path);

int testLoadNodes(string path);

int testLoadRoads(string path);

int testLoadSubroads(string path);


#endif /* SRC_LOADMAP_H_ */
