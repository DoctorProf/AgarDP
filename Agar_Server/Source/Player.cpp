#include "../Headers/Player.hpp"


Player::Player(TcpSocket* socket, Vector2<double> position) : socket(socket) {

	this->position = position;
	this->mass = 20;

	this->color = data::randomColor();
}

void Player::move() {

	update();

	double dist = data::distance(last_mouse_pos, position);

	Vector2<double> direction = (last_mouse_pos - position) / dist;

	if (dist < radius || !data::frame_collision(position + velocity * direction, { 7680, 4320 })) return;

	position += velocity * direction;
}

Vector2<double> Player::getPosition() {

	return position;
}

void Player::setPosition(Vector2<double> position) {

	this->position = position;
}

double Player::getRadius() {

	return radius;
}

void Player::update() {

	radius = pow(mass, 1 / 1.5);
	velocity = 80.0 / sqrt(mass) + 3.0;
	zoom = 50.0 / radius + 0.2;
	mass = mass * 0.99995;
}

void Player::setMass(double mass) {

	this->mass = mass;
}

double Player::getMass() {

	return mass;
}

TcpSocket* Player::getSocket() {

	return socket;
}

double Player::getZoom() {

	return zoom;
}

void Player::setLastMousePos(Vector2<double> last_mouse_pos) {

	this->last_mouse_pos = last_mouse_pos;
}

std::tuple<int, int, int> Player::getColor() {

	return color;
}

