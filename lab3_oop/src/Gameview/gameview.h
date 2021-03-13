#ifndef NEWLAB3_GAMEVIEW_H
#define NEWLAB3_GAMEVIEW_H

#include "player.h"
#include "fabrica.h"

class StringParser {
public:
    static bool isNumber(const std::string &s);
};

class Randomer {
public:
    static int *generateRandomSequence(const unsigned int &sequenceLength);
};

class Gameview {
public:
    static void printGameRules();
    static void startGame(const std::shared_ptr<Player> &Player1, const std::shared_ptr<Player> &Player2, const int &roundsAmount);
    static void startRound(std::shared_ptr<Player> player1, std::shared_ptr<Player> player2, const int &numRound, int &winnedRoundsPlayer1, int &winnedRoundsPlayer2);
    static GameStatus playerTurn(std::shared_ptr<Player> attacker, std::shared_ptr<Player> enemyPlayer);
    static void endRound(std::shared_ptr<Player> player1, std::shared_ptr<Player> player2, const int &numRound, int &winnedRoundsPlayer1, int &winnedRoundsPlayer2);
    static void endGame(int &winnedRoundsPlayer1, int &winnedRoundsPlayer2);
};

#endif //NEWLAB3_GAMEVIEW_H
