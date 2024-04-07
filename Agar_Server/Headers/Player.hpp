#pragma once

#include <iostream>
#include "boost/random.hpp"
#include "Data.hpp"
#include <SFML/Network.hpp>
#include "Food.hpp"
#include "PartPlayer.hpp"
#include "string"

using namespace sf;

class PartPlayer;

class Player {

public:

	Player(TcpSocket*& socket, Vector2<double> position, std::string name);

	void move(Vector2<int>& size_map);
	
	Vector2<double> getPosition();

	void setPosition(Vector2<double> position);

	TcpSocket* getSocket();

	double getZoom();

	double getMass();

	void setLastMousePos(Vector2<double> last_mouse_pos);

	std::tuple<int, int, int> getColor();

	std::vector<PartPlayer*>& getPartsPlayer();

	void strikePlayer(std::vector<Food*>& food_players, Vector2<int>& size_map);

	void segmentationPlayer(Vector2<int>& size_map);

	void setName(std::string name);

	std::string getName();

private:

	TcpSocket* socket;
	
	std::vector<PartPlayer*> parts_player;

	Vector2<double> position;
	Vector2<double> last_mouse_pos;

	double zoom;
	double mass;
	std::string name;

	double dist;
	Vector2<double> direction;

	std::tuple<int, int, int> color;
};