#include <iostream>
#include "Headers/Player.hpp"
#include "Headers/Game.hpp"
#include <thread>
using namespace sf;

int main() {

    std::vector<Player*> players;

    std::vector<Food*> foods;

    std::vector<std::thread*> events;

    int count_food = 10000;
    bool running = true;

    Game game(players, foods, count_food);

    TcpListener listener;

    if (listener.listen(5000) != Socket::Done) {

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

                    Player* player = new Player(new_connection, Vector2<double>(data::generateNumber(0, 1920), data::generateNumber(0, 1080)));
                    players.push_back(player);
                    std::cout << " Players - " << players.size() << "\n";
                    std::thread* new_player_thread = new std::thread(&Game::checkEventShot, &game, std::ref(player));
                    events.push_back(new_player_thread);
                    game.sendPositionFood(player);
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

    for (std::thread* thread : events) {

        thread->join();
    }

    return 0;
}