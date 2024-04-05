#pragma once

#include <iostream>
#include "Data.hpp"
#include "PartPlayer.hpp"
#include "string"

using namespace sf;

class Player {

public:

	Player(std::string name);

	void update();

	void draw(RenderWindow& window);

	void setPosition(Vector2<double> position);

	Vector2<double> getPosition();

	void setMass(double mass);

	double getMass();

	size_t getCountParts();

	std::vector<PartPlayer*> getPartsPlayer();

	void setRadius(double radius);

	void clearParts();

	void addPart(PartPlayer* new_part);

	void setName(std::string name);

	std::string getName();

private:

	std::vector<PartPlayer*> parts_player;

	size_t count_parts;

	double mass;
	double radius;
	Vector2<double> position;
	std::string name;
};