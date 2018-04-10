#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <list>
#include <chrono>

#include "Graphviewer.h"
#include "MutablePriorityQueue.h"

using namespace std;

#define GRAPH_VIEWER_WIDTH        ((int)600)
#define GRAPH_VIEWER_HEIGHT       ((int)600)

#define VERTEX_CLEAR_COLOR        BLUE
#define VERTEX_ACCIDENTED_COLOR   RED
#define VERTEX_SELECTED_COLOR     GREEN
#define VERTEX_UNREACHABLE_COLOR  YELLOW

#define EDGE_CLEAR_COLOR          BLACK
#define EDGE_ACCIDENTED_COLOR     RED
#define EDGE_SELECTED_COLOR       GREEN
#define EDGE_UNREACHABLE_COLOR    YELLOW

#define INF std::numeric_limits<double>::max()

class Edge;
class Graph;
class Vertex;
class Road;
class Subroad;

extern bool graphLoaded;
extern bool showEdgeLabels;
extern bool showEdgeWeights;
extern bool showEdgeFlows;

using microtime = chrono::duration<int64_t,micro>::rep;

//////////////////////////
/////// Class Graph //////
//////////////////////////

class Graph {
	const int width, height;
	vector<Vertex*> vertexSet;
	vector<Vertex*> accidentedVertexSet;
	GraphViewer *gv;
	double scale;

	// ***** Auxiliary
	bool withinBounds(int x, int y) const;
	void moveToVertexSet(Vertex *v);
	void moveToAccidentedVertexSet(Vertex *v);

public:
	///// ***** Visual API
	void update() const;
	void rearrange() const;

	bool setVertexLabel(Vertex *v, string label) const;
	bool setVertexColor(Vertex *v, string color) const;
	bool defineVertexColor(string color) const;
	bool setVertexDefaultColor(Vertex *v) const;

	bool setEdgeLabel(Edge *e, string label) const;
	bool setEdgeWeight(Edge *e, int weight) const;
	bool setEdgeFlow(Edge *e, int flow) const;
	bool setEdgeColor(Edge *e, string color) const;
	bool defineEdgeColor(string color) const;
	bool setEdgeDefaultColor(Edge *e) const;

	bool setEdgeThickness(Edge *e, int thickness) const;
	bool setBackground(string path) const;
	/////

	///// ***** Constructors and destructor
	explicit Graph(int width, int height, double scale = 1);
	~Graph();

	///// ***** Vertex
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
	vector<Vertex*> getPathRoad(Vertex* origin, Vertex* dest) const;
	double distance(Vertex *v1, Vertex *v2) const;
	double length(Edge *e) const;
	bool connectedTo(Vertex *v1, Vertex *v2, bool bothways = false) const;
	bool edgeIsAccidented(Edge *e) const;
	bool vertexIsAccidented(Vertex* v) const;

	bool fixVertex(Vertex *v);
	bool accidentVertex(Vertex *v);

	void removeVertex(int id);
	void removeVertex(Vertex *v);
	/////

	///// ***** Edge CRUD
	bool addEdge(int eid, int sourceId, int destId, double weight, Subroad* road, bool accidented = false);
	bool addEdge(int eid, Vertex *vsource, Vertex *vdest, double weight, Subroad* road, bool accidented = false);
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

	///// ***** Algorithms
	// Breadth First Search. Find reachable nodes
	void bfs(Vertex *origin);

	// Greedy Best-First Search
	void gbfsDist(Vertex *origin, Vertex *destination, microtime *time = nullptr);

	// Dijkstra by distance, single source. Find shortest paths to all other vertices
	Vertex * initSingleSource(const int &origin);
	bool relax(Vertex *v, Vertex *w, double weight);
	void dijkstraDist(Vertex *origin, microtime *time = nullptr);

	// Dijkstra by distance, with destination. Find shortest paths destination vertex
	void dijkstraDist(Vertex *vsource, Vertex *vdest, microtime *time = nullptr);

	// A* by distance. Find shortest path to destination vertex only
	void AstarDist(Vertex *vsource, Vertex *vdest, microtime *time = nullptr);

	// Greedy Best-First Search by travel time.
	void gbfsSimulation(Vertex *vsource, Vertex *vdest, microtime *time = nullptr);

	// Dijkstra by travel time, with destination. Find the quickest path to destination vertex
	void dijkstraSimulation(Vertex *vsource, Vertex *vdest, microtime *time = nullptr);
	/////

	///// ***** Operations
	friend ostream& operator<<(ostream &out, Graph* graph);
	friend class Vertex;
	friend class Edge;


	void generateGraphNewStatus();
};





//////////////////////////
////// Class Vertex //////
//////////////////////////

class Vertex {
	const int id;
	const int x, y;
	bool accidented;
	double dist = 0;
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
	double getDist() const;
	double distanceTo(Vertex *dest) const;
	bool isAdjacentTo(Vertex *dest) const;
	bool connectsTo(Vertex *dest) const;
	bool isAccidented() const;
	vector<Edge*> getAdj() const;
	vector<Edge*> getAccidentedAdj() const;
	Vertex *getPredecessor() const;
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
	friend ostream& operator<<(ostream &out, Vertex* v);
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
	double weight;
	bool accidented;
	Graph* graph = nullptr;
	Subroad* subroad;

public:
	///// ***** Constructor
	void _sgraph(Graph* graph);
	explicit Edge(int id, Vertex* vsource, Vertex *vdest, double weight, Subroad *subroad, bool accidented = false);

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
	void setWeight(double weight);
	bool setActualCapacity(int capacity);
	/////

	// ***** Operations
	friend ostream& operator<<(ostream &out, Edge* e);
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
	int averageSpeed = 0;

public:
	explicit Road(int rid, string name, bool bothways = false);

	int getID() const;
	string getName() const;
	bool isBidirectional() const;
	double getTotalDistance() const;
	int getAverageSpeed() const;

	bool setTotalDistance(double distance);

	friend ostream& operator<<(ostream& out, Road* r);
	friend class Subroad;
};





/////////////////////////
///// Class Subroad /////
/////////////////////////

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
