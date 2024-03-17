#pragma once

#include <iostream>
#include "Data.hpp"

using namespace sf;

class Food {

public:

	Food(int id, double radius, Vector2<double> position);

	void draw(RenderWindow& window);

	void setPosition(Vector2<double> position);

	int getId();

	void setColor(int r, int g, int b);

private:

	CircleShape food;

	int id;
};