#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "boost/random.hpp"
#include "Data.hpp"

using namespace sf;

class Food {

public:

	Food(int id, Vector2<double> position, Vector2<double> velocity, double radius, double mass);

	void move();

	Vector2<double> getPosition();

	double getRadius();

	double getMass();

	void regeneratePosition();

	int getId();

	void setColor(std::tuple<int, int, int> color);

	std::tuple<int, int, int> getColor();

private:

	int id;

	double radius;

	double mass;

	Vector2<double> position;

	Vector2<double> velocity;

	std::tuple<int, int, int> color;
};