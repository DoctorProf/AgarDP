#include "../Headers/Player.hpp"


Player::Player() {


}

void Player::draw(RenderWindow& window) {

	for (PartPlayer* part_player : parts_player) {

		part_player->draw(window);
	}
}

void Player::setPosition(Vector2<double> position) {

	this->position = position;
}

Vector2<double> Player::getPosition() {
	
	update();
	return this->position;
}


void Player::setMass(double mass) {

	this->mass = mass;
}

double Player::getMass() {
	
	return this->mass;
}

size_t Player::getCountParts() {

	return count_parts;
}

std::vector<PartPlayer*> Player::getPartsPlayer() {

	return parts_player;
}

void Player::update() {

	count_parts = parts_player.size();
	position = parts_player[0]->getPosition();
}

void Player::clearParts() {

	parts_player.clear();
}

void Player::addPart(PartPlayer* new_part) {

	parts_player.push_back(new_part);
}
