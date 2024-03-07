#include "../Headers/Food.hpp"


Food::Food(int id, Vector2<double> position) {

	radius = 5;

	food.setOrigin(radius, radius);
	food.setPosition(Vector2f(position));
	food.setFillColor(data::randomColor());
	food.setRadius(radius);
	food.setOutlineThickness(1);
	food.setOutlineColor(Color::Black);

	this->id = id;
	this->position = position;
}

void Food::draw(RenderWindow& window) {

	food.setOrigin(radius, radius);
	food.setPosition(Vector2f(position));
	window.draw(food);
}

Vector2<double> Food::getPosition() {

	return position;
}

void Food::setPosition(Vector2<double> position) {

	this->position = position;
}

double Food::getRadius() {

	return radius;
}

int Food::getId() {

	return id;
}
