#pragma once

#include <iostream>
#include "Data.hpp"

using namespace sf;

class PartPlayer {

public:

	PartPlayer(double& radius, Vector2<double>& position);

	void update();

	void draw(RenderWindow& window);

	void setPosition(Vector2<double> position);

	Vector2<double> getPosition();

	void setMass(double mass);

	double getMass();

	double getRadius();

	void setRadius(double radius);

	void setColor(int r, int g, int b);

private:

	CircleShape part;

	double mass;
	double radius;
	Vector2<double> position;
};