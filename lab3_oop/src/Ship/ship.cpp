#include "ship.h"

//! ShipBorder !//

ShipBorder::ShipBorder(const FieldCell &ltCorner_, const FieldCell &rbCorner_) : ltCorner(ltCorner_), rbCorner(rbCorner_) {

}

FieldCell ShipBorder::getTopCorner() const {
    return ltCorner;
}

FieldCell ShipBorder::getBottomCorner() const {
    return rbCorner;
}

ShipBorder &ShipBorder::operator=(const ShipBorder &border) {
    ltCorner = border.ltCorner;
    rbCorner = border.rbCorner;
    return *this;
}

bool ShipBorder::isBordersIntersect(const ShipBorder &border) const {
    FieldCell rb = border.getBottomCorner();
    FieldCell lt = border.getTopCorner();

    int min_x1 = ltCorner.x();
    int min_y1 = rbCorner.y();
    int max_x1 = rbCorner.x();
    int max_y1 = ltCorner.y();

    int min_x2 = lt.x();
    int min_y2 = rb.y();
    int max_x2 = rb.x();
    int max_y2 = lt.y();

    return !((max_x1 < min_x2) || (min_x1 > max_x2) || (min_y1 > max_y2) || (max_y1 < min_y2));
}

ShipBorder Ship::createShipBorder(const int &beginX, const int &beginY, const ShipDirection &direction) const {
    switch (direction) {
        case ShipDirection::RIGHT: {
            return ShipBorder(FieldCell(beginX - 1, beginY + 1),
                              FieldCell(beginX + static_cast<int>(length), beginY - 1));
        }
        case ShipDirection::LEFT: {
            return ShipBorder(FieldCell(beginX - static_cast<int>(length), beginY + 1),
                              FieldCell(beginX + 1, beginY - 1));
        }
        case ShipDirection::UP: {
            return ShipBorder(FieldCell(beginX - 1, beginY + 1),
                              FieldCell(beginX + 1, beginY - static_cast<int>(length)));
        }
        case ShipDirection::DOWN: {
            return ShipBorder(FieldCell(beginX - 1, beginY + static_cast<int>(length)),
                              FieldCell(beginX + 1, beginY - 1));
        }
        case ShipDirection::NO_DIRECTION: {
            return ShipBorder(FieldCell(beginX - 1, beginY + 1), FieldCell(beginX + 1, beginY - 1));
        }
        default:
            throw std::logic_error("Error of creating ship border! Unknown ship direction!");
    }
}

//! Ship !//

Ship::Ship(const ShipID &id, const int &beginX, const int &beginY, const ShipDirection &direction) {
    shipType = id;
    length = getShipLength(id);
    fillShipCells(beginX, beginY, direction);
    border_ = createShipBorder(beginX, beginY, direction);
}

void Ship::fillShipCells(const int &beginX, const int &beginY, const ShipDirection &direction) {
    switch (direction) {
        case ShipDirection::LEFT: {
            for (int x = 0; x < length; ++x)
                shipCells.emplace_back(FieldCell(beginX - x, beginY));
            break;
        }
        case ShipDirection::RIGHT: {
            for (int x = 0; x < length; ++x)
                shipCells.emplace_back(FieldCell(beginX + x, beginY));
            break;
        }
        case ShipDirection::UP: {
            for (int y = 0; y < length; ++y)
                shipCells.emplace_back(FieldCell(beginX, beginY - y));
            break;
        }
        case ShipDirection::DOWN: {
            for (int y = 0; y < length; ++y)
                shipCells.emplace_back(FieldCell(beginX, beginY + y));
            break;
        }
        case ShipDirection::NO_DIRECTION: {
            shipCells.emplace_back(FieldCell(beginX, beginY));
            break;
        }
        case ShipDirection::UNKNOWN:
            throw std::logic_error("Error of creating Battleship object! Unknown ship direction!");
    }
}

FieldCell Ship::operator[](const int &index) const {
    if (!(index >= 0 && index < length)) {
        throw std::logic_error("Segmentation error! Index of ship cells array out of range!");
    }
    return shipCells[index];
}

bool Ship::isShipCell(const FieldCell &cell) const {
    for (unsigned int index = 0; index < length; ++index) {
        if (cell == shipCells[index])
            return true;
    }
    return false;
}

ShipBorder Ship::border() const {
    return border_;
}

unsigned int Ship::size() const {
    return length;
}

unsigned int Ship::getShipLength(const ShipID &id) const {
    switch (id) {
        case ShipID::BATTLESHIP:
            return 4;
        case ShipID::CRUISER:
            return 3;
        case ShipID::DESTROYER:
            return 2;
        case ShipID::CARRIER:
            return 1;
        default:
            throw std::logic_error("Error get type ship size! Unknown ship type!");
    }
}

ShipID Ship::type() const {
    return shipType;
}

ShipBorder Ship::getShipBorder() const {
    FieldCell result = shipCells[length - 1] - shipCells[0];
    int x = result.x();
    int y = result.y();

    if ((y == 0 && x > 0) || (x == 0 && y < 0)) {
        return ShipBorder(shipCells[0], shipCells[length - 1]);
    } else if ((y == 0 && x < 0) || (x == 0 && y > 0)) {
        return ShipBorder(shipCells[length - 1], shipCells[0]);
    }

    return ShipBorder(shipCells[0], shipCells[0]);
}

Ship::Ship(const ShipID &id, const FieldCell &beginCoords, const ShipDirection &direction) {
    int beginX = beginCoords.x();
    int beginY = beginCoords.y();
    shipType = id;
    initDirection_ = direction;
    length = getShipLength(id);
    fillShipCells(beginX, beginY, direction);
    border_ = createShipBorder(beginX, beginY, direction);
}

Ship::Ship() {
    shipType = ShipID::UNKNOWN;
    length = 0;
    initDirection_ = ShipDirection::UNKNOWN;
    border_ = createShipBorder(-1, -1, ShipDirection::NO_DIRECTION);
}

Ship & Ship::operator=(const Ship &ship) {
    if (this != &ship) {
        initDirection_ = ship.initDirection();
        shipType = ship.type();
        length = ship.size();
        shipCells = ship.shipCells;
        damagedCells = ship.damagedCells;
        border_ = ship.border_;
    }
    return *this;
}

ShipDirection Ship::initDirection() const {
    return initDirection_;
}

bool Ship::isDefeatShip() const {
    return (damagedCells.size() == length);
}

void Ship::damage(const FieldCell &cell) {
    damagedCells.emplace_back(cell);
}

bool Ship::operator==(const Ship &ship) {
    if (length != ship.size() || shipType != ship.type())
        return false;

    for (int i = 0; i < length; ++i) {
        if (!(shipCells[i] == ship[i]))
            return false;
    }
    return true;
}

Ship::Ship(const Ship &ship) {
    initDirection_ = ship.initDirection();
    shipType = ship.type();
    length = ship.size();
    shipCells = ship.shipCells;
    damagedCells = ship.damagedCells;
    border_ = ship.border_;
}

Ship &Ship::operator=(Ship &&ship) {
    if (this != &ship) {
        initDirection_ = ship.initDirection();
        shipType = ship.type();
        length = ship.size();
        shipCells = ship.shipCells;
        damagedCells = ship.damagedCells;
        border_ = ship.border_;
    }
    return *this;
}
