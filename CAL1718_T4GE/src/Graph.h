#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <list>

#include "Graphviewer.h"

using namespace std;

#define GRAPH_VIEWER_WIDTH        ((int)600)
#define GRAPH_VIEWER_HEIGHT       ((int)600)
#define VERTEX_CLEAR_COLOR        BLUE
#define VERTEX_ACCIDENTED_COLOR   RED
#define EDGE_CLEAR_COLOR          BLACK
#define EDGE_ACCIDENTED_COLOR     RED

class Edge;
class Graph;
class Vertex;
class Road;
class Subroad;

//////////////////////////
/////// Class Graph //////
//////////////////////////

class Graph {
	const int width, height;
	vector<Vertex*> vertexSet;
	vector<Vertex*> accidentedVertexSet;
	GraphViewer *gv;

	// ***** Auxiliary
	bool withinBounds(int x, int y) const;
	void moveToVertexSet(Vertex *v);
	void moveToAccidentedVertexSet(Vertex *v);

public:
	///// ***** Visual API
	void update() const;
	void rearrange() const;

	bool setVertexLabel(int id, string label) const;
	bool setVertexColor(int id, string color) const;
	bool defineVertexColor(string color) const;

	bool setEdgeLabel(int eid, string label) const;
	bool setEdgeWeight(int id, int weight) const;
	bool setEdgeFlow(int id, int flow) const;
	bool setEdgeColor(int eid, string color) const;
	bool defineEdgeColor(string color) const;

	bool setEdgeThickness(int id, int thickness) const;
	bool setBackground(string path) const;
	/////

	///// ***** Constructors and destructor
	Graph(int width, int height);
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
	// Dijkstra by distance, single source. Find shortest paths to all other vertices
	bool dijkstraDist(Vertex *vsource, Vertex *vdest); // or int version
	// A* by distance. Find shortest path to destination vertex only
	bool AstarDist(Vertex *vsource, Vertex *vdest);
	/////

	///// ***** Operations
	ostream& operator<<(ostream &out) const;
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
	vector<Edge*> adj;
	vector<Edge*> accidentedAdj;
	Graph* graph = nullptr;

	void moveToAdj(Edge *e);
	void moveToAccidentedAdj(Edge *e);

public:
	///// ***** Constructor
	void _sgraph(Graph* graph);
	explicit Vertex(int id, int x, int y, bool accidented = false);
	~Vertex();

	///// ***** Vertex CRUD
	int getID() const;
	int getX() const; // could also make x a const public
	int getY() const; // could also make y a const public
	double distanceTo(Vertex *dest) const;
	bool isAdjacentTo(Vertex *dest) const;
	bool connectsTo(Vertex *dest) const;
	bool isAccidented() const;
	vector<Edge*> getAdj() const;
	vector<Edge*> getAccidentedAdj() const;
	Vertex *getPredecessor() const;
	vector<Vertex*> getPath() const;
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
	bool operator<(Vertex *v) const; // MutablePriorityQueue
	bool operator>(Vertex *v) const;
	bool operator==(Vertex *v) const;
	bool operator!=(Vertex *v) const;
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
	bool operator<(Edge *e) const; // MutablePriorityQueue
	bool operator>(Edge *e) const;
	bool operator==(Edge *e) const;
	bool operator!=(Edge *e) const;
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
	Road(int rid, string name, bool bothways = false);

	///// ***** Road CRUD
	// C
	// R
	int getID() const;
	string getName() const;
	bool isBidirectional() const;
	double getTotalDistance() const;
	int getAverageSpeed() const;
	// U
	bool setTotalDistance(double distance);
	// D
	/////

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
	Subroad(double distance, Road* road);

	///// ***** Subroad CRUD
	Road *getRoad() const;
	double getDistance() const;
	int getActualCapacity() const;
	int getMaxCapacity() const;
	int calculateAverageSpeed() const;

	bool setActualCapacity(int actualCapacity);
	/////

	friend class Road;
};
