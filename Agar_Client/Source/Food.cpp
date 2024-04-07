#include "../Headers/Food.hpp"


Food::Food(int id, double radius, Vector2<double> position) {

	this->id = id;

	food.setRadius(radius);
	food.setOrigin(radius, radius);
	food.setPosition(Vector2f(position));
	food.setFillColor(data::randomColor());
}

void Food::draw(RenderWindow& window) {

	window.draw(food);
}

void Food::setPosition(Vector2<double> position) {

	food.setPosition(Vector2f(position));
}

int Food::getId() {

	return id;
}

void Food::setColor(int r, int g, int b) {

	food.setFillColor(Color(r, g, b));
}