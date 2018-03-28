#include "Graph.h"

template <class T>
Edge<T>::Edge(Vertex<T> *d, double w): dest(d), weight(w) {}
