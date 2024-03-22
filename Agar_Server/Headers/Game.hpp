#pragma once

#include <iostream>
#include "Player.hpp"
#include "Food.hpp"
#include <thread>

#include <SFML/Network.hpp>

using namespace sf;

class Game {

public:

	Game(int& count_food, Vector2<int>& size_map);

	void connectPlayer(TcpSocket*& socket);

	void sendPositionFood(Player*& player);

	void getFromPlayer();

	void moveFoodPlayers();

	void movePlayer();

	void collisionFood();

	void collisionPlayers();

	void updateFood();

	void updatePlayers();

	void sendToPlayer();

	void GameThread();

private:

	std::vector<Player*> players;
	std::vector<Food*> foods;

	std::vector<Food*> update_food;
	std::vector<Food*> food_players;

	int count_food;
	Vector2<int> size_map;
};