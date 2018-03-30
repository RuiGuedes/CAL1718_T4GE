/*
 * Graph.h
 */
#pragma once

#include <vector>
#include <queue>
#include <list>
#include <limits>
#include <cmath>
#include <algorithm>
#include "MutablePriorityQueue.h"

using namespace std;

template <class T> class Edge;
template <class T> class Graph;
template <class T> class Vertex;

#define INF std::numeric_limits<double>::max()

/*************************** Graph  **************************/

template <class T>
class Graph {
	vector<Vertex<T> *> vertexSet;    			// vertex set
	vector<Vertex<T> *> accidentedVertexSet;	// accidented vertex set
public:
	Vertex<T> *findVertex(const T &in) const;
	bool addVertex(const T &in);
	void removeVertex(T info);
	void removeAccidentedVertex(T info);
	bool addEdge(const T &sourc, const T &dest, double w);
	int getNumVertex() const;
	vector<Vertex<T> *> getVertexSet() const;

	void dijkstraShortestPath(const T &s);
	void dijkstraShortestPathOld(const T &s);
	vector<T> getPath(const T &origin, const T &dest) const;

};

template <class T>
int Graph<T>::getNumVertex() const {
	return vertexSet.size() + accidentedVertexSet.size();
}

template <class T>
vector<Vertex<T> *> Graph<T>::getVertexSet() const {
	return vertexSet;
}

template <class T>
void Graph<T>::removeVertex(T info) {
	typename vector<Vertex<T> *>::iterator vertexIterator = this->vertexSet.begin();

	while(vertexIterator != this->vertexSet.end()) {
		if((*vertexIterator)->getInfo() == info) {
			this->accidentedVertexSet.push_back(*vertexIterator);
			this->vertexSet.erase(vertexIterator);
			return;
		}
		vertexIterator++;
	}
}

template <class T>
void Graph<T>::removeAccidentedVertex(T info) {
	typename vector<Vertex<T> *>::iterator vertexIterator = this->accidentedVertexSet.begin();

	while(vertexIterator != this->accidentedVertexSet.end()) {
		if((*vertexIterator)->getInfo() == info) {
			this->vertexSet.push_back(*vertexIterator);
			this->accidentedVertexSet.erase(vertexIterator);
			return;
		}
		vertexIterator++;
	}
}

/*
 * Auxiliary function to find a vertex with a given content.
 */
template <class T>
Vertex<T> * Graph<T>::findVertex(const T &in) const {
	for (auto v : vertexSet)
		if (v->info == in)
			return v;
	return NULL;
}

/*
 *  Adds a vertex with a given content or info (in) to a graph (this).
 *  Returns true if successful, and false if a vertex with that content already exists.
 */
template <class T>
bool Graph<T>::addVertex(const T &in) {
	if ( findVertex(in) != NULL)
		return false;
	vertexSet.push_back(new Vertex<T>(in));
	return true;
}

/*
 * Adds an edge to a graph (this), given the contents of the source and
 * destination vertices and the edge weight (w).
 * Returns true if successful, and false if the source or destination vertex does not exist.
 */
template <class T>
bool Graph<T>::addEdge(const T &sourc, const T &dest, double w) {
	auto v1 = findVertex(sourc);
	auto v2 = findVertex(dest);
	if (v1 == NULL || v2 == NULL)
		return false;
	v1->addEdge(v2,w);
	return true;
}


/**************** Single Source Shortest Path algorithms ************/

template<class T>
void Graph<T>::dijkstraShortestPath(const T &origin) {
	MutablePriorityQueue<Vertex<T> > q;
	for(auto v : vertexSet) {
		if(v->info == origin) {
			v->dist = 0;
			v->path = NULL;
			q.insert(v);
		}
		else {
			v->dist = INF;
			v->path = NULL;
		}
	}

	while(!q.empty()) {
		Vertex<T> *v = q.extractMin();

		for(auto w : v->adj) {
			if(w.dest->dist > v->dist + w.weight) {
				double oldDist = w.dest->dist;

				w.dest->dist = v->dist + w.weight;
				w.dest->path = v;

				if(oldDist == INF)
					q.insert(w.dest);
				else
					q.decreaseKey(w.dest);
			}

		}
	}

}

template<class T>
vector<T> Graph<T>::getPath(const T &origin, const T &dest) const{
	vector<T> res;
	auto v = findVertex(dest);
	if(v == nullptr || v->dist == INF) // missing or disconnected
		return res;
	for ( ; v != nullptr; v = v->path)
		res.push_back(v->info);
	reverse(res.begin(), res.end());

	return res;
}

/************************* Vertex  **************************/

template <class T>
class Vertex {
	T info;                // contents
	vector<Edge<T> > adj;  // outgoing edges
	bool visited;          // auxiliary field
	double dist = 0;
	Vertex<T> *path = NULL;
	int queueIndex = 0; 		// required by MutablePriorityQueue

	bool processing = false;
	void addEdge(Vertex<T> *dest, double w);

public:
	Vertex(T in);
	bool operator<(Vertex<T> & vertex) const; // // required by MutablePriorityQueue
	T getInfo() const;
	double getDist() const;
	Vertex *getPath() const;
	friend class Graph<T>;
	friend class MutablePriorityQueue<Vertex<T>>;
};

template <class T>
Vertex<T>::Vertex(T in): info(in) {}

/*
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 */
template <class T>
void Vertex<T>::addEdge(Vertex<T> *d, double w) {
	adj.push_back(Edge<T>(d, w));
}

template <class T>
bool Vertex<T>::operator<(Vertex<T> & vertex) const {
	return this->dist < vertex.dist;
}

template <class T>
T Vertex<T>::getInfo() const {
	return this->info;
}

template <class T>
double Vertex<T>::getDist() const {
	return this->dist;
}

template <class T>
Vertex<T> *Vertex<T>::getPath() const {
	return this->path;
}


/********************** Edge  ****************************/

template <class T>
class Edge {
	Vertex<T> * dest;      // destination vertex
	double weight;         // edge weight
public:
	Edge(Vertex<T> *d, double w);
	friend class Graph<T>;
	friend class Vertex<T>;
};

template <class T>
Edge<T>::Edge(Vertex<T> *d, double w): dest(d), weight(w) {}
