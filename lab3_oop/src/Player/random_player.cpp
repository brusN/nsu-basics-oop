#include "player.h"
#include "gameview.h"

void RandomPlayer::placeShips() {
    const unsigned int fieldSize = selfField.size();
    int beginIndex = 0;
    int* randomTraversing = Randomer::generateRandomSequence(fieldSize * fieldSize);

    randomPlaceShip(ShipID::BATTLESHIP, 1, randomTraversing, beginIndex);
    randomPlaceShip(ShipID::CRUISER, 2, randomTraversing, beginIndex);
    randomPlaceShip(ShipID::DESTROYER, 3, randomTraversing, beginIndex);
    randomPlaceShip(ShipID::CARRIER, 4, randomTraversing, beginIndex);

    delete [] randomTraversing;
}

GameStatus RandomPlayer::doStep(std::shared_ptr<Player> enemy) {
    StepStatus status = enemy->takeAttack(this, getRandomCoords());

    if (enemy->shipsLeft() == 0)
        return GameStatus::END_ROUND;

    if (status == StepStatus::HIT || status == StepStatus::DEFEAT)
        return GameStatus::CONTINUE;

    return GameStatus::NEXT_PLAYER_TURN;
}

void RandomPlayer::reset() {
    selfField.reset();
    enemyField.reset();
    ships.clear();
}

unsigned int RandomPlayer::shipsLeft() {
    return ships.size();
}

void RandomPlayer::randomPlaceShip(const ShipID &id, const unsigned int &count, const int *randomTraversing, int &beginIndex) {
    unsigned int shipsLeft = count;

    while (shipsLeft) {
        int *randomDirections = Randomer::generateRandomSequence(4);
        for (unsigned int i = 0; i < 4; ++i) {
            auto newRandomShip = Ship(id, FieldCell(randomTraversing[beginIndex]),getRandomDirection(randomDirections[i]));
            if (canPlaceShip(newRandomShip)) {
                placeNewShip(newRandomShip);
                ships.emplace_back(newRandomShip);
                --shipsLeft;
                break;
            }
        }
        ++beginIndex;
        delete [] randomDirections;
    }
}

ShipDirection RandomPlayer::getRandomDirection(const int &num) const {
    switch (num) {
        case 0:
            return ShipDirection::LEFT;
        case 1:
            return ShipDirection::RIGHT;
        case 2:
            return ShipDirection::DOWN;
        case 3:
            return ShipDirection::UP;
        default:
            return ShipDirection::UNKNOWN;
    }
}

void RandomPlayer::displayFields() {
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

bool RandomPlayer::canPlaceShip(Ship &ship) const {
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

void RandomPlayer::placeNewShip(Ship &newShip) {
    for (int i = 0; i < newShip.size(); ++i) {
        selfField(newShip[i]) = FieldState::MY_SHIP;
    }
}

StepStatus RandomPlayer::takeAttack(Player* shooter, const FieldCell &cell) {
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

FieldCell RandomPlayer::getRandomCoords() {
    int *randomTraversing = Randomer::generateRandomSequence(defaultFieldSize * defaultFieldSize);
    int nextIndex = 0;

    for (int fieldIndex = 0; fieldIndex < defaultFieldSize * defaultFieldSize; ++fieldIndex) {
        if (enemyField(FieldCell(randomTraversing[fieldIndex])) == FieldState::NOT_EXPLORED) {
            nextIndex = randomTraversing[fieldIndex];
            break;
        }
    }

    delete [] randomTraversing;
    return FieldCell(nextIndex);
}

void RandomPlayer::markAsHit(const FieldCell &cell) {
    enemyField(cell) = FieldState::HIT;
}

void RandomPlayer::markAsDefeatedEnemyShip(const Ship &defeatedShip) {
    for (int i = 0; i < defeatedShip.size(); ++i) {
        enemyField(defeatedShip[i]) = FieldState::DEFEAT;
    }
}

void RandomPlayer::markAsMiss(const FieldCell &cell) {
    enemyField(cell) = FieldState::EXPLORED;
}


