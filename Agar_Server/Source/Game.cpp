#include "../Headers/Game.hpp"

Game::Game(std::vector<Player*>& players, std::vector<Food*>& foods, int count_food) : players(players), foods(foods) {

	this->count_food = count_food;

	for (int i = 0; i < count_food; i++) {

		foods.push_back(new Food(i, Vector2<double>(data::generateNumber(-3840, 3840), data::generateNumber(-2160, 2160)), 5, 1));
	}
}


void Game::movePlayers() {

	for (Player*& player : players) {

		player->move();
	}
}

void Game::collisionFood() {

	update_food.clear();

	for (Player* player : players) {

		auto food = std::find_if(foods.begin(), foods.end(), [&](const auto& it) {

			return data::distance(player->getPosition(), it->getPosition()) < player->getRadius() &&
				it->getMass() < player->getMass();
			});

		if (food != foods.end()) {

			player->setMass(player->getMass() + (*food)->getMass());
			(*food)->regeneratePosition();

			update_food.push_back(*food);
		}
	}
}

void Game::collisionPlayers() {

	for (auto it = players.begin(); it != players.end(); ++it) {

		Player* player = *it;

		auto player1 = std::find_if(players.begin(), players.end(), [&](const auto& other) {

			return player != other && data::distance(player->getPosition(), other->getPosition()) < abs(player->getRadius() - other->getRadius());
			});

		if (player1 == players.end()) continue;

		if (std::abs(player->getMass() - (*player1)->getMass()) > (player->getMass() + (*player1)->getMass()) * 0.05) {

			if (player->getMass() > (*player1)->getMass()) {

				player->setMass(player->getMass() + (*player1)->getMass());
				(*player1)->getSocket()->disconnect();
				delete* player1;
				players.erase(player1);
			}
			else {

				(*player1)->setMass((*player1)->getMass() + player->getMass());
				player->getSocket()->disconnect();
				delete player;
				players.erase(it);
				break;
			}
		}
	}
}

void Game::updateFood() {

	Packet packet_food_update;

	packet_food_update << update_food.size();

	for (Food* food : update_food) {

		packet_food_update << food->getId() << food->getPosition().x << food->getPosition().y;
	}

	for (Player* player : players) {

		player->getSocket()->send(packet_food_update);
	}
}

void Game::updatePlayers() {

	for (Player* player : players) {

		Packet packet_player_data;

		packet_player_data << (size_t)(players.size() - 1);

		for (Player* player1 : players) {

			if (player == player1) continue;

			std::tuple<int, int, int> color = player1->getColor();

			packet_player_data << player1->getRadius() << player1->getPosition().x << player1->getPosition().y << std::get<0>(color) << std::get<1>(color) << std::get<2>(color);;

		}
		player->getSocket()->send(packet_player_data);
	}
}

void Game::getFromPlayer() {

	for (Player* player : players) {

		Packet packet_mouse_pos;

		if (player->getSocket()->receive(packet_mouse_pos) != sf::Socket::Done) {

			players.erase(std::remove(players.begin(), players.end(), player), players.end());
			delete player;

			std::cout << " Players - " << players.size() << "\n";

			return;
		}

		double x;
		double y;

		packet_mouse_pos >> x >> y;
		player->setLastMousePos(Vector2<double>(x, y));
	}
}

void Game::sendToPlayer() {

	for (Player* player : players) {

		Packet packet_player;

		std::tuple<int, int, int> color = player->getColor();

		packet_player << player->getRadius() << player->getMass() << player->getPosition().x << player->getPosition().y << player->getZoom() << std::get<0>(color) << std::get<1>(color) << std::get<2>(color);

		player->getSocket()->send(packet_player);
	}
}

void Game::sendPositionFood(Player* player) {

	Packet packet_food;

	packet_food << foods.size();

	for (int i = 0; i < foods.size(); i++) {

		Vector2<double> position = foods[i]->getPosition();

		packet_food << foods[i]->getId() << position.x << position.y;
	}

	player->getSocket()->send(packet_food);
}

void Game::checkEventShot(Player* player) {

	while (true) {

		Packet event_shot;

		player->getSocket()->receive(event_shot);

		std::string event_player;

		event_shot >> event_player;

		if (event_player == "LKM") {

			player->setMass(player->getMass() + 20);
		}
	}
	
}


