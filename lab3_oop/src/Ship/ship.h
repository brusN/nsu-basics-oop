#ifndef NEWLAB3_SHIP_H
#define NEWLAB3_SHIP_H

#include "battlefield.h"
#include <vector>
#include <iostream>

static const unsigned int battleshipAmount = 1;
static const unsigned int cruiserAmount = 2;
static const unsigned int destroyerAmount = 3;
static const unsigned int carrierAmount = 4;

static const unsigned int battleshipSize = 4;
static const unsigned int cruiserSize = 3;
static const unsigned int destroyerSize = 2;
static const unsigned int carrierSize = 1;

enum class ShipID {
    BATTLESHIP, CRUISER, DESTROYER, CARRIER, UNKNOWN
};

enum class ShipDirection {
    LEFT, RIGHT, UP, DOWN, NO_DIRECTION, UNKNOWN
};

class ShipBorder {
private:
    FieldCell ltCorner; // left top corner
    FieldCell rbCorner; // right bottom corner

public:
    ShipBorder(const FieldCell &ltCorner_ = FieldCell(0, 0), const FieldCell &rbCorner_ = FieldCell(0, 0));
    FieldCell getTopCorner() const;
    FieldCell getBottomCorner() const;
    ShipBorder & operator=(const ShipBorder &border);
    bool isBordersIntersect(const ShipBorder &border) const;
};

class Ship {
private:
    ShipID shipType;
    unsigned int length;
    std::vector<FieldCell> shipCells;
    std::vector<FieldCell> damagedCells;
    ShipBorder border_;
    ShipDirection initDirection_;

    unsigned int getShipLength(const ShipID &id) const;
    ShipBorder createShipBorder(const int &beginX, const int &beginY, const ShipDirection &direction) const;
    void fillShipCells(const int &beginX, const int &beginY, const ShipDirection &direction);
public:
    Ship();
    Ship(const ShipID &id, const int &beginX, const int &beginY, const ShipDirection &direction);
    Ship(const ShipID &id, const FieldCell &beginCoords, const ShipDirection &direction);
    Ship(const Ship &ship);

    unsigned int size() const;
    ShipID type() const;
    ShipBorder border() const;
    ShipBorder getShipBorder() const;
    ShipDirection initDirection() const;

    FieldCell operator[](const int &index) const;
    Ship &operator=(const Ship &ship);
    Ship &operator=(Ship &&ship);
    bool operator==(const Ship &ship);

    bool isShipCell(const FieldCell &cell) const;
    void damage(const FieldCell &cell);
    bool isDefeatShip() const;
};

#endif //NEWLAB3_SHIP_H
