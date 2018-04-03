#include "Road.h"

//////////////////
// Constructors //
//////////////////

Road::Road(int ID, string name, double distance, int averageSpeed) {
	this->ID = ID;
	this->name = name;
	this->distance = distance;
	this->averageSpeed = averageSpeed;
	this->actualCapacity = 0;
	this->maxCapacity = 0;
}

Road::Road(int ID, string name, double distance, int averageSpeed, int actualCapacity, int maxCapacity) {
	this->ID = ID;
	this->name = name;
	this->distance = distance;
	this->averageSpeed = averageSpeed;
	this->actualCapacity = actualCapacity;
	this->maxCapacity = maxCapacity;
}

/////////////////
// Get Methods //
/////////////////

int Road::getID() const {
	return ID;
}

string Road::getName() const {
	return name;
}

int Road::getAverageSpeed() const {
	return averageSpeed;
}

double Road::getDistance() const {
	return distance;
}

int Road::getActualCapacity() const {
	return actualCapacity;
}

int Road::getMaxCapacity() const {
	return maxCapacity;
}

double Road::getWeight() {
	return distance/calculateAverageSpeed();
}

/////////////////
// Set Methods //
/////////////////

void Road::setName(string name) {
	this->name = name;
}

void Road::setAtualCapacity(int actualCapacity) {
	this->actualCapacity = actualCapacity;
}

void Road::setMaxCapacity(int maxCapacity) {
	this->maxCapacity = maxCapacity;
}


////////////
// Others //
////////////

int Road::calculateAverageSpeed() {
	if(maxCapacity == 0)
		return averageSpeed;

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
