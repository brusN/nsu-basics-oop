#include "player.h"
#include "fabrica.h"
#include "gameview.h"
#include <algorithm>

std::vector<std::string> ConsolePlayer::readCoordsFromConsole() const {
    std::string temp;
    std::vector<std::string> coords;

    coords.clear();
    // Read X and Y (= 2)
    for (unsigned int i = 0; i < 2; ++i) {
        std::cin >> temp;
        coords.emplace_back(temp);
    }
    return coords;
}

bool ConsolePlayer::isRightCoords(const int &x, const int &y, const ShipDirection &direction) const {
    return (selfField.isFieldCell(x, y) && direction != ShipDirection::UNKNOWN);
}

bool ConsolePlayer::canPlaceShip(Ship &ship) const {
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

void ConsolePlayer::getCoords(int &x, int &y, ShipDirection &direction) const {
    bool isRight = false;
    do {
        isRight = readCoords(x, y);
        direction = readShipDirection();
        if (!isRight || !isRightCoords(x, y, direction)) {
            std::cout << "Incorrect coords for placement! Try again: ";
            isRight = false;
        } else {
            isRight = true;
        }
    } while (!isRight);
}


void ConsolePlayer::placeShip(const ShipID &shipID, const unsigned int &shipsAmount, const unsigned int &size) {
    // Data from console input
    int x, y;
    ShipDirection direction;
    bool canReplace = false;

    int shipsLeft = shipsAmount;
    for (unsigned int amount = 0; amount < shipsAmount; ++amount) {
        displayFields();
        std::cout << "Place ship with size "<< size << "x1 [" << shipsLeft << " ships left]" << std::endl;
        std::cout << "Enter coords and direction: ";
        do {
            getCoords(x, y, direction);
            auto newShip = Ship(shipID, x, y, direction);
            canReplace = canPlaceShip(newShip);
            if (canReplace) {
                placeNewShip(newShip);
                ships.emplace_back(newShip);
            } else {
                std::cout << "Can't place here. Try again: ";
            }
        } while (!canReplace);
        --shipsLeft;
        system("cls");
    }
}

void ConsolePlayer::placeShips() {
    placeShip(ShipID::BATTLESHIP, battleshipAmount, battleshipSize);
    placeShip(ShipID::CRUISER, cruiserAmount, cruiserSize);
    placeShip(ShipID::DESTROYER, destroyerAmount, destroyerSize);
    placeShip(ShipID::CARRIER, carrierAmount, carrierSize);
}

GameStatus ConsolePlayer::doStep(std::shared_ptr<Player> enemy) {
    system("cls");
    displayFields();
    std::cout << "Enter coords: ";
    int x, y;
    bool isRightCoords = false;

    do {
        if (!readCoords(x, y)) {
            isRightCoords = false;
            std::cout << "Incorrect coords! Try again: ";
            continue;
        }
        isRightCoords = enemyField.isFieldCell(x, y);
        if (!isRightCoords) {
            isRightCoords = false;
            std::cout << "Can't shoot here! Try again: ";
            continue;
        }
        if (enemyField(x, y) != FieldState::NOT_EXPLORED) {
            isRightCoords = false;
            std::cout << "This sell has already explored. Enter another coords: ";
        }
        isRightCoords = true;
    } while (!isRightCoords);

    StepStatus status = enemy->takeAttack(this, FieldCell(x, y));

    if (enemy->shipsLeft() == 0) {
        return GameStatus::END_ROUND;
    }

    system("cls");
    displayFields();
    if (status == StepStatus::HIT) {
        std::cout << "You are hit!" << std::endl;
        system("pause");
        return GameStatus::CONTINUE;
    } else if (status == StepStatus::DEFEAT) {
        std::cout << "You are defeated enemy ship!" << std::endl;
        system("pause");
        return GameStatus::CONTINUE;
    } else {
        std::cout << "You are missed!" << std::endl;
        system("pause");
    }

    return GameStatus::NEXT_PLAYER_TURN;
}

void ConsolePlayer::reset() {
    selfField.reset();
    enemyField.reset();
    ships.clear();
}

unsigned int ConsolePlayer::shipsLeft() {
    return ships.size();
}

void ConsolePlayer::displayFields() {
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

ShipDirection ConsolePlayer::readShipDirection() const {
    std::string temp;
    std::cin >> temp;
    std::transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
    if (temp.size() > 1) {
        if (temp == "right") {
            return ShipDirection::RIGHT;
        } else if (temp == "left") {
            return ShipDirection::LEFT;
        } else if (temp == "up") {
            return ShipDirection::UP;
        } else if (temp == "down") {
            return ShipDirection::DOWN;
        } else if (temp == "no") {
            return ShipDirection::NO_DIRECTION;
        } else {
            return ShipDirection::UNKNOWN;
        }
    }

    switch (temp[0]) {
        case 'r':
            return ShipDirection::RIGHT;
        case 'l':
            return ShipDirection::LEFT;
        case 'u':
            return ShipDirection::UP;
        case 'd':
            return ShipDirection::DOWN;
        case 'n':
            return ShipDirection::NO_DIRECTION;
        default:
            return ShipDirection::UNKNOWN;
    }
}

void ConsolePlayer::placeNewShip(Ship &newShip) {
    for (int i = 0; i < newShip.size(); ++i) {
        selfField(newShip[i]) = FieldState::MY_SHIP;
    }
}

bool ConsolePlayer::readCoords(int &x, int &y) const {
    std::vector<std::string> coords = readCoordsFromConsole();
    if (!(Parser::isNumber(coords[0]) && Parser::isNumber(coords[1])))
        return false;
    x = atoi(coords[0].c_str());
    y = atoi(coords[1].c_str());
    return true;
}


StepStatus ConsolePlayer::takeAttack(Player* shooter, const FieldCell &cell) {
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

void ConsolePlayer::markAsHit(const FieldCell &cell) {
    enemyField(cell) = FieldState::HIT;
}

void ConsolePlayer::markAsDefeatedEnemyShip(const Ship &defeatedShip) {
    for (int i = 0; i < defeatedShip.size(); ++i) {
        enemyField(defeatedShip[i]) = FieldState::DEFEAT;
    }
}

void ConsolePlayer::markAsMiss(const FieldCell &cell) {
    enemyField(cell) = FieldState::EXPLORED;
}
