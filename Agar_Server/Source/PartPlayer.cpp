#include "../Headers/PartPlayer.hpp"


PartPlayer::PartPlayer(Player* player, Vector2<double>& position, double mass, double pulse) : player(player) {

	this->position = position;
	this->mass = mass;
	this->pulse = pulse;
}

void PartPlayer::move(double& dist, Vector2<double>& direction, Vector2<int>& size_map) {

	update();

	if (dist < radius || !data::frame_collision(position + velocity * direction, size_map)) return;

	position += velocity * direction;
}

Vector2<double> PartPlayer::getPosition() {

 	return position;
}

void PartPlayer::setPosition(Vector2<double> position) {

	this->position = position;
}

double PartPlayer::getRadius() {

	return radius;
}

Player*& PartPlayer::getPlayer() {

	return player;
}

void PartPlayer::update() {

	pulse *= 0.95;
	radius = pow(mass, 1 / 1.5);
	velocity = 80.0 / sqrt(mass) + 1.5 + pulse;
	mass *= 0.99992;
}

void PartPlayer::setMass(double mass) {

	this->mass = mass;
}

double PartPlayer::getMass() {

	return mass;
}