#include "Graph.h"

#include <iostream>
#include <deque>
#include <algorithm>
#include <math.h>


///// ***** Auxiliary

/*
 * @brief (Private) Checks if a pair of integers is
 * within the bounds of the graph area
 * @return True if it is, false otherwise
 */
bool Graph::withinBounds(int x, int y) const {
	return x >= 0 && y >= 0 && x <= width && y <= height;
}

/*
 * @brief (Private) Moves vertex v from
 * accidentedVertexSet to vertexSet.
 * v is in accidentedVertexSet.
 */
void Graph::moveToVertexSet(Vertex *v) {
	accidentedVertexSet.erase(remove(accidentedVertexSet.begin(),accidentedVertexSet.end(), v), accidentedVertexSet.end());
	vertexSet.push_back(v);
	update();
}

/*
 * @brief (Private) Moves vertex v from
 * vertexSet to accidentedVertexSet.
 * v is in vertexSet.
 */
void Graph::moveToAccidentedVertexSet(Vertex *v) {
	vertexSet.erase(remove(vertexSet.begin(),vertexSet.end(), v),vertexSet.end());
	accidentedVertexSet.push_back(v);
	update();
}

/**
 * @brief Clear previous invocation of a pathing
 * algorithm.
 */
void Graph::clear() const {
	for (auto v : vertexSet) {
		v->priority = 0;
		v->cost = 0;
		v->path = nullptr;
	}
	for (auto v : accidentedVertexSet) {
		v->priority = 0;
		v->cost = 0;
		v->path = nullptr;
	}
}


///// ***** Visual GraphViewer API

/*
 * GRAPHVIEWER
 * @brief Updates the graph window with the new info
 */
void Graph::update() const {
	gv->rearrange();
}

/*
 * GRAPHVIEWER
 * @brief Updates the graph window with the new info
 */
void Graph::rearrange() const {
	gv->rearrange();
}

/*
 * GRAPHVIEWER
 * @brief Define the color of a particular vertex
 */
bool Graph::setVertexColor(Vertex *v, string color) const {
	if (v == nullptr) return false;
	return gv->setVertexColor(v->getID(), color);
}

/*
 * GRAPHVIEWER
 * @brief Reset the vertex's color to the expected color
 * VERTEX_ACCIDENTED_COLOR if the vertex is accidented
 * VERTEX_CLEAR_COLOR if the vertex is clear
 */
bool Graph::setVertexDefaultColor(Vertex *v) const {
	if (v == nullptr) return false;
	if (v->isAccidented()) {
		return setVertexColor(v, ACCIDENTED_COLOR);
	} else {
		return setVertexColor(v, VERTEX_CLEAR_COLOR);
	}
}

/*
 * GRAPHVIEWER
 * @brief Define the color of a particular edge
 */
bool Graph::setEdgeColor(Edge *e, string color) const {
	if (e == nullptr) return false;
	return gv->setEdgeColor(e->getID(), color);
}

/*
 * GRAPHVIEWER
 * @brief Reset the edge's color to the expected color
 * EDGE_ACCIDENTED_COLOR if the edge is accidented
 * EDGE_CLEAR_COLOR if the edge is clear
 */
bool Graph::setEdgeDefaultColor(Edge *e) const {
	if (e == nullptr) return false;
	if (e->isAccidented()) {
		return setEdgeColor(e, ACCIDENTED_COLOR);
	} else {
		return setEdgeColor(e, EDGE_CLEAR_COLOR);
	}
}

/**
 * GRAPHVIEWER
 * @brief Reset all vertices back to their default color
 * VERTEX_ACCIDENTED_COLOR if the vertex is accidented
 * VERTEX_CLEAR_COLOR if the vertex is clear
 */
void Graph::resetVertexColors() const {
	for (auto v : vertexSet) {
		setVertexDefaultColor(v);
	}
	for (auto v : accidentedVertexSet) {
		setVertexDefaultColor(v);
	}
}

/**
 * GRAPHVIEWER
 * @brief Reset all edges back to their default color
 * EDGE_ACCIDENTED_COLOR if the edge is accidented
 * EDGE_CLEAR_COLOR if the edge is clear
 */
void Graph::resetEdgeColors() const {
	for (auto v : vertexSet) {
		for (auto e : v->adj) {
			setEdgeDefaultColor(e);
		}
		for (auto e : v->accidentedAdj) {
			setEdgeDefaultColor(e);
		}
	}
	for (auto v : accidentedVertexSet) {
		for (auto e : v->adj) {
			setEdgeDefaultColor(e);
		}
		for (auto e : v->accidentedAdj) {
			setEdgeDefaultColor(e);
		}
	}
}

/**
 * GRAPHVIEWER
 * Adds phantom vertices not present in the
 * vertex set around the GraphViewer, delimiting
 * its boundaries.
 */
void Graph::showBoundaries() const {
	int ID = -1337;

	// Corners
	gv->addNode(--ID, 0, 0);
	gv->setVertexLabel(ID, "\t");
	gv->setVertexColor(ID, BLACK);
	gv->addNode(--ID, width, 0);
	gv->setVertexLabel(ID, "\t");
	gv->setVertexColor(ID, BLACK);
	gv->addNode(--ID, 0, height);
	gv->setVertexLabel(ID, "\t");
	gv->setVertexColor(ID, BLACK);
	gv->addNode(--ID, width, height);
	gv->setVertexLabel(ID, "\t");
	gv->setVertexColor(ID, BLACK);

	int increment;

	// Top and bottom
	increment = width / ((width + 1000) / 200);
	for (int i = increment; i < width - (increment / 3); i += increment) {
		gv->addNode(--ID, i, 0);
		gv->setVertexLabel(ID, "\t");
		gv->setVertexColor(ID, BLACK);
		gv->addNode(--ID, i, height);
		gv->setVertexLabel(ID, "\t");
		gv->setVertexColor(ID, BLACK);
	}

	// Left and right
	increment = height / ((height + 1000) / 200);
	for (int j = increment; j < height - (increment / 3); j += increment) {
		gv->addNode(--ID, 0, j);
		gv->setVertexLabel(ID, "\t");
		gv->setVertexColor(ID, BLACK);
		gv->addNode(--ID, width, j);
		gv->setVertexLabel(ID, "\t");
		gv->setVertexColor(ID, BLACK);
	}

}

/**
 * GRAPHVIEWER
 * Reshows all vertex labels
 */
void Graph::showAllVertexLabels() const {
	show.vertexLabels = true;
	for (auto v : vertexSet) {
		gv->setVertexLabel(v->getID(), to_string(v->getID()));
	}
	for (auto v : accidentedVertexSet) {
		gv->setVertexLabel(v->getID(), to_string(v->getID()));
	}
}

/**
 * GRAPHVIEWER
 * Hides all vertex labels
 */
void Graph::hideAllVertexLabels() const {
	show.vertexLabels = false;
	for (auto v : vertexSet) {
		gv->clearVertexLabel(v->getID());
	}
	for (auto v : accidentedVertexSet) {
		gv->clearVertexLabel(v->getID());
	}
}

/**
 * GRAPHVIEWER
 * Reshows all edge labels
 */
void Graph::showAllEdgeLabels() const {
	show.edgeLabels = true;
	for (auto v : vertexSet) {
		for (auto e : v->adj) {
			gv->setEdgeLabel(e->getID(), to_string(e->getID()));
		}
		for (auto e : v->accidentedAdj) {
			gv->setEdgeLabel(e->getID(), to_string(e->getID()));
		}
	}
	for (auto v : accidentedVertexSet) {
		for (auto e : v->adj) {
			gv->setEdgeLabel(e->getID(), to_string(e->getID()));
		}
		for (auto e : v->accidentedAdj) {
			gv->setEdgeLabel(e->getID(), to_string(e->getID()));
		}
	}
}

/**
 * GRAPHVIEWER
 * Hides all edge labels
 */
void Graph::hideAllEdgeLabels() const {
	show.edgeLabels = false;
	for (auto v : vertexSet) {
		for (auto e : v->adj) {
			gv->clearEdgeLabel(e->getID());
		}
		for (auto e : v->accidentedAdj) {
			gv->clearEdgeLabel(e->getID());
		}
	}
	for (auto v : accidentedVertexSet) {
		for (auto e : v->adj) {
			gv->clearEdgeLabel(e->getID());
		}
		for (auto e : v->accidentedAdj) {
			gv->clearEdgeLabel(e->getID());
		}
	}
}

/**
 * GRAPHVIEWER
 * Shows all edge labels in the format
 * time - cars / max cars
 */
void Graph::showEdgeSimulationLabels() const {
	static const auto lambda = [](Edge* e) -> string {
		return to_string(e->getWeight()) + " : ("
			 + to_string(e->getActualCapacity()) + "/"
			 + to_string(e->getMaxCapacity()) + ")";
	};

	show.edgeLabels = true;
	for (auto v : vertexSet) {
		for (auto e : v->adj) {
			gv->setEdgeLabel(e->getID(), lambda(e));
		}
		for (auto e : v->accidentedAdj) {
			gv->setEdgeLabel(e->getID(), lambda(e));
		}
	}
	for (auto v : accidentedVertexSet) {
		for (auto e : v->adj) {
			gv->setEdgeLabel(e->getID(), lambda(e));
		}
		for (auto e : v->accidentedAdj) {
			gv->setEdgeLabel(e->getID(), lambda(e));
		}
	}
}



bool Graph::setBackground(string path) const {
	return gv->setBackground(path);
}

bool Graph::straightEdges() const {
	return gv->defineEdgeCurved(false);
}



///// ***** Animation

void Graph::animatePath(vector<Vertex*> path, int interval, color color, bool last) const {
	if (path.empty()) return;

	for (unsigned int i = 1; i < path.size(); ++i) {
		if (i > 1 && interval > 5) Sleep(interval);
		setVertexColor(path.at(i), color);
		setEdgeColor(path.at(i - 1)->findEdge(path.at(i)), color);
		rearrange();
	}

	if (last) setVertexColor(path.back(), SELECTED_COLOR);
	rearrange();
}

void Graph::clearPath(vector<Vertex*> path, int interval, bool last) const {
	if (path.empty()) return;

	for (unsigned int i = 1; i < path.size(); ++i) {
		if (i > 1 && interval > 5) Sleep(interval);
		setVertexDefaultColor(path.at(i));
		setEdgeDefaultColor(path.at(i - 1)->findEdge(path.at(i)));
		rearrange();
	}

	if (last) setVertexColor(path.back(), SELECTED_COLOR);
	rearrange();
}



/*
 * @brief Graph constructor, taking
 * display width and display height in grid entries
 */
Graph::Graph(int width, int height, double scale): width(width), height(height), scale(scale) {
	gv = new GraphViewer(width, height, false);
	gv->createWindow(GRAPH_VIEWER_WIDTH, GRAPH_VIEWER_HEIGHT);
	gv->defineVertexColor(VERTEX_CLEAR_COLOR);
	gv->defineEdgeColor(EDGE_CLEAR_COLOR);
}

/*
 * @brief Graph destructor, destroys
 * GraphViewer, vertices and edges
 */
Graph::~Graph() {
	gv->closeWindow();
	for (auto vertex : vertexSet) {
		delete vertex;
	}
	for (auto vertex : accidentedVertexSet) {
		delete vertex;
	}
	delete gv;
}

/*
 * @brief Adds a vertex with the given data to the
 * appropriate vertex set of the graph
 *
 * This automatically incorporates the vertex into
 * GraphViewer as expected
 * @throws logic_error if Repeated vertex id
 * @throws out_of_range if Vertex out of graph bounds
 */
bool Graph::addVertex(int id, int x, int y, bool accidented) {
	if (findVertex(id)) {
		throw std::logic_error("Repeated vertex id " + to_string(id));
	} else if (!withinBounds(x, y)) {
		throw std::out_of_range("Vertex out of graph bounds");
	} else {
		return addVertex(new Vertex(id, x, y, accidented));
	}
}

/*
 * @brief Adds an already created vertex to the
 * appropriate vertex set of the graph
 *
 * This automatically incorporates the vertex into
 * GraphViewer as expected
 * @throws logic_error if Repeated vertex id
 * @throws out_of_range if Vertex out of graph bounds
 */
bool Graph::addVertex(Vertex* v) {
	if (findVertex(v->getID())) {
		throw std::logic_error("Repeated vertex id " + to_string(v->getID()));
	} else if (!withinBounds(v->getX(), v->getY())) {
		throw std::out_of_range("Vertex out of graph bounds");
	} else {
		int id = v->getID();
		v->_sgraph(this);
		if (v->isAccidented()) {
			accidentedVertexSet.push_back(v);
			gv->setVertexColor(id, ACCIDENTED_COLOR);
		} else {
			vertexSet.push_back(v);
		}
		gv->addNode(id, v->getX(), v->getY());

		// * Set Vertex Label
		if (show.vertexLabels)
			gv->setVertexLabel(id, to_string(id));

		gv->setVertexSize(id, VERTEX_SIZE);

		// No graph->update()
		return true;
	}
}

/*
 * @brief Returns a vertex with a given id
 * @return The vertex if found, nullptr otherwise
 */
Vertex* Graph::findVertex(int id) const {
	return getVertex(id);
}

/*
 * @brief Returns a vertex with a given id
 * @return The vertex if found, nullptr otherwise
 */
Vertex* Graph::getVertex(int id) const {
	const auto getId = [&id](Vertex *v) -> int { return v->getID() == id; };

	// Look for vertex in vertexSet
	auto it = find_if(vertexSet.cbegin(), vertexSet.cend(), getId);
	if (it != vertexSet.cend())
		return *it;

	// Look for vertex in accidentedVertexSet
	it = find_if(accidentedVertexSet.cbegin(), accidentedVertexSet.cend(), getId);
	if (it != accidentedVertexSet.cend())
		return *it;

	// Vertex not found
	return nullptr;
}

/*
 * @brief Returns the number of clear vertices
 */
int Graph::getNumVertices() const {
	return vertexSet.size();
}

/*
 * @brief Returns the number of accidented vertices
 */
int Graph::getNumAccidentedVertices() const {
	return accidentedVertexSet.size();
}

/*
 * @brief Returns the total number of vertices,
 * both clear and accidented
 */
int Graph::getTotalVertices() const {
	return vertexSet.size() + accidentedVertexSet.size();
}

/*
 * @brief Returns a vector copy of the graph's clear
 * vertex set
 */
vector<Vertex*> Graph::getVertexSet() const {
	return vertexSet;
}

/*
 * @brief Returns a vector copy of the graph's accidented
 * vertex set
 */
vector<Vertex*> Graph::getAccidentedVertexSet() const {
	return accidentedVertexSet;
}

/*
 * @brief Returns a vector with all of the graph's vertices
 */
vector<Vertex*> Graph::getAllVertexSet() const {
	vector<Vertex*> set = vertexSet;
	set.insert(set.end(), accidentedVertexSet.begin(), accidentedVertexSet.end());
	return set;
}

/*
 * @brief Returns the path between two vertices
 */
vector<Vertex*> Graph::getPath(Vertex* origin, Vertex* dest) const {
	vector<Vertex*> res;

	if (origin == nullptr || dest == nullptr || dest->path == nullptr || origin == dest)
		return res;

	for (; dest != nullptr && dest != origin; dest = dest->path)
		res.push_back(dest);

	if (dest == origin) res.push_back(origin);

	if (res.back() != origin) {
		res.clear();
		return res;
	}
	reverse(res.begin(), res.end());
	return res;
}

/* @brief Computes the distance between vertices
 * v1 and v2.
 * @throws invalid_argument if either Vertex is nullptr
 */
double Graph::distance(Vertex* v1, Vertex* v2) const {
	if (v1 == nullptr || v2 == nullptr) {
		throw std::invalid_argument("Vertex not found");
	}
	// long long because of overflow precautions
	long double x1 = v1->getX(), y1 = v1->getY();
	long double x2 = v2->getX(), y2 = v2->getY();
	return scale * sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

/*
 * @brief Computes the length of an edge e,
 * according to the distance between both endpoints
 * @throws invalid_argument if Edge is nullptr
 */
double Graph::length(Edge* e) const {
	if (e == nullptr) {
		throw std::invalid_argument("Edge not found");
	}
	return e->getDistance();
}

/*
 * @brief Checks if two vertices v1 and v2 are adjacent,
 * i.e. there is an edge from v1 to v2
 * @param bothways Check for an edge from v2 to v1 also
 * @return True if there is an edge from v1 to v2
 *         False otherwise
 * @throws invalid_argument if either Vertex is nullptr
 */
bool Graph::connectedTo(Vertex* v1, Vertex* v2, bool bothways) const {
	if (v1 == nullptr || v2 == nullptr) {
		throw std::invalid_argument("Vertex not found");
	}
	return v1->connectsTo(v2) || (bothways && v2->connectsTo(v1));
}

/*
 * @brief Fixes a previously accidented vertex v
 * @return True if vertex was previously accidented
 *         False otherwise
 * @throws invalid_argument if Vertex is nullptr
 */
bool Graph::fixVertex(Vertex* v) {
	if (v == nullptr) {
		throw std::invalid_argument("Vertex not found");
	}
	return v->fix();
}

/*
 * @brief Causes an accident on vertex v
 * @return True if vertex was previously clear
 *         False otherwise
 * @throws invalid_argument if Vertex is nullptr
 */
bool Graph::accidentVertex(Vertex* v) {
	if (v == nullptr) {
		throw std::invalid_argument("Vertex not found");
	}
	return v->accident();
}

/*
 * @brief Removes a vertex with given id from the graph
 * @throws invalid_argument if Vertex not found
 */
void Graph::removeVertex(int id) {
	return removeVertex(findVertex(id));
}

/*
 * @brief Removes a vertex v from the graph
 * @throws invalid_argument if Vertex is nullptr
 */
void Graph::removeVertex(Vertex* v) {
	if (v == nullptr) {
		throw std::invalid_argument("Vertex not found");
	}
	if (v->isAccidented()) {
		auto it = find(accidentedVertexSet.begin(), accidentedVertexSet.end(), v);
		delete v;
		accidentedVertexSet.erase(it);
	} else {
		int id = v->getID();
		auto it = find(vertexSet.begin(), vertexSet.end(), v);
		delete v;
		vertexSet.erase(it);
		gv->removeNode(id);
		// No graph->update()
	}
}

/*
 * @brief Adds an edge with given id starting at vertex
 * with id sourceId and ending in vertex with id destId
 * @return True if edge was successfully added
 *         False if a similar edge already existed
 * @throws invalid_argument if either Vertex not found
 * @throws logic_error if Repeated edge id
 */
bool Graph::addEdge(int eid, int sourceId, int destId,
		Subroad *subroad, bool accidented) {
	return addEdge(eid, findVertex(sourceId),
			findVertex(destId), subroad, accidented);
}

/*
 * @brief Adds and edge with id eid starting at vertex
 * vsource and ending in vertex vdest
 * @return True if edge was successfully added
 *         False if a similar edge already existed
 * @throws invalid_argument if either Vertex is nullptr
 * @throws logic_error if Repeated edge id
 */
bool Graph::addEdge(int eid, Vertex* vsource, Vertex* vdest,
		Subroad *subroad, bool accidented) {
	if (vsource == nullptr || vdest == nullptr) {
		throw std::invalid_argument("Vertex not found");
	}
	if (findEdge(eid) != nullptr) {
		throw std::logic_error("Repeated edge id");
	}
	Edge* e = new Edge(eid, vsource, vdest, subroad, accidented);
	subRoadsInfo.insert({eid, e});
	// Delegate to vertex
	if (vsource->addEdge(e)) {
		return true;
	} else {
		delete e;
		return false;
	}
}

/*
 * @brief Adds and edge previously created to the graph
 * @return True if edge was successfully added
 *         False if a similar edge already existed
 * @throws invalid_argument if specified vertices not found
 * @throws logic_error if Repeated edge id
 */
bool Graph::addEdge(Edge *e) {
	Vertex* vsource = e->getSource();
	Vertex* vdest = e->getDest();
	if (vsource == nullptr || vdest == nullptr) {
		throw std::invalid_argument("Vertex not found");
	}
	if (findEdge(e->getID()) != nullptr) {
		throw std::logic_error("Repeated edge id");
	}
	// Delegate to vertex
	return vsource->addEdge(e);
}

/*
 * @brief Returns a pointer to an edge with id eid
 * @return The edge pointer, or nullptr if not found
 */
Edge* Graph::findEdge(int eid) const {
	return getEdge(eid);
}

/*
 * @brief Returns a pointer to an edge with id eid
 * @return The edge pointer, or nullptr if not found
 */
Edge* Graph::getEdge(int eid) const {
	// Look for the edge in vertexSet first
	// Delegate to vertices
	for (auto vertex : vertexSet) {
		Edge* e = vertex->getEdge(eid);
		if (e != nullptr)
			return e;
	}
	for (auto vertex : accidentedVertexSet) {
		Edge* e = vertex->getEdge(eid);
		if (e != nullptr)
			return e;
	}
	return nullptr;
}

/*
 * @brief Returns a pointer to an edge starting at a
 * vertex with id sourceId and ending at vertex destId
 * @return The edge pointer, or nullptr if not found
 * 			nullptr also if either vertex not found
 */
Edge* Graph::findEdge(int sourceId, int destId) const {
	return getEdge(sourceId, destId);
}

/*
 * @brief Returns a pointer to an edge starting at a
 * vertex with id sourceId and ending at vertex destId
 * @return The edge pointer, or nullptr if not found
 * 			nullptr also if either vertex not found
 */
Edge* Graph::getEdge(int sourceId, int destId) const {
	Vertex* vsource = findVertex(sourceId);
	Vertex* vdest = findVertex(destId);
	if (vsource == nullptr || vdest == nullptr)
		return nullptr;
	// Delegate to vertex
	return vsource->getEdge(vdest);
}

/*
 * @brief Fixes a previously accidented edge e
 * @return True if edge was previously accidented
 *         False otherwise
 * @throws invalid_argument if Edge is nullptr
 */
bool Graph::fixEdge(Edge* e) {
	if (e == nullptr) {
		throw std::invalid_argument("Edge not found");
	}
	// Delegate to edge directly
	return e->fix();
}

/*
 * @brief Causes an accident on an edge e
 * @return True if edge was previously clear
 *         False otherwise
 * @throws invalid_argument if Edge is nullptr
 */
bool Graph::accidentEdge(Edge* e) {
	if (e == nullptr) {
		throw std::invalid_argument("Edge not found");
	}
	// Delegate to edge directly
	return e->accident();
}

/*
 * @brief Removes an edge with id eid
 * @return True if edge was successfully removed
 * @throws invalid_argument if Edge not found
 */
void Graph::removeEdge(int eid) {
	removeEdge(findEdge(eid));
}

/*
 * @brief Removes an edge e
 * @return True if edge was successfully removed
 * @throws invalid_argument if Edge not found
 */
void Graph::removeEdge(Edge* e) {
	if (e == nullptr) {
		throw std::invalid_argument("Edge not found");
	}
	// Delegate to vertex
	e->getSource()->removeEdge(e);
}

map<string, Road *> & Graph::getRoadsInfo() {
	return roadsInfo;
}

map<int,Edge *> & Graph::getSubRoadsInfo() {
	return subRoadsInfo;
}



/*
 * @brief (Private) Moves edge e from accidentedAdj to adj.
 * e is in accidentedAdj.
 */
void Vertex::moveToAdj(Edge *e) {
	accidentedAdj.erase(remove(accidentedAdj.begin(),accidentedAdj.end(), e),accidentedAdj.end());
	adj.push_back(e);
	graph->update();
}

/*
 * @brief (Private) Moves edge e from adj to accidentedAdj.
 * e is in adj.
 */
void Vertex::moveToAccidentedAdj(Edge *e) {
	adj.erase(remove(adj.begin(),adj.end(), e),adj.end());
	accidentedAdj.push_back(e);
	graph->update();
}

void Vertex::_sgraph(Graph* graph) {
	if (this->graph != nullptr) {
		throw std::logic_error("Graph already set");
	}
	this->graph = graph;
}

/*
 * @brief Vertex constructor, taking the vertex id,
 * cartesian coordinates x and y and an initial
 * conditional accidented, defaulting to false
 */
Vertex::Vertex(int id, int x, int y, bool accidented):
															id(id), x(x), y(y), accidented(accidented),
															graph(nullptr) {}

/*
 * @brief Vertex destructor, destroys all its own edges
 */
Vertex::~Vertex() {
}

/*
 * @brief Return the vertex id
 */
int Vertex::getID() const {
	return id;
}

/*
 * @brief Return the vertex X coordinate
 */
int Vertex::getX() const {
	return x;
}

/*
 * @brief Return the vertex Y coordinate
 */
int Vertex::getY() const {
	return y;
}

/*
 * @brief Return the distance to the previous vertex "path".
 */
double Vertex::getCost() const {
	return this->cost;
}

/*
 * @brief Check if the vertex has an edge leading to
 * vertex dest
 * @throws invalid_argument if Vertex is nullptr
 */
bool Vertex::isAdjacentTo(Vertex* dest) const {
	if (dest == nullptr) {
		throw std::invalid_argument("Vertex not found");
	}
	const auto getDest = [&dest](Edge *e) -> bool { return e->getDest() == dest; };
	return any_of(adj.cbegin(), adj.cend(), getDest)
			|| any_of(accidentedAdj.cbegin(), accidentedAdj.cend(), getDest);
}

/*
 * @brief Check if the vertex has an edge leading to
 * vertex dest
 * @throws invalid_argument if Vertex is nullptr
 */
bool Vertex::connectsTo(Vertex* dest) const {
	if (dest == nullptr) {
		throw std::invalid_argument("Vertex not found");
	}
	const auto getDest = [&dest](Edge *e) -> bool { return e->getDest() == dest; };
	return any_of(adj.cbegin(), adj.cend(), getDest)
			|| any_of(accidentedAdj.cbegin(), accidentedAdj.cend(), getDest);
}

/*
 * @brief Check if the vertex is accidented
 */
bool Vertex::isAccidented() const {
	return accidented;
}

/*
 * @brief Return the vertex's clear adjacency vector
 */
vector<Edge*> Vertex::getAdj() const {
	return adj;
}

/*
 * @brief Return the vertex's accidented adjacency vector
 */
vector<Edge*> Vertex::getAccidentedAdj() const {
	return accidentedAdj;
}

/*
 * @brief Return the previous vertex
 * @return The vertex path, possibly empty
 */
Vertex* Vertex::getPath() const {
	return this->path;
}

/*
 * @brief Adds a previously created edge e to the
 * vertex
 *
 * This automatically incorporates the edge into
 * GraphViewer as expected
 * @return True if edge was successfully added
 *         False if there was already an edge in the same
 *         spot (pointing to the same destination)
 * @throws invalid_argument if Edge is nullptr
 */
bool Vertex::addEdge(Edge* e) {
	if (e == nullptr) {
		throw std::invalid_argument("Edge not found");
	}
	if (findEdge(e->getDest())) {
		return false; // An edge already on the spot, delete it first
	}
	if (e->isAccidented()) {
		accidentedAdj.push_back(e);
	} else {
		adj.push_back(e);
	}
	int id = e->getID();
	e->_sgraph(graph);
	graph->gv->addEdge(id, e->getSource()->getID(), e->getDest()->getID(), EdgeType::DIRECTED);
	// * Set Edge Label
	if (graph->show.edgeLabels)
		graph->gv->setEdgeLabel(id, to_string(id));
	// * Set Edge Color
	if (e->isAccidented())
		graph->gv->setEdgeColor(id, ACCIDENTED_COLOR);
	// No graph->update()
	return true;
}

/*
 * @brief Returns a pointer to an edge with given id
 * @return The edge or nullptr if not found
 */
Edge* Vertex::findEdge(int eid) const {
	return getEdge(eid);
}

/*
 * @brief Returns a pointer to an edge with given id
 * @return The edge or nullptr if not found
 */
Edge* Vertex::getEdge(int eid) const {
	const auto getEid = [&eid](Edge *e) -> bool { return e->getID() == eid; };
	auto it = find_if(adj.cbegin(), adj.cend(), getEid);
	if (it != adj.cend()) return *it;

	it = find_if(accidentedAdj.cbegin(), accidentedAdj.cend(), getEid);
	if (it != accidentedAdj.cend()) return *it;

	return nullptr;
}

/*
 * @brief Returns a pointer to an edge with
 * destinationv vdest
 * @return The edge or nullptr if not found
 * @throws invalid_argument if Vertex is nullptr
 */
Edge* Vertex::findEdge(Vertex *vdest) const {
	return getEdge(vdest);
}

/*
 * @brief Returns a pointer to an edge with
 * destinationv vdest
 * @return The edge or nullptr if not found
 * @throws invalid_argument if Vertex is nullptr
 */
Edge* Vertex::getEdge(Vertex* vdest) const {
	if (vdest == nullptr) {
		throw std::invalid_argument("Vertex not found");
	}
	const auto getDest = [&vdest](Edge *e) -> bool { return e->getDest() == vdest; };
	auto it = find_if(adj.cbegin(), adj.cend(), getDest);
	if (it != adj.cend()) return *it;

	it = find_if(accidentedAdj.cbegin(), accidentedAdj.cend(), getDest);
	if (it != accidentedAdj.cend()) return *it;

	return nullptr;
}

/*
 * @brief Fixes this vertex from a previous accident
 * @return True if the vertex was previously accidented
 */
bool Vertex::fix() {
	if (accidented) {
		accidented = false;
		// * Set Vertex Color
		graph->setVertexColor(this, VERTEX_CLEAR_COLOR);
		// * Fix edges ?
		for (Edge *e : accidentedAdj) {
			e->fix();
		}
		// Move back to vertexSet
		graph->moveToVertexSet(this);
		return true;
	} else {
		return false;
	}
}

/*
 * @brief Causes an accident on this vertex
 * @return True if the vertex was previously clear
 */
bool Vertex::accident() {
	if (!accidented) {
		accidented = true;
		// * Set Vertex Color
		graph->setVertexColor(this, ACCIDENTED_COLOR);
		// * Accident edges ?
		for (Edge *e : adj) {
			e->accident();
		}
		// Move to accidentedVertexSet
		graph->moveToAccidentedVertexSet(this);
		return true;
	} else {
		return false;
	}
}

/*
 * @brief Fixes an edge pointing to vertex vdest
 * @return True if the edge previously accidented
 *         False otherwise
 * @throws invalid_argument if Vertex is nullptr
 */
bool Vertex::fixEdge(Vertex *vdest) {
	if (vdest == nullptr) {
		throw std::invalid_argument("Vertex not found");
	}
	Edge* e = findEdge(vdest);
	if (e == nullptr) return false;
	return e->fix();
}

/*
 * @brief Fixes an edge pointing to vertex vdest
 * @return True if the edge previously clear
 *         False otherwise
 * @throws invalid_argument if Vertex is nullptr
 */
bool Vertex::accidentEdge(Vertex *vdest) {
	if (vdest == nullptr) {
		throw std::invalid_argument("Vertex not found");
	}
	Edge* e = findEdge(vdest);
	if (e == nullptr) return false;
	return e->accident();
}

/*
 * @brief Removes an edge with given id
 * @throws invalid_argument if Edge not found
 * @throws invalid_argument if Edge is not from this vertex
 */
void Vertex::removeEdge(int eid) {
	return removeEdge(findEdge(eid));
}

/*
 * @brief Removes an edge with destination dest
 * @throws invalid_argument if Vertex is nullptr
 * @throws invalid_argument if Edge not found
 * @throws invalid_argument if Edge is not from this vertex
 */
void Vertex::removeEdge(Vertex* dest) {
	return removeEdge(findEdge(dest));
}

/*
 * @brief Removes an edge
 * @throws invalid_argument if Edge not found
 * @throws invalid_argument if Edge is not from this vertex
 */
void Vertex::removeEdge(Edge* edge) {
	if (edge == nullptr) {
		throw std::invalid_argument("Edge not found");
	}
	if (!findEdge(edge->getID())) {
		throw std::invalid_argument("Edge not from this vertex");
	}
	if (edge->isAccidented()) {
		auto it = find(accidentedAdj.begin(),
				accidentedAdj.end(), edge);
		delete edge;
		accidentedAdj.erase(it);
	} else {
		int id = edge->getID();
		auto it = find(adj.begin(), adj.end(), edge);
		delete edge;
		adj.erase(it);
		graph->gv->removeEdge(id);
		// No graph->update()
	}
}

/*
 * @brief Return true if "vertex" have greater distance
 */
bool Vertex::operator<(Vertex vertex) const {
	return this->priority < vertex.priority;
}









void Edge::_sgraph(Graph* graph) {
	if (this->graph != nullptr) {
		throw std::logic_error("Graph already set");
	}
	this->graph = graph;
}

/*
 * @brief Constructs an edge with given id
 * starting at vertex vsource with destination
 * vdest, with a given weight and an initial
 * conditional accidented, defaulting to false
 */
Edge::Edge(int id, Vertex* vsource, Vertex* vdest,
		Subroad* subroad, bool accidented):
	id(id), source(vsource), dest(vdest),
	accidented(accidented), subroad(subroad) {}

/*
 * @brief Return's the edge's source vertex
 */
Vertex* Edge::getSource() const {
	return source;
}

/*
 * @brief Return's the edge's destination vertex
 */
Vertex* Edge::getDest() const {
	return dest;
}

/*
 * @brief Return's the edge's id
 */
int Edge::getID() const {
	return id;
}

/*
 * @brief Return's the edge's weight
 * The edge's weight is the time it takes to travel
 * along the edge given the amount of cars in it.
 */
double Edge::getWeight() const {
	return (subroad->getDistance()/(double)1000)/subroad->calculateAverageSpeed();
}

/*
 * @brief Return's the edge's Road (not Subroad)
 */
Road* Edge::getRoad() const {
	return subroad->getRoad();
}

/*
 * @brief Check whether the edge is accidented
 */
bool Edge::isAccidented() const {
	return accidented;
}

/*
 * @brief Return the underlying subroad's total length
 */
double Edge::getDistance() const {
	return subroad->getDistance();
}

/*
 * @brief Return the edge's actual Car capacity
 */
int Edge::getActualCapacity() const {
	return subroad->getActualCapacity();
}

/*
 * @brief Return the edge's maximum Car capacity
 */
int Edge::getMaxCapacity() const {
	return subroad->getMaxCapacity();
}

/*
 * @brief Compute the edge's expected average speed
 * (based on actual capacity and max capacity)
 */
int Edge::calculateAverageSpeed() const {
	return subroad->calculateAverageSpeed();
}

/*
 * @brief Fixes this edge from a previous accident
 * @return True if the edge was previously accidented
 *         False otherwise
 */
bool Edge::fix() {
	if (accidented) {
		accidented = false;
		// * Set Edge Color
		graph->setEdgeColor(this, EDGE_CLEAR_COLOR);
		// Move back to adj
		source->moveToAdj(this);
		return true;
	} else {
		return false;
	}
}

/*
 * @brief Causes an accident on this vertex
 * @return True if the vertex was previously clear
 *         False otherwise
 */
bool Edge::accident() {
	if (!accidented) {
		accidented = true;
		// * Set Edge Color
		graph->setEdgeColor(this, ACCIDENTED_COLOR);
		// Move to accidentedAdj
		source->moveToAccidentedAdj(this);
		return true;
	} else {
		return false;
	}
}

/*
 * @brief Sets the edge's actual Car capacity
 * @return True if within the bounds set by max capacity
 *         False otherwise
 */
bool Edge::setActualCapacity(int capacity) {
	return subroad->setActualCapacity(capacity);
}






void Graph::generateGraphNewStatus(vector<Vertex*> path) {
	//For every edge generate new capacity
	vector<Vertex*> everyVextex = getAllVertexSet();

	for (auto vertex : everyVextex) {
		if ((vertex != path[0]) || (vertex != path[1]) || (vertex != path[path.size()-1])){
			int newStatus = rand() % 501;

			if(newStatus > 498){
				if(vertex->isAccidented())
					vertex->fix();
				else
					vertex->accident();
			}
		}

		for (auto edge : vertex->adj) {
			if (edge != nullptr) {
				int newActualCapacity = (rand() % edge->getMaxCapacity());
				edge->setActualCapacity(newActualCapacity);
			}
		}
	}

}


void Graph::bfs(Vertex *origin) {
	clear();

	deque<Vertex*> q;
	q.push_back(origin);
	while (!q.empty()) {
		auto v = q.front();
		q.pop_front();
		for (auto e : v->adj) { // Non-accidented only
			auto vertex = e->dest;
			// If vertex is accidented, skip
			if (vertex->isAccidented()) continue;
			if (!vertex->path) {
				vertex->path = v;
				vertex->priority = v->priority + 1;
				q.push_back(vertex);
			}
		}
	}
}



/**
 * Performs Greedy Best-First Search in the graph,
 * with given source and destination vertices.
 * The resulting path obtainable by getPath(vsource, vdest)
 * is not necessarily the best (shortest) path.
 */
void Graph::gbfsDist(Vertex *vsource, Vertex *vdest, microtime *time) {
	clear();

	auto start = chrono::high_resolution_clock::now();

	MutablePriorityQueue<Vertex> q;
	q.insert(vsource);
	while (!q.empty()) {
		auto current = q.extractMin();
		if (current == vdest) break;

		for (auto e : current->adj) { // Non-accidented only
			auto next = e->dest;
			if (next->path != nullptr) continue; // If visited, skip
			if (next->isAccidented()) continue; // If accidented, skip

			next->priority = distance(next, vdest); // <- Greedy Best-First

			next->path = current;
			q.insert(next);
		}
	}

	auto end = chrono::high_resolution_clock::now();
	if (time) *time = chrono::duration_cast<chrono::microseconds>(end - start).count();
}



/**
 * Performs Dijkstra in the graph, with a given
 * source vertex but no predetermined destination vertex,
 * so it finds the best path for all reachable vertices.
 */
void Graph::dijkstraDist(Vertex *vsource, microtime *time) {
	clear();

	auto start = chrono::high_resolution_clock::now();

	MutablePriorityQueue<Vertex> q;
	q.insert(vsource);
	while (!q.empty()) {
		auto current = q.extractMin();
		for (auto e : current->adj) { // Non-accidented only
			auto next = e->dest;
			if (next->isAccidented()) continue; // If accidented, skip

			long double newcost = current->cost + distance(current, next); // <- Dijkstra

			if (next->path == nullptr) {
				next->cost = newcost;
				next->priority = newcost;
				next->path = current;
				q.insert(next);
			}
			else if (newcost < next->cost) {
				next->cost = newcost;
				next->priority = newcost;
				next->path = current;
				q.decreaseKey(next);
			}
		}
	}

	auto end = chrono::high_resolution_clock::now();
	if (time) *time = chrono::duration_cast<chrono::microseconds>(end - start).count();
}



/**
 * Performs Dijkstra in the graph, with given
 * source and destination vertices, stopping once the best
 * path from vsource to vdest is found.
 */
void Graph::dijkstraDist(Vertex *vsource, Vertex *vdest, microtime *time) {
	clear();

	auto start = chrono::high_resolution_clock::now();

	MutablePriorityQueue<Vertex> q;
	q.insert(vsource);
	while (!q.empty()) {
		auto current = q.extractMin();
		if (current == vdest) break;
		for (auto e : current->adj) { // Non-accidented only
			auto next = e->dest;
			if (next->isAccidented()) continue; // If accidented, skip

			long double newcost = current->cost + distance(current, next); // <- Dijkstra

			if (next->path == nullptr) {
				next->cost = newcost;
				next->priority = newcost;
				next->path = current;
				q.insert(next);
			}
			else if (newcost < next->cost) {
				next->cost = newcost;
				next->priority = newcost;
				next->path = current;
				q.decreaseKey(next);
			}
		}
	}

	auto end = chrono::high_resolution_clock::now();
	if (time) *time = chrono::duration_cast<chrono::microseconds>(end - start).count();
}



/**
 * Performs A* in the graph, given origin and destination vertices,
 * stopping once the best path from vsource to vdest is found.
 */
void Graph::AstarDist(Vertex *vsource, Vertex *vdest, microtime *time) {
	clear();

	auto start = chrono::high_resolution_clock::now();

	MutablePriorityQueue<Vertex> q;
	q.insert(vsource);
	while (!q.empty()) {
		auto current = q.extractMin();
		if (current == vdest) break;
		for (auto e : current->adj) { // Non-accidented only
			auto next = e->dest;
			if (next->isAccidented()) continue; // If accidented, skip

			long double newcost = current->cost + distance(current, next);

			if (next->path == nullptr) {
				next->cost = newcost;
				next->priority = newcost + distance(next, vdest); // <- A*
				next->path = current;
				q.insert(next);
			}
			else if (newcost < next->cost) {
				next->cost = newcost;
				next->priority = newcost + distance(next, vdest); // <- A*
				next->path = current;
				q.decreaseKey(next);
			}
		}
	}

	auto end = chrono::high_resolution_clock::now();
	if (time) *time = chrono::duration_cast<chrono::microseconds>(end - start).count();
}



/**
 * Performs Dijkstra in the graph, computing the fastest path, in terms
 * of travel time, given source and destination vertices.
 */
void Graph::dijkstraSimulation(Vertex *vsource, Vertex *vdest, microtime *time) {
	clear();

	auto start = chrono::high_resolution_clock::now();

	MutablePriorityQueue<Vertex> q;
	q.insert(vsource);
	while (!q.empty()) {
		auto current = q.extractMin();
		if (current == vdest) break;
		for (auto e : current->adj) { // Non-accidented only
			auto next = e->dest;
			if (next->isAccidented()) continue; // If accidented, skip

			long double newcost = current->cost + e->getWeight(); // <- Dijkstra

			if (next->path == nullptr) {
				next->cost = newcost;
				next->priority = newcost;
				next->path = current;
				q.insert(next);
			}
			else if (newcost < next->cost) {
				next->cost = newcost;
				next->priority = newcost;
				next->path = current;
				q.decreaseKey(next);
			}
		}
	}

	auto end = chrono::high_resolution_clock::now();
	if (time) *time = chrono::duration_cast<chrono::microseconds>(end - start).count();
}




