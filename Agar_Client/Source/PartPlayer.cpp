#include "../Headers/PartPlayer.hpp"


PartPlayer::PartPlayer(double& radius, Vector2<double>& position) {

	part.setPointCount(100);
	this->radius = radius;
	this->position = position;
}

void PartPlayer::draw(RenderWindow& window) {

	update();
	window.draw(part);
}

void PartPlayer::setPosition(Vector2<double> position) {

	this->position = position;
}

Vector2<double> PartPlayer::getPosition() {

	return this->position;
}

void PartPlayer::setMass(double mass) {

	this->mass = mass;
}

double PartPlayer::getMass() {

	return this->mass;
}

double PartPlayer::getRadius() {

	return radius;
}

void PartPlayer::setRadius(double radius) {

	this->radius = radius;
}

void PartPlayer::update() {

	part.setRadius(radius);
	part.setOrigin(radius, radius);
	part.setPosition(Vector2f(position));
}

void PartPlayer::setColor(int r, int g, int b) {

	part.setFillColor(Color(r, g, b));
}
