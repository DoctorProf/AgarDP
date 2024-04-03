#include "../Headers/Player.hpp"

Player::Player(TcpSocket*& socket, Vector2<double> position) : socket(socket) {

	this->position = position;
	this->color = data::randomColor();
	parts_player.push_back(new PartPlayer(this, position, 50.0));
}

void Player::move(Vector2<int>& size_map) {
	
	for (PartPlayer* part_player : parts_player) {

		dist = data::distance(last_mouse_pos, position);

		direction = (last_mouse_pos - position) / dist;

		part_player->move(dist, direction, size_map);
	}
}

Vector2<double> Player::getPosition() {

	return position;
}

void Player::setPosition(Vector2<double> position) {

	this->position = position;
}

TcpSocket* Player::getSocket() {

	return socket;
}

double Player::getZoom() {

	return zoom;
}

double Player::getMass() {

	mass = 0;

	for (PartPlayer* part_player : parts_player) {

		mass += part_player->getMass();
	}

	return mass;
}

void Player::setLastMousePos(Vector2<double> last_mouse_pos) {

	this->last_mouse_pos = last_mouse_pos;
}

std::tuple<int, int, int> Player::getColor() {

	return color;
}

std::vector<PartPlayer*>& Player::getPartsPlayer() {

	zoom = 50.0 / parts_player[0]->getRadius() + 0.2;

	position = parts_player[0]->getPosition();

	return parts_player;
}

void Player::strikePlayer(std::vector<Food*>& food_players) {

	for (PartPlayer* part_player : parts_player) {

		if (part_player->getMass() > 100) {

			Vector2<double> pos = part_player->getPosition() + (part_player->getRadius() * direction);

			Food* new_food = new Food(food_players.size(), pos, 15.0 * direction, 10, 10);
			new_food->setColor(color);
			food_players.push_back(new_food);
			part_player->setMass(part_player->getMass() - 10);
		}
	}
}

void Player::segmentationPlayer() {

	int size_parts = parts_player.size();

	for (int i = 0; i < size_parts; i++) {

		if (parts_player[i]->getMass() < 200) continue;
		Vector2<double> pos = parts_player[i]->getPosition() + (parts_player[i]->getRadius() * direction);
		PartPlayer* new_part = new PartPlayer(this, pos, parts_player[i]->getMass() / 2.0, 5);
		parts_player[i]->setMass(parts_player[i]->getMass() / 2);
		parts_player.push_back(new_part);
	}
}

