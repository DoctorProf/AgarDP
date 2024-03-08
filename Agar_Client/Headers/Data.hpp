#pragma once

#include <boost/random.hpp>
#include <SFML/Graphics.hpp>

using namespace sf;

namespace data {

	extern boost::random::mt19937 rng;

	extern Font font;

	double distance(Vector2<double> vec1, Vector2<double> vec2);

	int generateNumber(int min, int max);

	Color randomColor();

	bool frame_collision(Vector2<double> pos, Vector2<int> size_map);

	void loadFont();
}
