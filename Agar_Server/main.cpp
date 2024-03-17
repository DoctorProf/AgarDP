#include <iostream>
#include "Headers/Player.hpp"
#include "Headers/Game.hpp"
#include <thread>
#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

using namespace sf;

int main() {

    std::ifstream file_config("config_server.json");

    json config = json::parse(file_config);

    std::vector<Player*> players;

    std::vector<Food*> foods;

    int count_food = config["count_food"];
    bool running = true;

    Game game(players, foods, count_food);

    TcpListener listener;

    if (listener.listen(config["port"]) != Socket::Done) {

        std::cout << "Error" << std::endl;
        return 1;
    }

    SocketSelector selector;
    selector.add(listener);

    std::cout << "Server Start" << std::endl;

    while (running) {

        if (selector.wait(seconds(0.0001))) {

            if (selector.isReady(listener)) {

                TcpSocket* new_connection = new TcpSocket;

                if (listener.accept(*new_connection) == sf::Socket::Done) {

                    Player* player = new Player(new_connection, Vector2<double>(data::generateNumber(-3840, 3840), data::generateNumber(-2160, 2160)));
                    players.push_back(player);
                    std::cout << " Players - " << players.size() << "\n";

                    std::thread send_position_food(&Game::sendPositionFood, &game, std::ref(player));
                    send_position_food.detach();
                }
            }
        }

        game.getFromPlayer();

        game.movePlayers();
        game.collisionFood();
        game.collisionPlayers();

        game.updateFood();
        game.updatePlayers();

        game.sendToPlayer();
    }

    return 0;
}