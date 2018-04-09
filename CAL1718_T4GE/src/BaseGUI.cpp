#include "FunctionsPrototypes.h"

#include <assert.h>

bool validIDInput(string input, int max) {
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
		cin >> input;

		if(validIDInput(input,MAX)) {
			return stoi(input);
		}
		else {
			cout << "Unavailable option (" << input << "). Try again !" << endl << endl;
		}
	}
}

Vertex* getOriginVertex(bool maybeAccidented) {
	static const regex esc("^\\s*esc[\\.,;]?\\s*$|^$", regex::icase); // esc or plain enter

	while(1) {
		string input;
		cout << "Select starting node (write esc to cancel): ";
		cin >> input;

		if(validIDInput(input)) {
			int id = stoi(input);
			Vertex* v = graph->findVertex(id);

			if (v != nullptr) { // Vertex found
				if (!maybeAccidented && v->isAccidented()) {
					cout << "Accidented node (" << input << "). Try again !" << endl << endl;
					continue;
				}
				else {
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

/**
 * (1) maybeAccidented -> if the destination vertex can be accidented or not
 * (2) mustBeReachable -> if the destination vertex must be tagged as reachable (->getPath() not null)
 */
Vertex* getDestinationVertex(bool maybeAccidented, bool mustBeReachable) {
	static const regex esc("^\\s*(?:esc|quit|q)[\\.,;]?\\s*$", regex::icase); // 'esc' or 'quit' or 'q'

	while(1) {
		string input;
		cout << "Select destination node (write esc to cancel): ";
		cin >> input;

		if (validIDInput(input)) {
			int id = stoi(input);
			Vertex* v = graph->findVertex(id);

			if (v != nullptr) { // Vertex found
				if (!maybeAccidented && v->isAccidented()) {
					cout << "Accidented node (" << input << "). Try again !" << endl << endl;
					continue;
				}
				else if (mustBeReachable && !v->getPath()) {
					cout << "Node not reachable (" << input << "). Try again !" << endl << endl;
					continue;
				}
				else {
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
