#include "Graph.h"

#include <limits>
#include <cmath>
#include <algorithm>
#include <cerrno>
#include <assert.h>

#define INF numeric_limits<double>::max()

/*
 * @brief Checks if a pair of integers is
 * within the bounds of the graph area
 * @nothrow
 */
bool Graph::withinBounds(int x, int y) const {
	return x >= 0 && y >= 0 && x <= width && y <= height;
}

/*
 * @brief Graph constructor, taking
 * display width and display height in grid entries
 */
Graph::Graph(int width, int height) {
	this->width = width; this->height = height;
	gv = new GraphViewer(width, height, false);
	gv->createWindow(GRAPH_VIEWER_WIDTH, GRAPH_VIEWER_HEIGHT);
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
 * @throws logic_error if Repeated vertex id
 * @throws out_of_range if Vertex out of graph bounds
 */
bool Graph::addVertex(int id, int x, int y, bool accidented) {
	if (findVertex(id)) {
		throw std::logic_error("Repeated vertex id");
	} else if (!withinBounds(x, y)) {
		throw std::out_of_range("Vertex out of graph bounds");
	} else {
		Vertex *v = new Vertex(id, x, y, accidented);
		v->graph = this;
		if (accidented) {
			accidentedVertexSet.push_back(v);
		} else {
			vertexSet.push_back(v);
		}
		return true;
	}
}

/*
 * @brief Adds an already created vertex to the
 * appropriate vertex set of the graph
 * @throws logic_error if Repeated vertex id
 * @throws out_of_range if Vertex out of graph bounds
 */
bool Graph::addVertex(Vertex* v) {
	if (findVertex(v->getId())) {
		throw std::logic_error("Repeated vertex id");
	} else if (!withinBounds(v->getX(), v->getY())) {
		throw std::out_of_range("Vertex out of graph bounds");
	} else {
		v->graph = this;
		if (v->isAccidented()) {
			accidentedVertexSet.push_back(v);
		} else {
			vertexSet.push_back(v);
		}
		return true;
	}
}

/*
 * @brief Returns a vertex with a given id
 * @return The vertex if found, nullptr otherwise
 * @nothrow
 */
Vertex* Graph::findVertex(int id) const {
	return getVertex(id);
}

/*
 * @brief Returns a vertex with a given id
 * @return The vertex if found, nullptr otherwise
 * @nothrow
 */
Vertex* Graph::getVertex(int id) const {
	static const auto getId = [](Vertex *v) -> int { return v->id; };

	// Look for vertex in vertexSet
	auto it = find(vertexSet.cbegin(), vertexSet.cend(), getId);
	if (it != vertexSet.cend()) return *it;

	// Look for vertex in accidentedVertexSet
	it = find(accidentedVertexSet.cbegin(), accidentedVertexSet.cend(), getId);
	if (it != accidentedVertexSet.cend()) return *it;

	// Vertex not found
	return nullptr;
}

/*
 * @brief Returns the number of clear vertices
 * @nothrow
 */
int Graph::getNumVertices() const {
	return vertexSet.size();
}

/*
 * @brief Returns the number of accidented vertices
 * @nothrow
 */
int Graph::getNumAccidentedVertices() const {
	return accidentedVertexSet.size();
}

/*
 * @brief Returns the total number of vertices,
 * both clear and accidented
 * @nothrow
 */
int Graph::getTotalVertices() const {
	return vertexSet.size() + accidentedVertexSet.size();
}

/*
 * @brief Returns a vector copy of the graph's clear
 * vertex set
 * @nothrow
 */
vector<Vertex*> Graph::getVertexSet() const {
	return vertexSet;
}

/*
 * @brief Returns a vector copy of the graph's accidented
 * vertex set
 * @nothrow
 */
vector<Vertex*> Graph::getAccidentedVertexSet() const {
	return accidentedVertexSet;
}

/*
 * @brief Returns a vector with all of the graph's vertices
 * @nothrow
 */
vector<Vertex*> Graph::getAllVertexSet() const {
	vector<Vertex*> set = vertexSet;
	copy(accidentedVertexSet.cbegin(), accidentedVertexSet.cend(), back_inserter(set));
	return set;
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
	long long x1 = v1->getX(), y1 = v1->getY();
	long long x2 = v2->getX(), y2 = v2->getY();
	return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

/*
 * @brief Computes the length of an edge e,
 * according to the distance between both endpoints
 * @throws invalid_argument if Edge not found
 */
double Graph::length(Edge* e) const {
	if (e == nullptr) {
		return std::invalid_argument("Edge not found");
	}
	return distance(e->getSource(), e->getDest());
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
 * @brief Checks if an edge e is currently accidented
 * @throws invalid_argument if Edge is nullptr
 */
bool Graph::edgeIsAccidented(Edge *e) const {
	if (e == nullptr) {
		throw std::invalid_argument("Edge not found");
	}
	return e->isAccidented();
}

/*
 * @brief Checks if vertex v is accidented
 * @throws invalid_argument if Vertex is nullptr
 */
bool Graph::vertexIsAccidented(Vertex* v) const {
	if (v == nullptr) {
		throw std::invalid_argument("Vertex not found");
	}
	return v->isAccidented();
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
		auto it = find(accidentedVertexSet.begin(),
				accidentedVertexSet.end(), v);
		delete v;
		accidentedVertexSet.erase(it);
	} else {
		auto it = find(vertexSet.begin(),
				vertexSet.end(), v);
		delete v;
		vertexSet.erase(it);
	}
}

/*
 * @brief Adds an edge with given id starting at vertex
 * with id sourceId and ending in vertex with id destId
 * @return True if edge was successfully added
 *         False if a similar edge already existed
 * @throws invalid_argument if Vertex not found
 * @throws logic_error if Repeated edge id
 */
bool Graph::addEdge(int eid, int sourceId, int destId,
		double weight/*, Road *w*/, bool accidented) {
	return addEdge(eid, findVertex(sourceId),
			findVertex(destId),weight/*, w*/);
}

/*
 * @brief Adds and edge with id eid starting at vertex
 * vsource and ending in vertex vdest
 * @return True if edge was successfully added
 *         False if a similar edge already existed
 * @throws invalid_argument if Vertex not found
 * @throws logic_error if Repeated edge id
 */
bool Graph::addEdge(int eid, Vertex* vsource, Vertex* vdest,
		double weight/*, Road *w*/, bool accidented) {
	if (vsource == nullptr || vdest == nullptr) {
		throw std::invalid_argument("Vertex not found");
	}
	if (findEdge(eid) != nullptr) {
		throw std::logic_error("Repeated edge id");
	}
	Edge* e = new Edge(eid, vsource, vdest, weight, accidented);
	e->graph = this;
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
	if (findEdge(e->getId()) != nullptr) {
		throw std::logic_error("Repeated edge id");
	}
	e->graph = this;
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
	for (auto vertex : vertexSet) {
		Edge* e = vertex->getEdge(eid);
		if (e != nullptr) return e;
	}
	for (auto vertex : accidentedVertexSet) {
		Edge* e = vertex->getEdge(eid);
		if (e != nullptr) return e;
	}
	return nullptr;
}

/*
 * @brief Returns a pointer to an edge starting at a
 * vertex with id sourceId and ending at vertex destId
 * @return The edge pointer, or nullptr if not found
 * @throws invalid_argument if either Vertex not found
 */
Edge* Graph::findEdge(int sourceId, int destId) const {
	return getEdge(sourceId, destId);
}

/*
 * @brief Returns a pointer to an edge starting at a
 * vertex with id sourceId and ending at vertex destId
 * @return The edge pointer, or nullptr if not found
 * @throws invalid_argument if either Vertex not found
 */
Edge* Graph::getEdge(int sourceId, int destId) const {
	Vertex* vsource = findVertex(sourceId);
	Vertex* vdest = findVertex(destId);
	if (vsource == nullptr || vdest == nullptr) {
		throw std::invalid_argument("Vertex not found");
	}
	return vsource->getEdge(vdest);
}

/*
 * @brief Fixes a previously accidented edge e
 * @return True if edge was previously accidented
 *         False otherwise
 * @throws invalid_argument if Edge not found
 */
bool Graph::fixEdge(Edge* e) {
	if (e == nullptr) {
		throw std::invalid_argument("Edge not found");
	}
	return e->fix();
}

/*
 * @brief Causes an accident on an edge e
 * @return True if edge was previously clear
 *         False otherwise
 * @throws invalid_argument if Edge not found
 */
bool Graph::accidentEdge(Edge* e) {
	if (e == nullptr) {
		throw std::invalid_argument("Edge not found");
	}
	return e->fix();
}

/*
 * @brief Removes an edge with id eid
 * @return True if edge was successfully removed
 * @throws invalid_argument if Edge not found
 */
bool Graph::removeEdge(int eid) {
	return removeEdge(findEdge(eid));
}

/*
 * @brief Removes an edge e
 * @return True if edge was successfully removed
 * @throws invalid_argument if Edge not found
 */
bool Graph::removeEdge(Edge* e) {
	if (e == nullptr) {
		throw std::invalid_argument("Edge not found");
	}
	return e->getSource()->removeEdge(e);
}



bool Graph::bfs(Vertex* vsource) {
	return true;
}

bool Graph::bfs(Vertex* vsource, Vertex* vdest) {
	return true;
}

bool Graph::dijkstraDist(Vertex* vsource) {
	return true;
}

bool Graph::dijkstraDist(Vertex* vsource, Vertex* vdest) {
	return true;
}

bool Graph::AstarDist(Vertex* vsource, Vertex* vdest) {
	return true;
}

bool Graph::floydWarshallDist() {
	return true;
}

bool Graph::johnsonDist() {
	return true;
}



vector<Vertex*> Graph::getPath(int sourceId, int destId) const {
}

vector<Vertex*> Graph::getPath(Vertex* source, Vertex* dest) const {
}



ostream& Graph::operator<<(ostream& out) const {
	for (auto vertex : vertexSet)
		out << vertex;
	for (auto vertex : accidentedVertexSet)
		out << vertex;
	return out;
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
	for (auto edge : adj)
		delete edge;
	for (auto edge : accidentedAdj)
		delete edge;
}

/*
 * @brief Return the vertex id
 */
int Vertex::getId() const {
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
 * @brief Return the vertex's cartesian distance
 * to vertex dest
 * @throws invalid_argument if Vertex is nullptr
 */
double Vertex::distanceTo(Vertex* dest) const {
	if (dest == nullptr) {
		throw std::invalid_argument("Vertex not found");
	}
	long long x1 = x, y1 = y;
	long long x2 = dest->getX(), y2 = dest->getY();
	return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
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
	const auto getDest = [&dest](Edge *e) -> bool { return e->dest == dest; };
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
	const auto getDest = [&dest](Edge *e) -> bool { return e->dest == dest; };
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
 * @brief Return the vertex predecessor
 * (used after application of a pathing algorithm)
 * @return The found path's predecessor vertex, or nullptr
 */
Vertex* Vertex::getPredecessor() const {
	return nullptr; // TODO
}

/*
 * @brief Return the entire path of vertices leading to
 * this vertex
 * @return The entire vector path, possibly empty
 */
vector<Vertex*> Vertex::getPath() const {
	return vector<Vertex*>(); // TODO
}

/*
 * @brief Adds a previously created edge e to the
 * vertex
 * @return True if edge was successfully added
 *         False if there was already an edge in the same
 *         spot (pointing to the same vertex)
 * @throws invalid_argument if Edge is nullptr
 */
bool Vertex::addEdge(Edge* e) {
	if (e == nullptr) {
		throw std::invalid_argument("Edge not found");
	}
	if (e->isAccidented()) {
		accidentedAdj.push_back(e);
	} else {
		adj.push_back(e);
	}
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
	const auto getEid = [&eid](Edge *e) -> bool { return e->id == eid; };
	auto it = find(adj.cbegin(), adj.cend(), getEid);
	if (it != adj.cend()) return *it;

	it = find(accidentedAdj.cbegin(), accidentedAdj.cend(), getEid);
	if (it != adj.cend()) return *it;

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
	const auto getDest = [&vdest](Edge *e) -> bool { return e->dest == vdest; };
	auto it = find(adj.cbegin(), adj.cend(), getDest);
	if (it != adj.cend()) return *it;

	it = find(accidentedAdj.cbegin(), accidentedAdj.cend(), getDest);
	if (it != adj.cend()) return *it;

	return nullptr;
}

/*
 * @brief Fixes this vertex from a previous accident
 * @return True if the vertex was previously accidented
 */
bool Vertex::fix() {
	if (accidented) {
		accidented = false;
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
 */
void Vertex::removeEdge(int eid) {
	return removeEdge(findEdge(eid));
}

/*
 * @brief Removes an edge with destination dest
 * @throws invalid_argument if Vertex is nullptr
 * @throws invalid_argument if Edge not found
 */
void Vertex::removeEdge(Vertex* dest) {
	return removeEdge(findEdge(dest));
}

/*
 * @brief Removes an edge
 * @throws invalid_argument if Edge not found
 */
void Vertex::removeEdge(Edge* edge) {
	if (edge == nullptr) {
		throw std::invalid_argument("Edge not found");
	}
	if (edge->isAccidented()) {
		auto it = find(accidentedAdj.begin(),
				accidentedAdj.end(), edge);
		delete edge;
		accidentedAdj.erase(it);
	} else {
		auto it = find(adj.begin(),
				adj.end(), edge);
		delete edge;
		adj.erase(it);
	}
}

bool Vertex::operator<(Vertex* v) const {
	return id < v->id;
}

bool Vertex::operator>(Vertex* v) const {
	return id > v->id;
}

bool Vertex::operator==(Vertex* v) const {
	return id == v->id;
}

bool Vertex::operator!=(Vertex* v) const {
	return id != v->id;
}

ostream& Vertex::operator<<(ostream& out) const {
	out << "ID = " << id << "; ";
	out << "(" << x << "," << y << "); ";
	out << "#Good = " << adj.size() << "; ";
	out << "#Bad = " << accidentedAdj.size();
	return out;
}










Edge::Edge(int id, Vertex* vsource, Vertex* vdest,
		double weight, bool accidented):
	id(id), source(vsource), dest(vdest),
	weight(weight), accidented(accidented),
	graph(nullptr) {}

Vertex* Edge::getSource() const {
	return source;
}

Vertex* Edge::getDest() const {
	return dest;
}

int Edge::getId() const {
	return id;
}

double Edge::getWeight() const {
	return weight;
}

bool Edge::isAccidented() const {
	return accidented;
}

bool Edge::fix() {
	if (accidented) {
		accidented = false;
		return true;
	} else {
		return false;
	}
}

bool Edge::accident() {
	if (!accidented) {
		accidented = true;
		return true;
	} else {
		return false;
	}
}

void Edge::setWeight(double weight) {
	this->weight = weight;
}

bool Edge::operator<(Edge* e) const {
	return id < e->id;
}

bool Edge::operator>(Edge* e) const {
	return id > e->id;
}

bool Edge::operator==(Edge* e) const {
	return id == e->id;
}

bool Edge::operator!=(Edge* e) const {
	return id != e->id;
}

ostream& Edge::operator<<(ostream& out) const {
	out << "EID = " << id << "; ";
	out << "Weight = " << weight << "; ";
	out << "(" << source->getId() << " -> " << dest->getId() << "); ";
	return out;
}




/*
void Graph::dijkstraDist(Vertex* vsource) {
	MutablePriorityQueue<Vertex> q;
	for (auto v : vertexSet) {
		if (v != vsource) {
			v->dist = 0;
			v->path = NULL;
			q.insert(v);
		}
		else {
			v->dist = INF;
			v->path = NULL;
		}
	}
	cout << "1\n\n";
	while (!q.empty()) {
		Vertex<T> *v = q.extractMin();

		for(auto w : v->adj) {
			cout << w.road->getWeight() << "\n\n";
			if(w.dest->dist > v->dist + w.road->getWeight()) {
				cout << w.road->getWeight() << "\n\n";
				double oldDist = w.dest->dist;

				w.dest->dist = v->dist + w.road->getWeight();
				w.dest->path = v;

				if(oldDist == INF)
					q.insert(w.dest);
				else
					q.decreaseKey(w.dest);
			}

		}
	}

}
*/









