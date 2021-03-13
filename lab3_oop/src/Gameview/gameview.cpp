#include "gameview.h"
#include <iostream>
#include <algorithm>

void Gameview::startGame(const std::shared_ptr<Player> &Player1, const std::shared_ptr<Player> &Player2, const int &roundsAmount) {
    int winnedRoundsPlayer1 = 0, winnedRoundsPlayer2 = 0;
    for (int round = 0; round < roundsAmount; ++round) {
        Gameview::startRound(Player1, Player2, round, winnedRoundsPlayer1, winnedRoundsPlayer2);
    }

    endGame(winnedRoundsPlayer1, winnedRoundsPlayer2);
    system("pause");
}

void Gameview::startRound(std::shared_ptr<Player> player1, std::shared_ptr<Player> player2, const int &numRound, int &winnedRoundsPlayer1, int &winnedRoundsPlayer2) {
    // Clear all placed ships and reset self and enemy field
    player1->reset();
    player2->reset();

    // Place ships on field
    player1->placeShips();
    player2->placeShips();

    std::cout << "Starting round #" << numRound + 1 << std::endl << std::endl;
    GameStatus gameStatus = GameStatus::NEXT_PLAYER_TURN;

    do {
        //std::cout << "Player 1 turn:" << std::endl << std::endl;
        gameStatus= Gameview::playerTurn(player1, player2);
        if (gameStatus == GameStatus::END_ROUND)
            break;

        //std::cout << "Player 2 turn:" << std::endl << std::endl;
        gameStatus= Gameview::playerTurn(player2, player1);
    } while (gameStatus != GameStatus::END_ROUND);

    endRound(player1, player2, numRound, winnedRoundsPlayer1, winnedRoundsPlayer2);
}

GameStatus Gameview::playerTurn(std::shared_ptr<Player> attacker, std::shared_ptr<Player> enemyPlayer) {
    GameStatus gameStatus = GameStatus::CONTINUE;
    do {
        gameStatus = attacker->doStep(enemyPlayer);
    } while (gameStatus == GameStatus::CONTINUE);

    if (enemyPlayer->shipsLeft() == 0)
        return GameStatus::END_ROUND;

    return GameStatus::NEXT_PLAYER_TURN;
}

void Gameview::endRound(std::shared_ptr<Player> player1, std::shared_ptr<Player> player2, const int &numRound, int &winnedRoundsPlayer1, int &winnedRoundsPlayer2) {
    std::cout << "Player 1: " << std::endl << std::endl;
    player1->displayFields();
    std::cout << "Player 2: " << std::endl << std::endl;
    player2->displayFields();

    if (player2->shipsLeft() == 0) {
        std::cout << "Player 1 has wun round #" << numRound + 1 << "!" << std::endl;
        ++winnedRoundsPlayer1;
    } else if (player1->shipsLeft() == 0) {
        std::cout << "Player 2 has wun round #" << numRound + 1 << "!" << std::endl;
        ++winnedRoundsPlayer2;
    } else {
        throw std::logic_error("Error! Called end round func without winner!");
    }
}

void Gameview::endGame(int &winnedRoundsPlayer1, int &winnedRoundsPlayer2) {
    std::cout << std::endl;
    std::cout << "\t*-------------------------*" << std::endl;
    std::cout << "\tGame has been end" << std::endl;
    std::cout << "\tPlayer 1 score: " << winnedRoundsPlayer1 << std::endl;
    std::cout << "\tPlayer 2 score: " << winnedRoundsPlayer2 << std::endl;

    if (winnedRoundsPlayer1 > winnedRoundsPlayer2) {
        std::cout << "\tPlayer 1 has wun the game!" << std::endl;
    } else if (winnedRoundsPlayer2 > winnedRoundsPlayer1) {
        std::cout << "\tPlayer 2 has wun the game!" << std::endl;
    } else {
        std::cout << "\tTie. Friendship won" << std::endl;
    }
    std::cout << "\t*-------------------------*" << std::endl;
}

void Gameview::printGameRules() {
    std::cout << std::endl<< "\t---The battleship game rules---" << std::endl << std::endl;
    std::cout << "#1. Players should place their ships on self field (left field). The results of enemy shelling are marked on the enemy field (right field)" << std::endl;
    std::cout << "#2. All players have four different ships for placement:" << std::endl
        << "\t- Battleship 4x1 [1 ship]" << std::endl
        << "\t- Cruiser 3x1 [2 ships]" << std::endl
        << "\t- Destroyer 2x1 [3 ships]" << std::endl
        << "\t- Carrier 1x1 [4 ships]" << std::endl;
    std::cout << std::endl;

    std::cout << "#3. Before placement ship, console gamer should enter X, Y and ship direction [left/right/up/down/no]" << std::endl;
    std::cout << "P.S. no ~ no direction for carrier, but doesn't matter for it" << std::endl;
    std::cout << "Examples:" << std::endl << "\t- 2 3 d" << std::endl << "\t- 1 1 right" << std::endl << "\t- 1 1 R";
    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << "#4. The firing point is set along the coordinates of the axes. The top axis is the X-axis, the left axis is the Y-axis" << std::endl;
    std::cout << "#5. All enemy ships must be sunk to win the round" << std::endl;
    std::cout << "#6. To win the game, you need to win more rounds than your opponent" << std::endl;
    std::cout << std::endl;

    std::cout << "\t--- How you can see, rules are simple. GLHF, commander! ---" << std::endl;
}

bool StringParser::isNumber(const std::string &s) {
    return !s.empty() && std::find_if(s.begin(), s.end(), [](unsigned char c) { return !isdigit(c); }) == s.end();
}

int *Randomer::generateRandomSequence(const unsigned int &sequenceLength) {
    int *randomSequence = new int[sequenceLength];

    int *subArray = new int[sequenceLength - 1];
    for (int i = 1; i < sequenceLength; ++i)
        subArray[i - 1] = i;
    int subArrBeginIndex = 0;
    int elementsLeft = static_cast<int>(sequenceLength) - 1;
    int index = 0;
    int subIndex, next;

    while (elementsLeft) {
        subIndex = subArrBeginIndex + (rand() % elementsLeft);
        next = subArray[subIndex];
        randomSequence[index] = next;
        index = next;
        std::swap(subArray[subIndex], subArray[subArrBeginIndex]);
        ++subArrBeginIndex;
        --elementsLeft;
    }

    randomSequence[index] = 0;
    delete[] subArray;

    return randomSequence;
}

