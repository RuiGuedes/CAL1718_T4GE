#include <iostream>
#include <cstdio>
#include "graphviewer.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "Graph.h"

int main() {

	Graph<int> myGraph;

	GraphViewer *gv = new GraphViewer(600, 600, true);
	gv->createWindow(600, 600);
	gv->defineVertexColor("blue");
	gv->defineEdgeColor("black");

	for(int i = 1; i <= 7; i++) {
		myGraph.addVertex(i);
		gv->addNode(i);
	}

	myGraph.addEdge(1, 2, 2);	gv->addEdge(1,1,2,EdgeType::UNDIRECTED);	gv->setEdgeLabel(1, "2");
	myGraph.addEdge(1, 4, 7);	gv->addEdge(2,1,4,EdgeType::UNDIRECTED);	gv->setEdgeLabel(2, "7");
	myGraph.addEdge(2, 4, 3);	gv->addEdge(3,2,4,EdgeType::UNDIRECTED);	gv->setEdgeLabel(3, "3");
	myGraph.addEdge(2, 5, 5);	gv->addEdge(4,2,5,EdgeType::UNDIRECTED);	gv->setEdgeLabel(4, "5");
	myGraph.addEdge(3, 1, 2);	gv->addEdge(5,3,1,EdgeType::UNDIRECTED);	gv->setEdgeLabel(5, "2");
	myGraph.addEdge(3, 6, 5);	gv->addEdge(6,3,6,EdgeType::UNDIRECTED);	gv->setEdgeLabel(6, "5");
	myGraph.addEdge(4, 3, 1);	gv->addEdge(7,4,3,EdgeType::UNDIRECTED);	gv->setEdgeLabel(7, "1");
	myGraph.addEdge(4, 5, 1);	gv->addEdge(8,4,5,EdgeType::UNDIRECTED);	gv->setEdgeLabel(8, "1");
	myGraph.addEdge(4, 6, 6);	gv->addEdge(9,4,6,EdgeType::UNDIRECTED);	gv->setEdgeLabel(9, "6");
	myGraph.addEdge(4, 7, 4);	gv->addEdge(10,4,7,EdgeType::UNDIRECTED);	gv->setEdgeLabel(10, "4");
	myGraph.addEdge(5, 7, 2);	gv->addEdge(11,5,7,EdgeType::UNDIRECTED);	gv->setEdgeLabel(11, "2");
	myGraph.addEdge(6, 4, 3);	gv->addEdge(12,6,4,EdgeType::UNDIRECTED);	gv->setEdgeLabel(12, "3");
	myGraph.addEdge(7, 6, 4);	gv->addEdge(13,7,6,EdgeType::UNDIRECTED);	gv->setEdgeLabel(13, "4");

	gv->setVertexColor(1,"green");
	myGraph.dijkstraShortestPath(1);
	vector<int> nos = myGraph.getPath(1, 7);
	int i = nos.size() - 1;

	while(i >= 0) {
		gv->setVertexColor(nos.at(i),"green");
		gv->rearrange();
		Sleep(2000);
		i--;
	}

	for(int i = nos.size() - 1; i >= 0; i--) {
		cout << nos.at(i) << " ";
	}
	cout << endl;
	getchar();
	return 0;

}
