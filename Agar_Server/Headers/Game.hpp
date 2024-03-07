#pragma once

#include <iostream>
#include "Player.hpp"
#include "Food.hpp"

#include <SFML/Network.hpp>

using namespace sf;

class Game {

public:

	Game(std::vector<Player*>& players, std::vector<Food*>& foods, int count_food);

	void movePlayers();

	void collisionFood();

	void collisionPlayers();

	void updateFood();

	void updatePlayers();

	void getFromPlayer();

	void sendToPlayer();

	void sendPositionFood(Player* player);

	void checkEventShot(Player* player);

private:

	std::vector<Player*>& players;
	std::vector<Food*>& foods;

	std::vector<Food*> update_food;

	int count_food;
};