#include "../Headers/Food.hpp"


Food::Food(int id, Vector2<double> position, double radius, double mass) {

	this->id = id;
	this->position = position;
	this->radius = radius;
	this->mass = mass;
}

Vector2<double> Food::getPosition() {

	return position;
}

double Food::getRadius() {

	return radius;
}

double Food::getMass() {

	return mass;
}

void Food::regeneratePosition() {

	position = Vector2<double>(data::generateNumber(-3840, 3840), data::generateNumber(-2160, 2160));
}

int Food::getId() {

	return id;
}

