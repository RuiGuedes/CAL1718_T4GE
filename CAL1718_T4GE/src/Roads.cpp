#include <math.h>
#include <stdlib.h>

#include "Graph.h"

static int maxCapacityEstimation(double distance) {
	return 10 + (distance * 0.25);
}



////////////////
// Class Road //
////////////////

Road::Road(int rid, string name, bool bothways):
	id(rid), name(name), bothways(bothways) {}

int Road::getID() const {
	return id;
}

string Road::getName() const {
	return name;
}

bool Road::isBidirectional() const {
	return bothways;
}

double Road::getTotalDistance() const {
	return totalDistance;
}

int Road::getMaxSpeed() const {
	return maxSpeed;
}

bool Road::setTotalDistance(double distance) {
	if (totalDistance != 0 || distance <= 0) return false;
	totalDistance = distance;

	if (distance > 1000)
		maxSpeed = 120;
	else if (distance > 500)
		maxSpeed = 90;
	else if (distance > 250)
		maxSpeed = 70;
	else
		maxSpeed = 50;

	return true;
}




///////////////////
// Class Subroad //
///////////////////

Subroad::Subroad(double distance, Road* road):
		distance(distance), road(road) {
	maxCapacity = maxCapacityEstimation(distance);
	actualCapacity = (rand() % maxCapacity);
}

Road* Subroad::getRoad() const {
	return road;
}

double Subroad::getDistance() const {
	return distance;
}

int Subroad::getActualCapacity() const {
	return actualCapacity;
}

int Subroad::getMaxCapacity() const {
	return maxCapacity;
}

int Subroad::calculateAverageSpeed() const {
	int maxSpeed = road->getMaxSpeed();

	if(((double)actualCapacity/maxCapacity)*100 <= 25)
		return maxSpeed;
	else if(((double)actualCapacity/maxCapacity)*100 <= 50)
		return maxSpeed*0.75;
	else if(((double)actualCapacity/maxCapacity)*100 <= 75)
		return (double)maxSpeed*0.5;
	else if((actualCapacity/maxCapacity)*100 <= 90)
		return maxSpeed*0.2;

	return maxSpeed*0.1;
}

bool Subroad::setActualCapacity(int capacity) {
	if (actualCapacity <= 0 || actualCapacity > maxCapacity) return false;
	actualCapacity = capacity;
	return true;
}
