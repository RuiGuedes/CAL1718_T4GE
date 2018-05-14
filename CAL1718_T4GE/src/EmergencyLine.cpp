#include "Interface.h"
#include "SearchAlgorithms.h"

//////////////////////////
// Functions Prototypes //
//////////////////////////

void printAllRoads() {

	map<string,Road *>::iterator it = graph->roadsInfo.begin();

	while(it != graph->roadsInfo.end()) {
		cout << it->second->getID() << " - " << it->first << endl;
		it++;
	}

    cout << endl;
    system("pause");
}

void emergencyLine() {

	int option;

	cout << "Emergency Line" << endl << endl;

	cout << "1 - Exact string matching" << endl;
	cout << "2 - Approximate string matching" << endl << endl;

	option = selectOption(3);
	if (option == 3) return;

	system("cls");

	switch(option)
	{
	case 1:
		printAllRoads();
		break;
	case 2:

		break;
	}

	system("cls");

	return emergencyLine();

}
