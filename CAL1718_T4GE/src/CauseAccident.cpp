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
	cout << "2 - Road accident" << endl << endl;

	option = selectOption(2);
	if (option == 3)
		return;

	switch (option)
	{
	case 1:
		causeIntersectionAccident();
		break;
	case 2:
		causeRoadAccident();
		break;
	}

	system("cls");
	return causeAccident();
}

/*
 * Cause Node accident UI
 */
void causeIntersectionAccident() {
	system("cls");

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
	system("cls");

	Edge *edge = nullptr;

	cout << "Cause an accident" << endl << endl;

	edge = selectEdge(true);
	if (edge == nullptr) return;

	// Cause accident
	edge->accident();
	graph->rearrange();
}
