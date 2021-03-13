#include "fabrica.h"
#include <iostream>

std::shared_ptr<Player> PlayerFabrica::createNewPlayer(const PlayerType &id) {
    switch (id) {
        case PlayerType::CONSOLE:
            return std::shared_ptr<Player>(new ConsolePlayer());
        case PlayerType::RANDOM:
            return std::shared_ptr<Player>(new RandomPlayer());
        case PlayerType::OPTIMAL:
            return std::shared_ptr<Player>(new OptimalPlayer());
        default:
            throw std::logic_error("New player creation error! Unknown player type!");
    }
}

