#include "Interface.h"


// 'esc' or 'q' or 'quit'
static const regex esc("^\\s*(?:esc|quit|q)[\\.,;]?\\s*$", regex::icase);

static string align(string str, size_t width) {
	if (str.size() >= width) {
		return str;
	} else {
		return str + string(width, ' ');
	}
}


/*
 * @brief Checks if input is a string containing
 * a valid Vertex, Edge, or Road id -- a positive integer
 * greater than 1 (and smaller than max if given)
 * > The "max" optional parameter is inclusive.
 * @param input The user's input sequence
 * @param max Verify that stoi(input) <= max
 * @return true if valid and stoi(input) does not throw, false otherwise
 */
bool validNumberInput(string input, int max) {
	static const regex reg_int("^\\s*(\\d+)[.,;]?\\s*$");
	smatch match;

	if (regex_match(input, match, reg_int)) {
		try {
			int value = stoi(match[1]);

			if (value < 1) // reserved ids
				return false;

			if (max > 0 && value > max)
				return false;

			return true;
		}
		catch (runtime_error &e) {
			return false;
		}
	} else {
		return false;
	}
}


// Choose an option from the range 1..MAX.
int getOption(int max) {
	while (1) {
		string input;
		cout << "Select option: ";
		cin >> input;

		if (validNumberInput(input, max)) {
			return stoi(input);
		}
		else {
			cout << "Unavailable option (" << input << "). Try again !" << endl << endl;
		}
	}
}


/**
 * @brief Choose any vertex in the Graph.
 * To allow the choice of the node, all node IDs
 * are temporarily displayed. Once finished the
 * node IDs are hidden once again.
 * @param maybeAccidented If the vertex may be accidented.
 */
Vertex* selectVertex(bool maybeAccidented) {
	graph->showAllVertexLabels();
	graph->update();
	Vertex* selected;

	while (1) {
		string input;
		cout << "Select node (write esc to cancel): ";
		cin >> input;

		if (validNumberInput(input)) {
			int id = stoi(input);
			Vertex* v = graph->findVertex(id);

			if (v != nullptr) { // Vertex found
				if (!maybeAccidented && v->isAccidented()) { // Illegal accidented vertex
					cout << "Accidented node (" << v->getID() << "). Try again !" << endl << endl;
					continue;
				}
				else { // All goochy
					selected = v;
					break;
				}
			}
			else { // Vertex not found
				cout << "Node not found (" << id << "). Try again !" << endl << endl;
				continue;
			}
		}
		else { // Non numeric input
			if (regex_match(input, esc)) { // Esc-type input, exit
				selected = nullptr;
				break;
			} else { // Anything else
				cout << "Invalid input (" << input << "). Try again !" << endl << endl;
			}
		}
	}

	graph->hideAllVertexLabels();
	graph->update();
	return selected;
}


/**
 * @brief Choose any origin vertex in the Graph.
 * To allow the choice of the node, all node IDs
 * are temporarily displayed. Once finished the
 * node IDs are hidden once again.
 * @param maybeAccidented If the vertex may be accidented.
 */
Vertex* selectOriginVertex(bool maybeAccidented) {
	graph->showAllVertexLabels();
	graph->update();
	Vertex* selected;

	while (1) {
		string input;
		cout << "Select origin node (write esc to cancel): ";
		cin >> input;

		if (validNumberInput(input)) {
			int id = stoi(input);
			Vertex* v = graph->findVertex(id);

			if (v != nullptr) { // Vertex found
				if (!maybeAccidented && v->isAccidented()) { // Illegal accidented vertex
					cout << "Accidented node (" << v->getID() << "). Try again !" << endl << endl;
					continue;
				}
				else if (v->getAdj().size() == 0) { // No reachable nodes
					cout << "There are no reachable nodes at the moment from " << v->getID() << "." << endl << endl;
					continue;
				}
				else { // All goochy
					selected = v;
					break;
				}
			}
			else { // Vertex not found
				cout << "Node not found (" << id << "). Try again !" << endl << endl;
				continue;
			}
		}
		else { // Non numeric input
			if (regex_match(input, esc)) { // Esc-type input, exit
				selected = nullptr;
				break;
			} else { // Anything else
				cout << "Invalid input (" << input << "). Try again !" << endl << endl;
			}
		}
	}

	graph->hideAllVertexLabels();
	graph->update();
	return selected;
}


/**
 * @brief Choose any destination vertex in the Graph.
 * To allow the choice of the node, all node IDs
 * are temporarily displayed. Once finished the
 * node IDs are hidden once again.
 * @param maybeAccidented If the vertex may be accidented
 * @param mustBeReachable If the vertex has to be reachable from origin
 * (1) maybeAccidented -> if the destination vertex can be accidented or not
 * (2) mustBeReachable -> if the destination vertex must be tagged as reachable (->getPath() not null)
 */
Vertex* selectDestinationVertex(Vertex* origin, bool maybeAccidented, bool mustBeReachable) {
	graph->showAllVertexLabels();
	graph->update();
	if (mustBeReachable) graph->bfs(origin);
	Vertex* selected;

	while (1) {
		string input;
		cout << "Select destination node (write esc to cancel): ";
		cin >> input;

		if (validNumberInput(input)) {
			int id = stoi(input);
			Vertex* v = graph->findVertex(id);

			if (v != nullptr) { // Vertex found
				if (!maybeAccidented && v->isAccidented()) { // Illegal accidented vertex
					cout << "Accidented node (" << v->getID() << "). Try again !" << endl << endl;
					continue;
				}
				else if (mustBeReachable && !v->getPath()) { // Unreachable vertex
					cout << "Node not reachable (" << v->getID() << "). Try again !" << endl << endl;
					continue;
				}
				else { // All goochy
					selected = v;
					break;
				}
			}
			else { // Vertex not found
				cout << "Node not found (" << id << "). Try again !" << endl << endl;
				continue;
			}
		}
		else { // Non numeric input
			if (regex_match(input, esc)) { // Esc-type input, exit
				selected = nullptr;
				break;
			} else { // Anything else
				cout << "Invalid input (" << input << "). Try again !" << endl << endl;
			}
		}
	}

	graph->hideAllVertexLabels();
	graph->update();
	if (mustBeReachable) graph->clear();
	return selected;
}


/**
 * @brief Choose any edge in the Graph.
 * To allow the choice of the edge, all edge IDs
 * are temporarily displayed. Once finished the
 * edge IDs are hidden once again.
 * @param maybeAccidented If the vertex may be accidented.
 */
Edge* selectEdge(bool maybeAccidented) {
	graph->showAllEdgeLabels();
	graph->update();
	Edge* selected;

	while (1) {
		string input;
		cout << "Select edge (write esc to cancel): ";
		cin >> input;

		if (validNumberInput(input)) {
			int eid = stoi(input);
			Edge* e = graph->findEdge(eid);

			if (e != nullptr) { // Edge found
				if (!maybeAccidented && e->isAccidented()) { // Illegal accidented edge
					cout << "Accidented edge (" << input << "). Try again !" << endl << endl;
					continue;
				}
				else { // All goochy
					selected = e;
					break;
				}
			}
			else { // Edge not found
				cout << "Edge not found (" << input << "). Try again !" << endl << endl;
				continue;
			}
		}
		else { // Non numeric input
			if (regex_match(input, esc)) { // Esc-type input, exit
				selected = nullptr;
				break;
			} else { // Anything else
				cout << "Invalid input (" << input << "). Try again !" << endl << endl;
			}
		}
	}

	graph->hideAllEdgeLabels();
	graph->update();
	return selected;
}


/**
 * @brief Initialises and loops the interactive Menu tree.
 */
void mainMenu() {
	system("cls");

	int option;

	while (1) {
		cout << "Main Menu" << endl << endl;

		cout << "1 - Cause an accident" << endl;
		cout << "2 - Fix accident" << endl;
		cout << "3 - Edit road information" << endl;
		cout << "4 - Get shortest path" << endl;
		cout << "5 - System information" << endl;
		cout << "6 - Exit" << endl;

		option = getOption(6);
		if (option == 6) return;

		system("cls");

		switch (option)
		{
		case 1:
			causeAccident();
			break;
		case 2:
			fixAccident();
			break;
		case 3:
			editRoadInfo();
			break;
		case 4:
			shortestPathUI();
			break;
		case 5:
			systemInformation();
			break;
		}

		system("cls");
	}
}


/**
 * @brief Prints the list for color conventions
 */
void systemInformation() {
	cout << "System information" << endl << endl;

	cout << "Node and edge colors meaning:" << endl;
	cout << align(VERTEX_CLEAR_COLOR, 12)      << "-> Available nodes" << endl;
	cout << align(ACCIDENTED_COLOR, 12)        << "-> Accidented nodes" << endl;
	cout << align(PATH_COLOR, 12)              << "-> Chosen path" << endl;
	cout << align(NEXT_PATH_COLOR, 12)         << "-> Predicted path" << endl;
	cout << align(SELECTED_COLOR, 12)          << "-> Selected node" << endl;
	cout << align(UNREACHABLE_COLOR, 12)       << "-> Unreachable nodes" << endl;

	system("pause");
}
