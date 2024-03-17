#include "../Headers/Player.hpp"


Player::Player() {

	player.setPointCount(100);
}

void Player::draw(RenderWindow& window) {

	update();
	window.draw(player);
}

void Player::setPosition(Vector2<double> position) {

	this->position = position;
}

Vector2<double> Player::getPosition() {
	
	return this->position;
}


void Player::setMass(double mass) {

	this->mass = mass;
}

double Player::getMass() {
	
	return this->mass;
}

void Player::setRadius(double radius) {

	this->radius = radius;
}

void Player::update() {

	player.setRadius(radius);
	player.setOrigin(radius, radius);
	player.setPosition(Vector2f(position));
}

void Player::setColor(int r, int g, int b) {

	player.setFillColor(Color(r, g, b));
}
