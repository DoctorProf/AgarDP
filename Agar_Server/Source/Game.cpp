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

		if (player->getSocket()->receive(packet_mouse_pos) != sf::Socket::Done || player->getPartsPlayer().size() == 0) {

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
		bool foodCollision = true;

		while (foodCollision) {

			foodCollision = false;
			for (PartPlayer* part_player : player->getPartsPlayer()) {

				for (auto it = foods.begin(); it != foods.end(); ) {

					if (data::distance(part_player->getPosition(), (*it)->getPosition()) < part_player->getRadius() && (*it)->getRadius() < part_player->getRadius()) {

						part_player->setMass(part_player->getMass() + (*it)->getMass());
						(*it)->regeneratePosition();
						update_food.push_back(*it);
						it = foods.erase(it);
						foodCollision = true;
					}
					else {

						++it;
					}
				}
				for (auto it = food_players.begin(); it != food_players.end(); ) {

					if (data::distance(part_player->getPosition(), (*it)->getPosition()) < part_player->getRadius() && (*it)->getRadius() < part_player->getRadius()) {

						part_player->setMass(part_player->getMass() + (*it)->getMass());
						delete* it;
						it = food_players.erase(it);
						foodCollision = true;
					}
					else {

						++it;
					}
				}
			}
		}
	}
}

void Game::collisionPlayers() {

	std::vector<PartPlayer*> parts_to_remove{};

	for (Player* player : players) {

		for (Player* other_player : players) {

			for (PartPlayer* part_player : player->getPartsPlayer()) {

				for (PartPlayer* part_other_player : other_player->getPartsPlayer()) {

					if (part_player == part_other_player) continue;

					if (data::distance(part_player->getPosition(), part_other_player->getPosition()) < abs(part_player->getRadius() - part_other_player->getRadius())) {

						if (part_player->getMass() > part_other_player->getMass()) {

							auto it = std::find(other_player->getPartsPlayer().begin(), other_player->getPartsPlayer().end(), part_other_player);

							if (it != other_player->getPartsPlayer().end()) {

								part_player->setMass(part_player->getMass() + part_other_player->getMass());
								parts_to_remove.push_back(part_other_player);
							}
						}
						else if (part_player->getMass() < part_other_player->getMass()) {

							auto it = std::find(player->getPartsPlayer().begin(), player->getPartsPlayer().end(), part_player);

							if (it != player->getPartsPlayer().end()) {

								part_other_player->setMass(part_other_player->getMass() + part_player->getMass());
								parts_to_remove.push_back(part_player);
							}
						}
					}
				}
			}
		}
	}

	std::sort(parts_to_remove.begin(), parts_to_remove.end());
	parts_to_remove.erase(std::unique(parts_to_remove.begin(), parts_to_remove.end()), parts_to_remove.end());

	for (PartPlayer*& part : parts_to_remove) {

		Player*& player = part->getPlayer();

		std::vector<PartPlayer*>& parts = player->getPartsPlayer();

		auto it = std::find(parts.begin(), parts.end(), part);

		parts.erase(it);
		delete part;
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

			packet_player_data << player1->getPartsPlayer().size() << std::get<0>(color) << std::get<1>(color) << std::get<2>(color) ;

			for (PartPlayer* part_player : player1->getPartsPlayer()) {

				packet_player_data << part_player->getRadius() << part_player->getPosition().x << part_player->getPosition().y;
			}
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

		packet_player << player->getPartsPlayer().size() << player->getZoom() << player->getMass() << std::get<0>(color) << std::get<1>(color) << std::get<2>(color) ;

		for (PartPlayer* part_player : player->getPartsPlayer()) {

			packet_player << part_player->getRadius() << part_player->getPosition().x << part_player->getPosition().y;
		}

		player->getSocket()->send(packet_player);
	}
}

void Game::GameThread() {
	
}
