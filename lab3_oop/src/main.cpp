#include "gameview.h"

int main(int argc, char **argv) {
    // For Randomer
    srand(time(nullptr));

    int roundsAmount;
    PlayerType player1_type, player2_type;
    if (Parser::parseArguments(argc, argv, player1_type, player2_type, roundsAmount))
        return 1;

    auto Player1 = PlayerFabrica::createNewPlayer(player1_type);
    auto Player2 = PlayerFabrica::createNewPlayer(player2_type);
    Gameview::startGame(Player1, Player2, roundsAmount);

    return 0;
}

