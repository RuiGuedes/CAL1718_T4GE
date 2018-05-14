#pragma once

#include "Graph.h"

using namespace std;

static const string meta_suffix = "_meta.txt";
static const string nodes_suffix = "_nodes.txt";
static const string roads_suffix = "_roads.txt";
static const string subroads_suffix = "_subroads.txt";
static constexpr long double default_density = 0.000100;


struct MetaData {
	long double min_longitude, max_longitude;
	long double min_latitude, max_latitude;
	double scale;
	int nodes, edges;
	int width = 0, height = 0;
	bool boundaries = false;
	bool background = false;
	bool oneway = false, bothways = false;
	bool straightedges = false;
	string background_filename = "";
	long double density = default_density;
};

void showBoundaries(MetaData &meta, Graph* graph);

bool checkFilename(string filename);

int loadMap(string filename, Graph* &graph, bool boundaries = false);

int loadMeta(string filename, MetaData &meta);

int loadNodes(string filename, MetaData &meta, Graph* graph);

int loadRoads(string filename, MetaData &meta, Graph* graph);

int loadSubroads(string filename, MetaData &meta, Graph* graph);

int testLoadMeta(string path);

int testLoadNodes(string path);

int testLoadRoads(string path);

int testLoadSubroads(string path);

int testLoadMap(string path);

int testNewMap(string path);
