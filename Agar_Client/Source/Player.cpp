#include "../Headers/Player.hpp"


Player::Player() {


}

void Player::draw(RenderWindow& window) {

	update();
	for (PartPlayer* part_player : parts_player) {

		part_player->draw(window);
	}
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

	radius = parts_player[0]->getRadius();
	position = parts_player[0]->getPosition();
}

void Player::clearParts() {

	parts_player.clear();
}

void Player::addPart(PartPlayer* new_part) {

	parts_player.push_back(new_part);
}
