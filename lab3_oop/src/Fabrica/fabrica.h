#ifndef NEWLAB3_FABRICA_PLAYER_H
#define NEWLAB3_FABRICA_PLAYER_H

#include "player.h"
#include "memory.h"
#include "../cmdparser.h"

class PlayerFabrica {
public:
    static std::shared_ptr<Player> createNewPlayer(const PlayerType &id = PlayerType::RANDOM);
};


#endif //NEWLAB3_FABRICA_PLAYER_H
