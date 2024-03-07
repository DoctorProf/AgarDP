#include <SFML/Graphics.hpp>
#include <iostream>
#include "Headers/Player.hpp"
#include "Headers/Food.hpp"
#include <SFML/Network.hpp>
#include <fstream>


using namespace sf;


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

		std::cout << "No data to server" << std::endl;
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

void sendToServer(TcpSocket* socket, Vector2<double>& last_mouse_pos) {

	Packet packet_mouse_pos;

	packet_mouse_pos << last_mouse_pos.x << last_mouse_pos.y;
	socket->send(packet_mouse_pos);
}

void render(Player*& player, std::vector<Player*>& players, std::vector<Food*>& foods, RenderWindow& window, View& world, View& gui, Vector2f& size, VertexArray& grid, Text& score) {

	window.clear(Color::White);

	world.setSize(size);
	world.setCenter(Vector2f(player->getPosition()));

	window.setView(world);

	window.draw(grid);

	for (Food* food : foods) {

		food->draw(window);
	}

	player->draw(window);

	for (Player* player : players) {

		player->draw(window);
	}

	window.setView(gui);
	window.draw(score);
	window.display();
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

		foods.push_back(new Food(id, Vector2<double>(x, y)));
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
int main() {

	TcpSocket* socket = new TcpSocket;

	std::vector<Food*> foods;
	std::vector<Player*> players;

	Vector2<int> size_map = { 7680, 4320 };

	std::ifstream config("config.txt");

	std::string ip;

	std::getline(config, ip);

	if (socket->connect(ip, 5000) != sf::Socket::Done) {

		return -1;
	}

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
		}

		window.setView(world);

		last_mouse_pos = Vector2<double>(window.mapPixelToCoords(Mouse::getPosition(window)));

		sendToServer(socket, last_mouse_pos);

		updatePositionFood(socket, foods);

		generatePlayers(socket, players);

		if (getFromServer(socket, player, zoom)) return 0;

		Vector2f size = Vector2f(window.getSize().x, window.getSize().y) / (float)zoom;

		score.setString("Score - " + std::to_string((int)player->getMass()));

		render(player, players, foods, window, world, gui, size, grid, score);
	}
}