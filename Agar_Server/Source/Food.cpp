#include "../Headers/Food.hpp"


Food::Food(int id, Vector2<double> position, Vector2<double> velocity, double radius, double mass) {

	this->id = id;
	this->position = position;
	this->velocity = velocity;
	this->radius = radius;
	this->mass = mass;
}

void Food::move(Vector2<int> size_map) {

	if (!data::frame_collision(position + velocity, size_map)) return;
	velocity *= 0.99;
	position += velocity;
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

void Food::setColor(std::tuple<int, int, int> color) {

	this->color = color;
}

std::tuple<int, int, int> Food::getColor() {

	return color;
}