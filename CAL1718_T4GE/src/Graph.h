#pragma once

#include "graphviewer.h"
#include "MutablePriorityQueue.h"

#include <limits>
#include <chrono>
#include <map>

using namespace std;

#define GRAPH_VIEWER_WIDTH     ((int)600)
#define GRAPH_VIEWER_HEIGHT    ((int)600)

#define VERTEX_CLEAR_COLOR     BLUE
#define EDGE_CLEAR_COLOR       BLACK

#define ACCIDENTED_COLOR       RED
#define SELECTED_COLOR         MAGENTA
#define UNREACHABLE_COLOR      YELLOW
#define PATH_COLOR             GREEN
#define NEXT_PATH_COLOR        CYAN

#define VERTEX_SIZE            20
#define SMALL_VERTEX_SIZE      10

#define MAX_WAIT               10000 // 10 seconds

class Edge;
class Graph;
class Vertex;
class Road;
class Subroad;

extern Graph* graph;

using microtime = chrono::duration<int64_t,micro>::rep;
using color = string;

//////////////////////////
/////// Class Graph //////
//////////////////////////

class Graph {
	const int width, height;
	vector<Vertex*> vertexSet;
	vector<Vertex*> accidentedVertexSet;
	GraphViewer *gv;
	double scale;
	map<string,Road *> roadsInfo;
	map<int, Edge *> subRoadsInfo;

	mutable struct Mode {
		bool vertexLabels = false;
		bool edgeLabels = false;
	} show;

	///// ***** Auxiliary
	bool withinBounds(int x, int y) const;
	void moveToVertexSet(Vertex *v);
	void moveToAccidentedVertexSet(Vertex *v);

public:
	///// ***** Visual GraphViewer API
	// Update Screen
	void update() const;
	void rearrange() const;

	// Editing Color
	bool setVertexColor(Vertex *v, color color) const;
	bool setVertexDefaultColor(Vertex *v) const;
	bool setEdgeColor(Edge *e, color color) const;
	bool setEdgeDefaultColor(Edge *e) const;
	void resetVertexColors() const;
	void resetEdgeColors() const;

	// Phantom Display
	void showBoundaries() const;

	// General Display
	void showAllVertexLabels() const;
	void hideAllVertexLabels() const;
	void showAllEdgeLabels() const;
	void hideAllEdgeLabels() const;
	void showEdgeSimulationLabels() const;

	// Advanced API
	bool setBackground(string path) const;
	bool straightEdges() const;
	/////

	///// ***** Animation
	void animatePath(vector<Vertex*> path, int interval = 100, color = PATH_COLOR, bool last = false) const;
	void clearPath(vector<Vertex*> path, int interval = 0, bool last = false) const;
	/////

	///// ***** Constructors and destructor
	explicit Graph(int width, int height, double scale = 1);
	~Graph();
	/////

	///// ***** Vertex CRUD
	bool addVertex(int id, int x, int y, bool accidented = false);
	bool addVertex(Vertex *v);

	Vertex *findVertex(int id) const;
	Vertex *getVertex(int id) const;
	int getNumVertices() const;
	int getNumAccidentedVertices() const;
	int getTotalVertices() const;
	vector<Vertex*> getVertexSet() const;
	vector<Vertex*> getAccidentedVertexSet() const;
	vector<Vertex*> getAllVertexSet() const;
	vector<Vertex*> getPath(Vertex* origin, Vertex* dest) const;
	double distance(Vertex *v1, Vertex *v2) const;
	double length(Edge *e) const;
	bool connectedTo(Vertex *v1, Vertex *v2, bool bothways = false) const;

	bool fixVertex(Vertex *v);
	bool accidentVertex(Vertex *v);

	void removeVertex(int id);
	void removeVertex(Vertex *v);
	/////

	///// ***** Edge CRUD
	bool addEdge(int eid, int sourceId, int destId, Subroad* road, bool accidented = false);
	bool addEdge(int eid, Vertex *vsource, Vertex *vdest, Subroad* road, bool accidented = false);
	bool addEdge(Edge *e);

	Edge *findEdge(int eid) const;
	Edge *getEdge(int eid) const;
	Edge *findEdge(int sourceId, int destId) const;
	Edge *getEdge(int sourceId, int destId) const;

	bool fixEdge(Edge *e);
	bool accidentEdge(Edge *e);

	void removeEdge(int eid);
	void removeEdge(Edge *e);
	/////

	void generateGraphNewStatus(vector<Vertex*> path);

	///// ***** Algorithms
	void clear() const;

	// Breadth First Search. Find reachable nodes
	void bfs(Vertex *origin);

	// Greedy Best-First Search.
	void gbfsDist(Vertex *origin, Vertex *destination, microtime *time = nullptr);

	// Dijkstra by distance, single source. Find shortest paths to all other vertices
	void dijkstraDist(Vertex *origin, microtime *time = nullptr);

	// Dijkstra by distance, with destination. Find shortest paths destination vertex
	void dijkstraDist(Vertex *vsource, Vertex *vdest, microtime *time = nullptr);

	// A* by distance. Find shortest path to destination vertex only
	void AstarDist(Vertex *vsource, Vertex *vdest, microtime *time = nullptr);

	// Dijkstra by travel time, with destination. Find the quickest path to destination vertex
	void dijkstraSimulation(Vertex *vsource, Vertex *vdest, microtime *time = nullptr);
	/////

	///// ***** Operations
	map<string, Road *> & getRoadsInfo();
	map<int, Edge *> & getSubRoadsInfo();
	friend class Vertex;
	friend class Edge;

};





//////////////////////////
////// Class Vertex //////
//////////////////////////

class Vertex {
	const int id;
	const int x, y;
	bool accidented;
	long double cost = 0;
	long double priority = 0;
	Vertex* path = nullptr;
	vector<Edge*> adj;
	vector<Edge*> accidentedAdj;
	Graph* graph = nullptr;

	void moveToAdj(Edge *e);
	void moveToAccidentedAdj(Edge *e);

public:
	int queueIndex = 0;

	///// ***** Constructor
	void _sgraph(Graph* graph);
	explicit Vertex(int id, int x, int y, bool accidented = false);
	~Vertex();

	///// ***** Vertex CRUD
	int getID() const;
	int getX() const; // could also make x a const public
	int getY() const; // could also make y a const public
	double getCost() const;
	bool isAdjacentTo(Vertex *dest) const;
	bool connectsTo(Vertex *dest) const;
	bool isAccidented() const;
	vector<Edge*> getAdj() const;
	vector<Edge*> getAccidentedAdj() const;
	Vertex *getPath() const;
	/////

	///// ***** Edge CRUD
	bool addEdge(Edge *e);

	Edge *findEdge(int eid) const;
	Edge *getEdge(int eid) const;
	Edge *findEdge(Vertex *vdest) const;
	Edge *getEdge(Vertex *vdest) const;

	bool fix();
	bool accident();
	bool fixEdge(Vertex *vdest);
	bool accidentEdge(Vertex *vdest);

	void removeEdge(int eid);
	void removeEdge(Vertex *dest);
	void removeEdge(Edge *edge);
	/////

	///// ***** Operations
	bool operator<(Vertex v) const;
	friend class Graph;
	friend class Edge;
};





//////////////////////////
/////// Class Edge ///////
//////////////////////////

class Edge {
	const int id;
	Vertex *source;
	Vertex *dest;
	bool accidented;
	Graph* graph = nullptr;
	Subroad* subroad;

public:
	///// ***** Constructor
	void _sgraph(Graph* graph);
	explicit Edge(int id, Vertex* vsource, Vertex *vdest, Subroad *subroad, bool accidented = false);

	///// ***** Vertex CRUD
	Vertex *getSource() const;
	Vertex *getDest() const;
	/////

	///// ***** Edge CRUD
	int getID() const;
	double getWeight() const;
	Road *getRoad() const;
	bool isAccidented() const;
	double getDistance() const;
	int getActualCapacity() const;
	int getMaxCapacity() const;
	int calculateAverageSpeed() const;

	bool fix();
	bool accident();
	bool setActualCapacity(int capacity);
	/////

	// ***** Operations
	friend class Graph;
	friend class Vertex;
};





//////////////////////////
/////// Class Road ///////
//////////////////////////

class Road {
	const int id;
	const string name;
	const bool bothways;
	double totalDistance = 0;
	int maxSpeed = 0;

public:
	explicit Road(int rid, string name, bool bothways = false);

	int getID() const;
	string getName() const;
	bool isBidirectional() const;
	double getTotalDistance() const;
	int getMaxSpeed() const;

	bool setTotalDistance(double distance);

	friend class Subroad;
};





//////////////////////////
////// Class Subroad /////
//////////////////////////

class Subroad {
	const double distance;
	Road *const road;

	int actualCapacity = 0;
	int maxCapacity = 0;

public:
	explicit Subroad(double distance, Road* road);

	Road *getRoad() const;
	double getDistance() const;
	int getActualCapacity() const;
	int getMaxCapacity() const;
	int calculateAverageSpeed() const;

	bool setActualCapacity(int actualCapacity);

	friend class Road;
};
