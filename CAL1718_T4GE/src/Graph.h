/*
 * Graph.h
 */
#pragma once

#include <vector>
#include <queue>
#include <list>

#include "Graphviewer.h"
#include "MutablePriorityQueue.h"

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

//////////////////////////
/////// Class Graph //////
//////////////////////////

class Graph {
	const int width, height;
	GraphViewer *gv;
	vector<Vertex*> vertexSet;
	vector<Vertex*> accidentedVertexSet;

	// ***** Auxiliary
	bool withinBounds(int x, int y) const;
	void moveToVertexSet(Vertex *v);
	void moveToAccidentedVertexSet(Vertex *v);

public:
	// ***** Visual API
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

	// ***** Constructors and destructor
	Graph(int width, int height);
	~Graph();

	// ***** Vertex CRUD
	// C
	bool addVertex(int id, int x, int y, bool accidented = false);
	bool addVertex(Vertex *v);
	// R
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
	// U
	bool fixVertex(Vertex *v);
	bool accidentVertex(Vertex *v);
	// D
	void removeVertex(int id);
	void removeVertex(Vertex *v);

	// ***** Edge CRUD
	// C
	bool addEdge(int eid, int sourceId, int destId, double weight/*, Road* road*/, bool accidented = false);
	bool addEdge(int eid, Vertex *vsource, Vertex *vdest, double weight/*, Road* road*/, bool accidented = false);
	bool addEdge(Edge *e);
	// R
	Edge *findEdge(int eid) const;
	Edge *getEdge(int eid) const;
	Edge *findEdge(int sourceId, int destId) const;
	Edge *getEdge(int sourceId, int destId) const;
	// U
	bool fixEdge(Edge *e);
	bool accidentEdge(Edge *e);
	// D
	bool removeEdge(int eid);
	bool removeEdge(Edge *e);

	// ***** Algorithms
	// Breadth First Search, single source. Find minimal paths to all other vertices
	bool bfs(Vertex *vsource);
	// Breadth First Search, with destination. Find minimal path to destination vertex only
	bool bfs(Vertex *vsource, Vertex *vdest);
	// Dijkstra by distance, single source. Find shortest paths to all other vertices
	bool dijkstraDist(Vertex *vsource); // or int version
	// Dijkstra by distance, with destination. Find shortest path to destination vertex only
	bool dijkstraDist(Vertex *vsource, Vertex *vdest); // or int version
	// A* by distance. Find shortest path to destination vertex only
	bool AstarDist(Vertex *vsource, Vertex *vdest);
	// Floyd-Warshall by distance.
	bool floydWarshallDist();
	// Johnson by distance.
	bool johnsonDist();

	vector<Vertex*> getPath(Vertex *source, Vertex *dest) const;

	// ***** Operations
	ostream& operator<<(ostream &out) const;
	friend class Vertex;
	friend class Edge;
};





//////////////////////////
////// Class Vertex //////
//////////////////////////

class Vertex {
	Graph* graph;
	const int id;
	const int x, y;
	bool accidented;
	vector<Edge*> adj;
	vector<Edge*> accidentedAdj;

	// Algorithm auxiliary
	//bool visited = false;
	//double dist = 0;
	//Vertex *path = nullptr;
	//int queueIndex = 0; // MutablePriorityQueue
	//bool processing = false;

	void moveToAdj(Edge *e);
	void moveToAccidentedAdj(Edge *e);

public:
	// Constructors and destructor
	explicit Vertex(int id, int x, int y, bool accidented = false);
	~Vertex();

	///// ***** Vertex CRUD
	// C
	// R
	int getId() const;
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
	// U
	// D
	/////

	///// ***** Edge CRUD
	// C
	bool addEdge(Edge *e);
	// R
	Edge *findEdge(int eid) const;
	Edge *getEdge(int eid) const;
	Edge *findEdge(Vertex *vdest) const;
	Edge *getEdge(Vertex *vdest) const;
	// U
	bool fix();
	bool accident();
	bool fixEdge(Vertex *vdest);
	bool accidentEdge(Vertex *vdest);
	// D
	void removeEdge(int eid);
	void removeEdge(Vertex *dest);
	void removeEdge(Edge *edge);
	/////

	// ***** Operations
	bool operator<(Vertex *v) const; // MutablePriorityQueue
	bool operator>(Vertex *v) const;
	bool operator==(Vertex *v) const;
	bool operator!=(Vertex *v) const;
	ostream& operator<<(ostream &out) const;
	friend class Edge;
	friend class MutablePriorityQueue<Vertex>;
};





//////////////////////////
/////// Class Edge ///////
//////////////////////////

class Edge {
	Graph* graph;
	const int id;
	Vertex *source;
	Vertex *dest;
	double weight;
	bool accidented;
	// ...
	/*Road *road;*/

public:
	explicit Edge(int id, Vertex* vsource, Vertex *vdest, double weight/*, Road *w*/, bool accidented = false);

	// ***** Vertex CRUD
	// C
	// R
	Vertex *getSource() const;
	Vertex *getDest() const;
	// U
	// D

	// ***** Edge CRUD
	// C
	// R
	int getId() const;
	double getWeight() const;
	bool isAccidented() const;
	// U
	bool fix();
	bool accident();
	void setWeight(double weight);
	// D

	// ***** Operations
	bool operator<(Edge *e) const; // MutablePriorityQueue
	bool operator>(Edge *e) const;
	bool operator==(Edge *e) const;
	bool operator!=(Edge *e) const;
	ostream& operator<<(ostream &out) const;
	friend class MutablePriorityQueue<Vertex>;
};

