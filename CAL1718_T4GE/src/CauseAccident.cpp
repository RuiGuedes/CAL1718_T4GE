#include "Interface.h"

//////////////////////////
// Functions Prototypes //
//////////////////////////

void causeIntersectionAccident();
void causeRoadAccident();

/*
 * Cause Accident UI Menu
 */
void causeAccident() {
	int option;

	cout << "Cause accident" << endl << endl;

	cout << "1 - Intersection accident" << endl;
	cout << "2 - Road accident" << endl;
	cout << "3 < return" << endl;

	option = selectOption(3);
	if (option == 3) return;

	switch (option)
	{
	case 1:
		causeIntersectionAccident();
		break;
	case 2:
		causeRoadAccident();
		break;
	}

	system("pause"); // dont like this
	system("cls");
	return causeAccident();
}

/*
 * Cause Node accident UI
 */
void causeIntersectionAccident() {
	Vertex *vertex = nullptr;

	cout << "Cause an accident" << endl << endl;

	vertex = selectVertex(true);
	if (vertex == nullptr) return;

	// Cause accident
	vertex->accident();
	graph->rearrange();
}

/*
 * Cause Edge accident UI
 */
void causeRoadAccident() {
	Edge *edge = nullptr;

	cout << "Cause an accident" << endl << endl;

	edge = selectEdge(true);
	if (edge == nullptr) return;

	// Cause accident
	edge->accident();
	graph->rearrange();
}
