#include <math.h>

#include "Roads.h"

////////////////
// Class Road //
////////////////

Road::Road(int rid, string name, bool bothways):
	id(rid), name(name), bothways(bothways) {
	// DUMMY
	if (totalDistance > 1.0)
		averageSpeed = 120;
	if (totalDistance > 0.5)
		averageSpeed = 90;
	if (totalDistance > 0.25)
		averageSpeed = 70;
	if (totalDistance > 0.1)
		averageSpeed = 50;
	averageSpeed = 30;
}

int Road::getID() const {
	return id;
}

string Road::getName() const {
	return name;
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
	return true;
}





///////////////////
// Class Subroad //
///////////////////

Subroad::Subroad(double distance, Road* road):
		distance(distance), road(road) {
	maxCapacity = ceil(distance * 300);
	actualCapacity = rand() % maxCapacity;
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

bool Subroad::setAtualCapacity(int capacity) {
	if (actualCapacity <= 0 || actualCapacity > maxCapacity) return false;
	actualCapacity = capacity;
	return true;
}
