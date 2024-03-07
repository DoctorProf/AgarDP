#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "boost/random.hpp"
#include "Data.hpp"

using namespace sf;

class Food {

public:

	Food(int id, Vector2<double> position);


	void draw(RenderWindow& window);

	Vector2<double> getPosition();

	void setPosition(Vector2<double> position);

	double getRadius();

	int getId();

private:

	CircleShape food;

	int id;
	double radius;

	Vector2<double> position;
};