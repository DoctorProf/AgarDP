#include "../Headers/Player.hpp"


Player::Player(TcpSocket*& socket, Vector2<double> position) : socket(socket) {

	this->position = position;
	this->mass = 50;
	this->color = data::randomColor();
}

void Player::move(Vector2<int> size_map) {

	update();

	dist = data::distance(last_mouse_pos, position);

	direction = (last_mouse_pos - position) / dist;

	if (dist < radius || !data::frame_collision(position + velocity * direction, size_map)) return;

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

	radius = pow(mass, 1/ 1.5);
	velocity = 80.0 / sqrt(mass) + 2.0;
	zoom = 50.0 / radius + 0.2;
	mass = mass * 0.99992;
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

void Player::strikePlayer(std::vector<Food*>& food_players) {

	if (mass > 200) {

		Vector2<double> pos = position + (radius * direction);

		Food* new_food = new Food(food_players.size(), pos, 15.0 * direction, 10, 20);
		new_food->setColor(color);
		food_players.push_back(new_food);
		mass -= 20;
	}
}

void Player::segmentationPlayer() {


}

