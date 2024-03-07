#include "../Headers/Food.hpp"


Food::Food(int id, Vector2<double> position) {

	this->radius = 5;
	this->id = id;
	this->position = position;
}

Vector2<double> Food::getPosition() {

	return position;
}

double Food::getRadius() {

	return radius;
}

void Food::regeneratePosition() {

	position = Vector2<double>(data::generateNumber(-3840, 3840), data::generateNumber(-2160, 2160));
}

int Food::getId() {

	return id;
}

