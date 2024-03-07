#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "boost/random.hpp"
#include "Data.hpp"

using namespace sf;

class Food {

public:

	Food(int id, Vector2<double> position, double radius, double mass);

	Vector2<double> getPosition();

	double getRadius();

	double getMass();

	void regeneratePosition();

	int getId();

private:

	int id;

	double radius;

	double mass;

	Vector2<double> position;
};