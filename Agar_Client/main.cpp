#include <SFML/Graphics.hpp>
#include <iostream>
#include "Headers/Player.hpp"
#include "Headers/Food.hpp"
#include <SFML/Network.hpp>
#include <fstream>
#include <SFML/GpuPreference.hpp>
#include <nlohmann/json.hpp>

#define SFML_DEFINE_DISCRETE_GPU_PREFERENCE

using namespace sf;
using json = nlohmann::json;

VertexArray createGrid(Vector2<int>& size_map)
{
	VertexArray grid;
	grid.setPrimitiveType(Lines);

	int map_half_x = size_map.x / 2;
	int map_half_y = size_map.y / 2;

	for (int i = -map_half_x; i <= map_half_x; i += 40)
	{
		Vertex point;

		point.color = Color::Color(40, 40, 40);
		point.position = Vector2f(i, -map_half_y);

		grid.append(point);

		point.position = Vector2f(i, map_half_y);

		grid.append(point);
	}
	for (int i = -map_half_y; i <= map_half_y; i += 40)
	{
		Vertex point;

		point.color = Color::Color(40, 40, 40);
		point.position = Vector2f(-map_half_x, i);

		grid.append(point);

		point.position = Vector2f(map_half_x, i);

		grid.append(point);
	}
	return grid;

}
bool getFromServer(TcpSocket* socket, Player* player, double& zoom) {

	Packet packet_player;

	if (socket->receive(packet_player) != Socket::Done) {

		std::cerr << "No data to server" << std::endl;
		return 1;
	}

	double radius;
	double mass;
	Vector2<double> position;

	int r;
	int g;
	int b;

	packet_player >> radius >> mass >> position.x >> position.y >> zoom >> r >> g >> b;

	player->setRadius(radius);
	player->setMass(mass);
	player->setPosition(position);
	player->setColor(r, g, b);

	return 0;
}

void sendToServer(TcpSocket* socket, Vector2<double>& last_mouse_pos, bool& strike, bool& segmentation) {

	Packet packet_mouse_pos;
	packet_mouse_pos << last_mouse_pos.x << last_mouse_pos.y << strike << segmentation;

	socket->send(packet_mouse_pos);
	strike = false;
	segmentation = false;
}


void getPositionFood(TcpSocket* socket, std::vector<Food*>& foods) {

	Packet packet_food;

	size_t count_food;

	socket->receive(packet_food);

	packet_food >> count_food;

	for (int i = 0; i < count_food; i++) {

		int id;
		double x;
		double y;

		packet_food >> id >> x >> y;

		foods.push_back(new Food(id, 5, Vector2<double>(x, y)));
	}
}

void updatePositionFood(TcpSocket* socket, std::vector<Food*>& foods) {

	Packet packet_food_update;

	size_t size_update{};

	socket->receive(packet_food_update);

	packet_food_update >> size_update;

	for (int i = 0; i < size_update; i++) {

		int id;
		double x;
		double y;

		packet_food_update >> id >> x >> y;

		auto food = std::find_if(foods.begin(), foods.end(), [&](const auto& it) {
			return id == it->getId();
			});

		(*food)->setPosition(Vector2<double>(x, y));
	}
}

void generatePlayers(TcpSocket* socket, std::vector<Player*>& players) {

	players.clear();

	Packet packet_player;

	size_t count_players{};

	socket->receive(packet_player);

	packet_player >> count_players;

	for (int i = 0; i < count_players; i++) {

		double radius;

		Vector2<double> position;

		int r;
		int g;
		int b;

		packet_player >> radius >> position.x >> position.y >> r >> g >> b;

		Player* player = new Player;

		player->setRadius(radius);
		player->setPosition(position);
		player->setColor(r, g, b);

		players.push_back(player);
	}
}

void generateFoodPlayers(TcpSocket* socket, std::vector<Food*>& food_players) {

	food_players.clear();

	Packet packet_food_players;

	size_t count_food_players{};

	socket->receive(packet_food_players);

	packet_food_players >> count_food_players;

	for (int i = 0; i < count_food_players; i++) {

		int id;
		double x;
		double y;

		int r;
		int g;
		int b;

		packet_food_players >> id >> x >> y >> r >> g >> b;

		Food* food = new Food(id, 10, Vector2<double>(x, y));
		food->setColor(r, g, b);

		food_players.push_back(food);
	}
}

void render(Player*& player, std::vector<Player*>& players, std::vector<Food*>& foods, std::vector<Food*> food_players, RenderWindow& window, View& world, View& gui, Vector2f& size, VertexArray& grid, Text& score) {

	window.clear(Color::White);

	world.setSize(size);
	world.setCenter(Vector2f(player->getPosition()));

	window.setView(world);

	window.draw(grid);

	for (Food* food : foods) {

		food->draw(window);
	}

	for (Food* food : food_players) {

		food->draw(window);
	}

	for (Player* player : players) {

		player->draw(window);
	}

	player->draw(window);

	window.setView(gui);
	window.draw(score);
	window.display();
}

int main() {

	std::ifstream file_config("config_client.json");

	json config = json::parse(file_config);

	TcpSocket* socket = new TcpSocket;

	std::vector<Food*> foods;
	std::vector<Food*> food_players;
	std::vector<Player*> players;

	bool strike = false;
	bool segmentation = false;

	Vector2<int> size_map{};

	Vector2f size{};
	
	std::string ip = config["ip"];
	int port = config["port"];

	if (socket->connect(ip, port) != sf::Socket::Done) {

		return -1;
	}

	Packet packet_size_map;

	socket->receive(packet_size_map);

	packet_size_map >> size_map.x >> size_map.y;

	getPositionFood(socket, foods);

	data::loadFont();

	ContextSettings settings;
	settings.antialiasingLevel = 8;

	RenderWindow window(VideoMode(1920, 1080), "Agario", Style::Fullscreen, settings);
	window.setVerticalSyncEnabled(true);

	View world;

	View gui;
	gui.setSize(window.getSize().x, window.getSize().y);
	gui.setCenter(window.getSize().x / 2, window.getSize().y / 2);

	VertexArray grid;

	grid = createGrid(size_map);

	Vector2<double> last_mouse_pos;

	Player* player = new Player();
	double zoom = 1;

	Text score;
	score.setFillColor(Color::Black);
	score.setFont(data::font);
	score.setPosition(Vector2f(10, 1030));
	score.setCharacterSize(32);

	while (window.isOpen()) {

		Event event;
		while (window.pollEvent(event)) {

			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape || event.type == Event::Closed) {

				window.close();
				socket->disconnect();
			}
			if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {

				strike = true;
			}
			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space) {

				segmentation = true;
			}
		}

		window.setView(world);

		last_mouse_pos = Vector2<double>(window.mapPixelToCoords(Mouse::getPosition(window)));

		sendToServer(socket, last_mouse_pos, strike, segmentation);

		updatePositionFood(socket, foods);

		generatePlayers(socket, players);
		generateFoodPlayers(socket, food_players);

		if (getFromServer(socket, player, zoom)) return 0;

		size = Vector2f(window.getSize().x, window.getSize().y) / (float)zoom;

		score.setString("Score - " + std::to_string((int)player->getMass()));

		render(player, players, foods, food_players, window, world, gui, size, grid, score);
	}
}