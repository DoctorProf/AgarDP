#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "boost/random.hpp"
#include "Data.hpp"

using namespace sf;

class Player {

public:

	Player();

	void draw(RenderWindow& window);

	void setPosition(Vector2<double> position);

	Vector2<double> getPosition();

	void setMass(double mass);

	double getMass();

	void setRadius(double radius);

	void update();

	void setColor(int r, int g, int b);

private:

	CircleShape player;

	double mass;
	double radius;
	Vector2<double> position;
};