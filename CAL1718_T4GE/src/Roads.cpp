#include <math.h>
#include <stdlib.h>

#include "Graph.h"

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

int Road::getAverageSpeed() const {
	return averageSpeed;
}

bool Road::setTotalDistance(double distance) {
	if (totalDistance != 0 || distance <= 0) return false;
	totalDistance = distance;

	if (distance > 1.0)
		averageSpeed = 120;
	else if (distance > 0.5)
		averageSpeed = 90;
	else if (distance > 0.25)
		averageSpeed = 70;
	else
		averageSpeed = 50;

	return true;
}





///////////////////
// Class Subroad //
///////////////////

Subroad::Subroad(double distance, Road* road):
		distance(distance), road(road) {
	maxCapacity = ceil(distance * 300 + 10);
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
	int averageSpeed = road->getAverageSpeed();

	if(((double)actualCapacity/maxCapacity)*100 <= 20)
		return averageSpeed;
	else if(((double)actualCapacity/maxCapacity)*100 <= 50)
		return averageSpeed*0.75;
	else if(((double)actualCapacity/maxCapacity)*100 <= 75)
		return (double)averageSpeed*0.5;
	else if((actualCapacity/maxCapacity)*100 <= 90)
		return averageSpeed*0.2;

	return averageSpeed*0.1;
}

bool Subroad::setActualCapacity(int capacity) {
	if (actualCapacity <= 0 || actualCapacity > maxCapacity) return false;
	actualCapacity = capacity;
	return true;
}
