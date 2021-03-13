#ifndef NEWLAB3_PLAYER_H
#define NEWLAB3_PLAYER_H

#include <ctime> // for rand()
#include <memory> // for std::shared_ptr
#include <string>
#include <vector>
#include <iostream>

#include "battlefield.h"
#include "ship.h"

enum class FieldEdge {
    LEFT, RIGHT, TOP, BOTTOM
};

enum class FieldCorner {
    LEFT_TOP, RIGHT_TOP, LEFT_BOTTOM, RIGHT_BOTTOM
};

enum class StepStatus {
    HIT, DEFEAT, MISS
};

enum class GameStatus {
    START_GAME, START_ROUND, CONTINUE, NEXT_PLAYER_TURN, NEXT_ROUND, END_ROUND, END_GAME
};

class Player {
public:
    virtual void reset() = 0;
    virtual void placeShips() = 0;
    virtual GameStatus doStep(std::shared_ptr<Player> enemy) = 0;
    virtual unsigned int shipsLeft() = 0;
    virtual void displayFields() = 0;
    virtual void markAsMiss(const FieldCell &cell) = 0;
    virtual void markAsHit(const FieldCell &cell) = 0;
    virtual void markAsDefeatedEnemyShip(const Ship &defeatedShip) = 0;
    virtual StepStatus takeAttack(Player* shooter, const FieldCell &cell) = 0;
};

class ConsolePlayer : public Player {
private:
    Battlefield selfField;
    Battlefield enemyField;
    std::vector<Ship> ships;

    bool isRightCoords(const int &x, const int &y, const ShipDirection &direction) const;
    bool canPlaceShip(Ship& ship) const;
    ShipDirection readShipDirection() const;
    std::vector<std::string> readCoordsFromConsole() const;
    bool readCoords(int &x, int &y) const;
    void getCoords(int &x, int &y, ShipDirection &direction) const;
    void placeNewShip(Ship &newShip);
    void placeShip(const ShipID &shipID, const unsigned int &shipsAmount, const unsigned int &size);

public:
    void reset() override;
    void placeShips() override;
    GameStatus doStep(std::shared_ptr<Player> enemy) override;
    unsigned int shipsLeft() override;
    void displayFields() override;
    void markAsMiss(const FieldCell &cell) override;
    void markAsHit(const FieldCell &cell) override;
    void markAsDefeatedEnemyShip(const Ship &defeatedShip) override;
    StepStatus takeAttack(Player* shooter, const FieldCell &cell) override;
};

class RandomPlayer : public Player {
private:
    Battlefield selfField;
    Battlefield enemyField;
    std::vector<Ship> ships;

    ShipDirection getRandomDirection(const int &num) const;
    bool canPlaceShip(Ship& ship) const;
    void placeNewShip(Ship &newShip);
    void randomPlaceShip(const ShipID &id, const unsigned int &count, const int* randomTraversing, int &beginIndex);
    FieldCell getRandomCoords();
public:
    void reset() override;
    void placeShips() override;
    GameStatus doStep(std::shared_ptr<Player> enemy) override;
    unsigned int shipsLeft() override;
    void displayFields() override;
    void markAsMiss(const FieldCell &cell) override;
    void markAsHit(const FieldCell &cell) override;
    void markAsDefeatedEnemyShip(const Ship &defeatedShip) override;
    StepStatus takeAttack(Player* shooter, const FieldCell &cell) override;
};

class OptimalPlayer : public Player {
private:
    Battlefield selfField;
    Battlefield enemyField;
    std::vector<Ship> ships;
    bool finishingMode = false;
    FieldCell lastHittedCell = FieldCell(-1, -1);

    bool canPlaceShip(Ship &ship) const;
    void placeNewShip(Ship &newShip);
    void placeShipInCorner(const ShipID &shipID, const FieldCorner &corner);
    FieldCorner getRandomFieldCorner(const int &num) const;
    FieldEdge getRandomFieldEdge(const int &num) const;
    bool tryPlaceOnEdge(const unsigned int &shipSize, const FieldEdge &edge);

    void optimalPlaceBattleship(const int* randomTraversing, int &beginIndex);
    void optimalPlaceCruiser(const unsigned int &count, const int* randomTraversing, int &beginIndex);
    void optimalPlaceDestroyer(const unsigned int &count, const int* randomTraversing, int &beginIndex);
    void optimalPlaceCarrier(const unsigned int &count);

    ShipDirection findDirection(const std::shared_ptr<Player>& enemy);
    ShipDirection changeToOppositeDirection(const ShipDirection &direction);
    FieldCell getShift(const ShipDirection &direction) const;
    StepStatus shootLine(const std::shared_ptr<Player>& enemy, const ShipDirection &direction);
    StepStatus tryFinishShip(const std::shared_ptr<Player>& enemy);
    FieldCell getNextCoords();
    void markUselessCells(const ShipBorder &defeatedShipBorder);
public:
    void reset() override;
    void placeShips() override;
    GameStatus doStep(std::shared_ptr<Player> enemy) override;
    unsigned int shipsLeft() override;
    void displayFields() override;
    void markAsMiss(const FieldCell &cell) override;
    void markAsHit(const FieldCell &cell) override;
    void markAsDefeatedEnemyShip(const Ship &defeatedShip) override;
    StepStatus takeAttack(Player* shooter, const FieldCell &cell) override;
};

#endif //NEWLAB3_PLAYER_H
