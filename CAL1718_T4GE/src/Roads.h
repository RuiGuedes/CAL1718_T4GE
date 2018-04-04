#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "Graph.h"

using namespace std;

class Road;
class Subroad;

class Road {
	const int id;
	const string name;
	double totalDistance = 0;
	bool bothways;
	int averageSpeed;

public:
	Road(int rid, string name, bool bothways = false);

	///// ***** Road CRUD
	// C
	// R
	int getID() const;
	string getName() const;
	double getTotalDistance() const;
	int getAverageSpeed() const;
	// U
	bool setTotalDistance(double distance);
	// D
	/////

	friend class Subroad;
};

class Subroad {
	const double distance;
	Road *const road;

	int actualCapacity = 0;
	int maxCapacity;

public:
	Subroad(double distance, Road* road);

	///// ***** Subroad CRUD
	// C
	// R
	Road *getRoad() const;
	double getDistance() const;
	int getActualCapacity() const;
	int getMaxCapacity() const;
	int calculateAverageSpeed() const;
	// U
	bool setAtualCapacity(int actualCapacity);
	// D
	/////

	friend class Road;
};

