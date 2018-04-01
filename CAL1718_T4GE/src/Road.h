#include <iostream>
#include <cstdio>
#include "graphviewer.h"
#include <fstream>
#include <iostream>
#include <sstream>

class Road {
	int ID;
	string name;
	double distance;
	int averageSpeed;
	int actualCapacity;
	int maxCapacity;
public:
	Road(int ID, string name, double distance, int averageSpeed);
	Road(int ID, string name, double distance, int averageSpeed, int actualCapacity, int maxCapacity);

	int getID() const;
	string getName() const;
	double getDistance() const;
	int getAverageSpeed() const;
	int getActualCapacity() const;
	int getMaxCapacity() const;
	double getWeight();

	void setName(string name);
	void setAtualCapacity(int actualCapacity);
	void setMaxCapacity(int actualCapacity);

	int calculateAverageSpeed();
};
