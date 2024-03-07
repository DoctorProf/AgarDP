#include "../Headers/Data.hpp"

boost::random::mt19937 data::rng(time(NULL));

double data::distance(Vector2<double> vec1, Vector2<double> vec2)
{
	double dx = vec1.x - vec2.x;
	double dy = vec1.y - vec2.y;
	return std::sqrt(dx * dx + dy * dy);
}

int data::generateNumber(int min, int max) {

	boost::random::uniform_real_distribution<> dist(min, max);

	return dist(rng);
}

bool data::frame_collision(Vector2<double> pos, Vector2<int> size_map) {

	int half_x = size_map.x / 2;
	int half_y = size_map.y / 2;

	return  (-half_x < pos.x && pos.x < half_x) && (-half_y < pos.y && pos.y < half_y);
}

std::tuple<int, int, int> data::randomColor() {

	boost::random::uniform_real_distribution<> dist(0, 255);
	return std::tuple<int, int, int>(dist(rng), dist(rng), dist(rng));
} 