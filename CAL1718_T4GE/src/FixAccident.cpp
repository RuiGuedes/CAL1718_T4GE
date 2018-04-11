#include "Interface.h"

//////////////////////////
// Functions Prototypes //
//////////////////////////

void fixIntersectionAccident();
void fixRoadAccident();

/*
 * Fix Accident Menu UI
 */
void fixAccident() {
	int option;

	cout << "Fix accident" << endl << endl;

	cout << "1 - Intersection accident" << endl;
	cout << "2 - Road accident" << endl;
	cout << "3 < return" << endl;

	option = selectOption(3);
	if (option == 3) return;

	system("cls");

	switch(option)
	{
	case 1:
		fixIntersectionAccident();
		break;
	case 2:
		fixRoadAccident();
		break;
	}

	system("pause"); // dont like this
	system("cls");
	return fixAccident();
}

/*
 * Fix Node accident UI
 */
void fixIntersectionAccident() {
	Vertex *vertex = nullptr;

	cout << "Fix an accident" << endl << endl;

	// Get node
	vertex = selectVertex(true);
	if (vertex == nullptr) return;

	// Fix node
	vertex->fix();
	graph->rearrange();
}

/*
 * Fix Edge accident UI
 */
void fixRoadAccident() {
	Edge *edge = nullptr;

	cout << "Fix an accident" << endl << endl;

	// Get edge
	edge = selectEdge(true);
	if (edge == nullptr) return;

	// Fix edge
	edge->fix();
	graph->rearrange();
}
