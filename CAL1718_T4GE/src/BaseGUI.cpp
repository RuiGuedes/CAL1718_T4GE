#include "FunctionsPrototypes.h"
#include "BaseGUI.h"

#include <assert.h>

bool validIDInput(string input, int max = 0) {
	static const regex reg_int("^\\s*(\\d+)[.,;]?\\s*$");
	smatch match;

	if (regex_match(input, match, reg_int)) {
		try {
			int value = stoi(input);

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
int getOption(int MAX) {
	while(1) {
		string input;
		cout << "Select option: ";
		getline(cin, input);

		if(validIDInput(input,MAX)) {
			return stoi(input);
		}
		else {
			cout << "Unavailable option (" << input << "). Try again !" << endl << endl;
		}
	}
}

Vertex* getOriginVertex(bool maybeAccidented, bool color) {
	static const regex esc("^\s*esc[\\.,;]?\s*$|^$", regex::icase); // esc or plain enter

	while(1) {
		string input;
		cout << "Select starting node (write esc to cancel): ";
		getline(cin, input);

		if(validIDInput(input)) {
			int id = stoi(input);
			Vertex* v = graph->findVertex(id);

			if (v != nullptr) { // Vertex found
				if (!maybeAccidented && v->isAccidented()) {
					cout << "Accidented node (" << input << "). Try again !" << endl << endl;
					continue;
				}
				else {
					if (color) graph->setVertexColor(v, VERTEX_SELECTED_COLOR);
					return v;
				}
			}
			else { // Vertex not found
				cout << "Node not found (" << input << "). Try again !" << endl << endl;
				continue;
			}
		}
		else { // Non numeric input
			if (regex_match(input, esc)) {
				return nullptr;
			} else {
				cout << "Invalid input (" << input << "). Try again !" << endl << endl;
			}
		}
	}
}

Vertex* getDestinationVertex(bool maybeAccidented, Vertex *source, bool mustBeReachable, bool color) {
	static const regex esc("^\s*(?:esc|quit|q)[\\.,;]?\s*$", regex::icase); // 'esc' or 'quit' or 'q'
	assert(!mustBeReachable || source);

	while(1) {
		string input;
		cout << "Select destination node (write esc to cancel): ";
		getline(cin, input);

		if (validIDInput(input)) {
			int id = stoi(input);
			Vertex* v = graph->findVertex(id);

			if (v != nullptr) { // Vertex found
				if (!maybeAccidented && v->isAccidented()) {
					cout << "Accidented node (" << input << "). Try again !" << endl << endl;
					continue;
				}
				else if (mustBeReachable && !source->isAdjacentTo(v)) {
					cout << "Node not reachable (" << input << "). Try again !" << endl << endl;
					continue;
				}
				else {
					if (color) graph->setVertexColor(v, VERTEX_SELECTED_COLOR);
					return v;
				}
			}
			else { // Vertex not found
				cout << "Node not found (" << input << "). Try again !" << endl << endl;
				continue;
			}
		}
		else { // Non numeric input
			if (regex_match(input, esc)) {
				return nullptr;
			} else {
				cout << "Invalid input (" << input << "). Try again !" << endl << endl;
			}
		}
	}
}
