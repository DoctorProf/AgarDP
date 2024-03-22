#include "../Headers/Game.hpp"

Game::Game(int& count_food, Vector2<int>& size_map) {

	this->count_food = count_food;
	this->size_map = size_map;

	for (int i = 0; i < count_food; i++) {

		foods.push_back(new Food(i, Vector2<double>(data::generateNumber(-size_map.x / 2, size_map.x / 2), data::generateNumber(-size_map.y / 2, size_map.y / 2)), { 0.0, 0.0 }, 5, 1));
	}
}

void Game::connectPlayer(TcpSocket*& socket) {

	Player* player = new Player(socket, Vector2<double>(data::generateNumber(-size_map.x / 2, size_map.x / 2), data::generateNumber(-size_map.y / 2, size_map.y / 2)));

	players.push_back(player);

	std::cout << "Players - " << players.size() << "\n";

	Packet packect_size_map;

	packect_size_map << size_map.x << size_map.y;

	player->getSocket()->send(packect_size_map);

	std::thread send_position_food(&Game::sendPositionFood, this, std::ref(player));
	send_position_food.join();
}

void Game::sendPositionFood(Player*& player) {

	Packet packet_food;

	packet_food << foods.size();

	for (int i = 0; i < foods.size(); i++) {

		Vector2<double> position = foods[i]->getPosition();

		packet_food << foods[i]->getId() << position.x << position.y;
	}
	player->getSocket()->send(packet_food);
}

void Game::getFromPlayer() {

	for (Player* player : players) {

		Packet packet_mouse_pos;

		if (player->getSocket()->receive(packet_mouse_pos) != sf::Socket::Done) {

			players.erase(std::remove(players.begin(), players.end(), player), players.end());
			delete player;

			std::cout << "Players - " << players.size() << "\n";
			return;
		}

		double x;
		double y;
		bool strike;
		bool segmentation;

		packet_mouse_pos >> x >> y >> strike >> segmentation;
		player->setLastMousePos(Vector2<double>(x, y));

		if (strike) player->strikePlayer(food_players);
		if (segmentation) player->segmentationPlayer();
	}
}

void Game::moveFoodPlayers() {

	for (Food* food : food_players) {

		food->move(size_map);
	}
}

void Game::movePlayer() {

	for (Player* player : players) {

		player->move(size_map);
	}
}

void Game::collisionFood() {

	for (Player* player : players) {

		update_food.clear();

		while (true) {

			auto food = std::find_if(foods.begin(), foods.end(), [&](const auto& it) {

				return data::distance(player->getPosition(), it->getPosition()) < player->getRadius() && it->getRadius() < player->getRadius();
				});

			if (food == foods.end()) break;

			player->setMass(player->getMass() + (*food)->getMass());
			(*food)->regeneratePosition();

			update_food.push_back(*food);
		}
		while (true) {

			auto food_player = std::find_if(food_players.begin(), food_players.end(), [&](const auto& it) {

				return data::distance(player->getPosition(), it->getPosition()) < player->getRadius() && it->getRadius() < player->getRadius();
				});

			if (food_player == food_players.end()) break;

			player->setMass(player->getMass() + (*food_player)->getMass());

			delete* food_player;
			food_players.erase(food_player);
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

	for (Player* player : players) {

		Packet packet_food_update;

		packet_food_update << update_food.size();

		for (Food* food : update_food) {

			packet_food_update << food->getId() << food->getPosition().x << food->getPosition().y;
		}

		player->getSocket()->send(packet_food_update);
	}
}

void Game::updatePlayers() {

	for (Player* player : players) {

		Packet packet_player_data;
		Packet packet_food_players;

		packet_player_data << (size_t)(players.size() - 1);

		for (Player* player1 : players) {

			if (player == player1) continue;

			std::tuple<int, int, int> color = player1->getColor();

			packet_player_data << player1->getRadius() << player1->getPosition().x << player1->getPosition().y << std::get<0>(color) << std::get<1>(color) << std::get<2>(color);

		}
		player->getSocket()->send(packet_player_data);

		packet_food_players << food_players.size();

		for (int i = 0; i < food_players.size(); i++) {

			Vector2<double> position = food_players[i]->getPosition();

			std::tuple<int, int, int> color = food_players[i]->getColor();

			packet_food_players << food_players[i]->getId() << position.x << position.y << std::get<0>(color) << std::get<1>(color) << std::get<2>(color);
		}
		player->getSocket()->send(packet_food_players);
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

void Game::GameThread() {
	

}
