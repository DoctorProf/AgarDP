#include "../Headers/Player.hpp"

Player::Player(TcpSocket*& socket, Vector2<double> position) : socket(socket) {

	this->position = position;
	this->color = data::randomColor();
	parts_player.push_back(new PartPlayer(position, 50.0));
}

void Player::move(Vector2<int> size_map) {

	zoom = 50.0 / parts_player[0]->getRadius() + 0.2;
	
	position = parts_player[0]->getPosition();

	dist = data::distance(last_mouse_pos, position);

	direction = (last_mouse_pos - position) / dist;
	
	for (PartPlayer* part_player : parts_player) {

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

std::vector<PartPlayer*> Player::getPartsPlayer() {

	return parts_player;
}

void Player::removePart(PartPlayer* part) {

	auto it = std::find(parts_player.begin(), parts_player.end(), part);
	if (it != parts_player.end()) {

		parts_player.erase(it);
	}
}

void Player::strikePlayer(std::vector<Food*>& food_players) {

	for (PartPlayer* part_player : parts_player) {

		if (part_player->getMass() > 200) {

			Vector2<double> pos = position + (part_player->getRadius() * direction);

			Food* new_food = new Food(food_players.size(), pos, 15.0 * direction, 10, 20);
			new_food->setColor(color);
			food_players.push_back(new_food);
			part_player->setMass(part_player->getMass() - 20);
		}
	}
}

void Player::segmentationPlayer() {

	int size_parts = parts_player.size();

	for (int i = 0; i < size_parts; i++) {

		Vector2<double> pos = position + (parts_player[i]->getRadius() * direction);
		PartPlayer* new_part = new PartPlayer(pos, parts_player[i]->getMass() / 2.0, 8);
		parts_player[i]->setMass(parts_player[i]->getMass() / 2);
		parts_player.push_back(new_part);
	}
}

