#pragma once

#include <iostream>
#include "boost/random.hpp"
#include "Data.hpp"
#include <SFML/Network.hpp>
#include "Food.hpp"

using namespace sf;

class Player {

public:

	Player(TcpSocket* socket, Vector2<double> position);

	void move();
	
	Vector2<double> getPosition();

	void setPosition(Vector2<double> position);

	double getRadius();

	void update();

	void setMass(double mass);

	double getMass();

	TcpSocket* getSocket();

	double getZoom();

	void setLastMousePos(Vector2<double> last_mouse_pos);

	std::tuple<int, int, int> getColor();

	void strikePlayer(std::vector<Food*>& food_players);

	void segmentationPlayer();

private:

	TcpSocket* socket;

	double mass;
	double radius;
	Vector2<double> position;
	Vector2<double> last_mouse_pos;
	double velocity;
	double zoom;

	double dist;
	Vector2<double> direction;

	std::tuple<int, int, int> color;
};