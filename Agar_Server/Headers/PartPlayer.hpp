#pragma once

#include <iostream>
#include "boost/random.hpp"
#include "Data.hpp"
#include <SFML/Network.hpp>
#include "Food.hpp"
#include "Player.hpp"

using namespace sf;

class Player;

class PartPlayer {

public:

	PartPlayer(Player* player, Vector2<double>& position, double mass, double pulse = 0);

	void move(double& dist, Vector2<double>& direction, Vector2<int>& size_map);

	Vector2<double> getPosition();

	void setPosition(Vector2<double> position);

	double getRadius();

	Player*& getPlayer();

	void update();

	void setMass(double mass);

	double getMass();

private:

	Player* player;
	double mass;
	double radius;
	Vector2<double> position;
	double velocity;
	double pulse;
};