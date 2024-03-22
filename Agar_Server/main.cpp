#include <iostream>
#include "Headers/Player.hpp"
#include "Headers/Game.hpp"
#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

using namespace sf;

int main() {

    std::ifstream file_config("config_server.json");

    json config = json::parse(file_config);

    int count_food = config["count_food"];
    bool running = true;

    Vector2<int> size_map = Vector2<int>(config["size_map"][0], config["size_map"][1]);

    Game game(count_food, size_map);

    TcpListener listener;

    if (listener.listen(config["port"]) != Socket::Done) {

        std::cout << "Error" << std::endl;
        return 1;
    }

    SocketSelector selector;
    selector.add(listener);

    std::cout << "Server Start" << std::endl;

    while (running) {

        if (selector.wait(seconds(0.001))) {

            if (selector.isReady(listener)) {

                TcpSocket* socket = new TcpSocket();

                if (listener.accept(*socket) == sf::Socket::Done) {

                    game.connectPlayer(socket);
                }
            }
        }
        game.getFromPlayer();

        game.movePlayer();
        game.moveFoodPlayers();

        game.collisionFood();
        game.collisionPlayers();

        game.updateFood();

        game.updatePlayers();

        game.sendToPlayer();
    }

    return 0;
}