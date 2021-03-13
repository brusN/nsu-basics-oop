#include "ship.h"
#include "player.h"
#include "gameview.h"

void OptimalPlayer::placeShips() {
    const unsigned int fieldSize = selfField.size();
    int beginIndex = 0;
    int* randomTraversing = Randomer::generateRandomSequence(4);

    optimalPlaceBattleship(randomTraversing, beginIndex);
    optimalPlaceCruiser(cruiserAmount, randomTraversing, beginIndex);
    optimalPlaceDestroyer(destroyerAmount, randomTraversing, beginIndex);
    optimalPlaceCarrier(carrierAmount);

    delete [] randomTraversing;
}

GameStatus OptimalPlayer::doStep(std::shared_ptr<Player> enemy) {
    StepStatus status;
    FieldCell nextCoords(-1, -1);
    if (!finishingMode) {
        nextCoords = getNextCoords();
        status = enemy->takeAttack(this, nextCoords);
        //displayFields();
    } else {
        status = tryFinishShip(enemy);
    }

    if (enemy->shipsLeft() == 0)
        return GameStatus::END_ROUND;

    if (status == StepStatus::HIT) {
        lastHittedCell = nextCoords;
        finishingMode = true;
        return GameStatus::CONTINUE;
    } else if (status == StepStatus::DEFEAT) {
        finishingMode = false;
        return GameStatus::CONTINUE;
    }

    return GameStatus::NEXT_PLAYER_TURN;
}

void OptimalPlayer::reset() {
    selfField.reset();
    enemyField.reset();
    ships.clear();
    finishingMode = false;
    lastHittedCell = FieldCell(-1, -1);
}

unsigned int OptimalPlayer::shipsLeft() {
    return ships.size();
}

void OptimalPlayer::displayFields() {
    const unsigned int fieldSize = selfField.size();

    // Prints X-axis self field
    std::cout << ' ';
    for (int x = 0; x < fieldSize; ++x)
        std::cout << ' ' << x;
    std::cout << "  |  ";

    // Prints X-axis enemy field
    std::cout << ' ';
    for (int x = 0; x < fieldSize; ++x)
        std::cout << ' ' << x;
    std::cout << std::endl;

    for (int y = 0; y < fieldSize; ++y) {
        // Prints y-axis self field
        std::cout << y;
        for (int x = 0; x < fieldSize; ++x)
            std::cout << ' ' << static_cast<char>(selfField(x, y));
        std::cout << "  |  ";

        // Prints y-axis enemy field
        std::cout << y;
        for (int x = 0; x < fieldSize; ++x)
            std::cout << ' ' << static_cast<char>(enemyField(x, y));

        std::cout << std::endl;
    }
    // One line indent
    std::cout << std::endl;
}

void OptimalPlayer::optimalPlaceBattleship(const int *randomTraversing, int &beginIndex) {
    placeShipInCorner(ShipID::BATTLESHIP, getRandomFieldCorner(randomTraversing[beginIndex]));
    ++beginIndex;
}

FieldCorner OptimalPlayer::getRandomFieldCorner(const int &num) const {
    switch (num) {
        case 0:
            return FieldCorner::LEFT_TOP;
        case 1:
            return FieldCorner::RIGHT_TOP;
        case 2:
            return FieldCorner::LEFT_BOTTOM;
        case 3:
            return FieldCorner::RIGHT_BOTTOM;
        default:
            throw std::logic_error("Getting random field corner in optimal place error! Unknown field corner!");
    }
}

void OptimalPlayer::placeShipInCorner(const ShipID &shipID, const FieldCorner &edge) {
    int randValue = rand() % 2;
    Ship newShip;
    switch (edge) {
        case FieldCorner::LEFT_TOP: {
            if (randValue == 0) {
                newShip = Ship(shipID, FieldCell(0, 0), ShipDirection::DOWN);
            } else {
                newShip = Ship(shipID, FieldCell(0, 0), ShipDirection::RIGHT);
            }
            break;
        }
        case FieldCorner::RIGHT_TOP: {
            if (randValue == 0) {
                newShip = Ship(shipID, FieldCell(defaultFieldSize - 1, 0), ShipDirection::DOWN);
            } else {
                newShip = Ship(shipID, FieldCell(defaultFieldSize - 1, 0), ShipDirection::LEFT);
            }
            break;
        }
        case FieldCorner::LEFT_BOTTOM: {
            if (randValue == 0) {
                newShip = Ship(shipID, FieldCell(0, defaultFieldSize - 1), ShipDirection::UP);
            } else {
                newShip = Ship(shipID, FieldCell(0, defaultFieldSize - 1), ShipDirection::RIGHT);
            }
            break;
        }
        case FieldCorner::RIGHT_BOTTOM: {
            if (randValue == 0) {
                newShip = Ship(shipID, FieldCell(defaultFieldSize - 1, defaultFieldSize - 1), ShipDirection::UP);
            } else {
                newShip = Ship(shipID, FieldCell(defaultFieldSize - 1, defaultFieldSize - 1), ShipDirection::LEFT);
            }
            break;
        }

        default:
            throw std::logic_error("Placing ship in field corner error! Unknown field corner!");
    }

    placeNewShip(newShip);
    ships.emplace_back(newShip);
}

bool OptimalPlayer::canPlaceShip(Ship &ship) const {
    for (int index = 0; index < ship.size(); ++index) {
        if (!selfField.isFieldCell(ship[index]))
            return false;
    }

    ShipBorder ship_line = ship.getShipBorder();
    for (const auto &playerShip : ships) {
        if (ship_line.isBordersIntersect(playerShip.border())) {
            return false;
        }
    }

    return true;
}

void OptimalPlayer::placeNewShip(Ship &newShip) {
    for (int i = 0; i < newShip.size(); ++i) {
        selfField(newShip[i]) = FieldState::MY_SHIP;
    }
}

void OptimalPlayer::optimalPlaceCruiser(const unsigned int &count, const int *randomTraversing, int &beginIndex) {
    for (unsigned int shipsPlaced = 0; shipsPlaced < count; ++shipsPlaced) {
        placeShipInCorner(ShipID::CRUISER, getRandomFieldCorner(randomTraversing[beginIndex]));
        ++beginIndex;
    }
}

void OptimalPlayer::optimalPlaceDestroyer(const unsigned int &count, const int *randomTraversing, int &beginIndex) {
    // Place cruiser in last free corner
    placeShipInCorner(ShipID::DESTROYER, getRandomFieldCorner(randomTraversing[beginIndex]));
    ++beginIndex;

    int placedDestroyers = 1;
    int *randomEdgeTraversing = Randomer::generateRandomSequence(4);
    int beginEdgeIndex = 0;

    while (placedDestroyers != count) {
        if (tryPlaceOnEdge(destroyerSize, getRandomFieldEdge(randomEdgeTraversing[beginEdgeIndex]))) {
            ++placedDestroyers;
        }
        ++beginEdgeIndex;
    }

    delete [] randomEdgeTraversing;
}

bool OptimalPlayer::tryPlaceOnEdge(const unsigned int &shipSize, const FieldEdge &edge) {
    unsigned int counter = 0;
    switch (edge) {
        case FieldEdge::TOP: {
            for (int x = 0; x < defaultFieldSize; ++x) {
                if (selfField(x, 0) != FieldState::NOT_EXPLORED) {
                    counter = 0;
                } else {
                    ++counter;
                }
                if (counter == (shipSize + 2)) {
                    Ship newShip = Ship(ShipID::DESTROYER, x - 2, 0, ShipDirection::RIGHT);
                    placeNewShip(newShip);
                    ships.emplace_back(newShip);
                    return true;
                }
            }
            break;
        }
        case FieldEdge::BOTTOM: {
            for (int x = 0; x < defaultFieldSize; ++x) {
                if (selfField(x, defaultFieldSize - 1) != FieldState::NOT_EXPLORED) {
                    counter = 0;
                } else {
                    ++counter;
                }
                if (counter == (shipSize + 2)) {
                    Ship newShip = Ship(ShipID::DESTROYER, x - 2, defaultFieldSize - 1, ShipDirection::RIGHT);
                    placeNewShip(newShip);
                    ships.emplace_back(newShip);
                    return true;
                }
            }
            break;
        }
        case FieldEdge::RIGHT: {
            for (int y = 0; y < defaultFieldSize; ++y) {
                if (selfField(defaultFieldSize - 1, y) != FieldState::NOT_EXPLORED) {
                    counter = 0;
                } else {
                    ++counter;
                }
                if (counter == (shipSize + 2)) {
                    Ship newShip = Ship(ShipID::DESTROYER, defaultFieldSize - 1, y - 2, ShipDirection::DOWN);
                    placeNewShip(newShip);
                    ships.emplace_back(newShip);
                    return true;
                }
            }
            break;
        }
        case FieldEdge::LEFT: {
            for (int y = 0; y < defaultFieldSize; ++y) {
                if (selfField(0, y) != FieldState::NOT_EXPLORED) {
                    counter = 0;
                } else {
                    ++counter;
                }
                if (counter == (shipSize + 2)) {
                    Ship newShip = Ship(ShipID::DESTROYER, 0, y - 2, ShipDirection::DOWN);
                    placeNewShip(newShip);
                    ships.emplace_back(newShip);
                    return true;
                }
            }
        }
    }
    return false;
}

FieldEdge OptimalPlayer::getRandomFieldEdge(const int &num) const {
    switch (num) {
        case 0: {
            return FieldEdge::RIGHT;
        }
        case 1: {
            return FieldEdge::LEFT;
        }
        case 2: {
            return FieldEdge::TOP;
        }
        case 3: {
            return FieldEdge::BOTTOM;
        }
        default:
            throw std::logic_error("Getting random field edge error! Unknown field edge!");

    }
}

void OptimalPlayer::optimalPlaceCarrier(const unsigned int &count) {
    int *randomTraversing = Randomer::generateRandomSequence(defaultFieldSize * defaultFieldSize);
    int carriersPlaced = 0;
    int index = 0;

    while (carriersPlaced != count) {
        auto newShip = Ship(ShipID::CARRIER, FieldCell(randomTraversing[index]), ShipDirection::NO_DIRECTION);
        if (canPlaceShip(newShip)) {
            placeNewShip(newShip);
            ships.emplace_back(newShip);
            ++carriersPlaced;
        }
        ++index;
    }

    delete [] randomTraversing;
}

void OptimalPlayer::markAsHit(const FieldCell &cell) {
    enemyField(cell) = FieldState::HIT;
}

void OptimalPlayer::markAsDefeatedEnemyShip(const Ship &defeatedShip) {
    markUselessCells(defeatedShip.border());
    for (int i = 0; i < defeatedShip.size(); ++i)
        enemyField(defeatedShip[i]) = FieldState::DEFEAT;
}

StepStatus OptimalPlayer::takeAttack(Player* shooter, const FieldCell &cell) {
    if (selfField(cell) == FieldState::MY_SHIP) {

        // Check all ships
        for (auto it = ships.begin(); it != ships.end();) {

            // If is it ship cell
            if (it->isShipCell(cell)) {

                // Damage this cell
                it->damage(cell);

                // If after damage ship was defeat
                if (it->isDefeatShip()) {

                    // Marking defeated ship on the field
                    for (int j = 0; j < it->size(); ++j)
                        selfField((*it)[j]) = FieldState::DEFEAT;
                    shooter->markAsDefeatedEnemyShip((*it));

                    // Erase from ship list
                    ships.erase(it);
                    return StepStatus::DEFEAT;
                }

                shooter->markAsHit(cell);
                selfField(cell) = FieldState::HIT;
                return StepStatus::HIT;
            } else {
                ++it;
            }
        }
    }
    shooter->markAsMiss(cell);
    selfField(cell) = FieldState::EXPLORED;
    return StepStatus::MISS;
}

void OptimalPlayer::markAsMiss(const FieldCell &cell) {
    enemyField(cell) = FieldState::EXPLORED;
}

FieldCell OptimalPlayer::getNextCoords() {
    int *randomTraversing = Randomer::generateRandomSequence(defaultFieldSize * defaultFieldSize);
    int nextIndex = 0;

    for (int i = 0; i < defaultFieldSize * defaultFieldSize; ++i) {
        if (enemyField(randomTraversing[i]) == FieldState::NOT_EXPLORED) {
            nextIndex = randomTraversing[i];
            break;
        }
    }

    delete [] randomTraversing;
    return FieldCell(nextIndex);
}

StepStatus OptimalPlayer::tryFinishShip(const std::shared_ptr<Player>& enemy) {
    ShipDirection direction = findDirection(enemy);
    return shootLine(enemy, direction);
}

StepStatus OptimalPlayer::shootLine(const std::shared_ptr<Player> &enemy, const ShipDirection &direction) {
    FieldCell shift = getShift(direction);
    FieldCell firstHit = lastHittedCell;
    FieldCell nextCell = lastHittedCell + shift;

    StepStatus status = enemy->takeAttack(this, nextCell);
    //displayFields();
    while (status == StepStatus::HIT) {
        nextCell = nextCell + shift;
        if (!enemyField.isFieldCell(nextCell)) {
            shift = getShift(changeToOppositeDirection(direction));
            nextCell = firstHit + shift;
        }
        status = enemy->takeAttack(this, nextCell);
       // displayFields();
    }

    if (status == StepStatus::MISS) {
        lastHittedCell = firstHit;
    }

    return status;
}

ShipDirection OptimalPlayer::findDirection(const std::shared_ptr<Player> &enemy) {
    FieldCell nextCell(0, 0);

    if (enemyField.isFieldCell(lastHittedCell + FieldCell(1, 0))) {
        nextCell = lastHittedCell + FieldCell(1, 0);
        if (enemyField(nextCell) == FieldState::NOT_EXPLORED) {
            return ShipDirection::RIGHT;
        } else if (enemyField(nextCell) == FieldState::HIT) {
            return ShipDirection::LEFT;
        }
    }

    if (enemyField.isFieldCell(lastHittedCell + FieldCell(-1, 0))) {
        nextCell = lastHittedCell + FieldCell(-1, 0);
        if (enemyField(nextCell) == FieldState::NOT_EXPLORED) {
            return ShipDirection::LEFT;
        } else if (enemyField(nextCell) == FieldState::HIT) {
            return ShipDirection::RIGHT;
        }
    }

    if (enemyField.isFieldCell(lastHittedCell + FieldCell(0, -1))) {
        nextCell = lastHittedCell + FieldCell(0, -1);
        if (enemyField(nextCell) == FieldState::NOT_EXPLORED) {
            return ShipDirection::UP;
        } else if (enemyField(nextCell) == FieldState::HIT) {
            return ShipDirection::DOWN;
        }
    }

    if (enemyField.isFieldCell(lastHittedCell + FieldCell(0, 1))) {
        nextCell = lastHittedCell + FieldCell(0, 1);
        if (enemyField(nextCell) == FieldState::NOT_EXPLORED) {
            return ShipDirection::DOWN;
        } else if (enemyField(nextCell) == FieldState::HIT) {
            return ShipDirection::UP;
        }
    }

    displayFields();
    enemy->displayFields();

    throw std::logic_error("Error of finding next cell for shoot!");
}

ShipDirection OptimalPlayer::changeToOppositeDirection(const ShipDirection &direction) {
    switch (direction) {
        case ShipDirection::RIGHT:
            return ShipDirection::LEFT;

        case ShipDirection::LEFT:
            return ShipDirection::RIGHT;

        case ShipDirection::DOWN:
            return ShipDirection::UP;

        case ShipDirection::UP:
            return ShipDirection::DOWN;

        default:
           throw std::logic_error("Cannot invert direction for shooting! Unknown direction!");
    }
}

FieldCell OptimalPlayer::getShift(const ShipDirection &direction) const {
    switch (direction) {
        case ShipDirection::RIGHT:
            return FieldCell(1, 0);

        case ShipDirection::LEFT:
            return FieldCell(-1, 0);

        case ShipDirection::UP:
            return FieldCell(0, -1);

        case ShipDirection::DOWN:
            return FieldCell(0, 1);

        default:
            throw std::logic_error("Error of getting shift for shooting! Unknown direction!");
    }
}

void OptimalPlayer::markUselessCells(const ShipBorder &defeatedShipBorder) {
    FieldCell lt = defeatedShipBorder.getTopCorner();
    FieldCell rb = defeatedShipBorder.getBottomCorner();

    int min_x = lt.x();
    int min_y = rb.y();
    int max_x = rb.x();
    int max_y = lt.y();

    for (int x = min_x; x <= max_x; ++x) {
        for (int y = min_y; y <= max_y; ++y) {
            if (enemyField.isFieldCell(FieldCell(x, y)))
                enemyField(x, y) = FieldState::EXPLORED;
        }
    }
}
