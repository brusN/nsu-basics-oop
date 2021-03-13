#include "battlefield.h"
#include <iostream>

//! FieldCell !//

FieldCell::FieldCell(const int &x, const int &y) : x_(x), y_(y) { }

FieldCell::FieldCell(const FieldCell &cell) {
    x_ = cell.x();
    y_ = cell.y();
}

int FieldCell::x() const {
    return x_;
}

int FieldCell::y() const {
    return y_;
}

FieldCell FieldCell::operator+(const FieldCell &cell) const {
    return FieldCell(x_ + cell.x(), y_ + cell.y());
}

FieldCell FieldCell::operator-(const FieldCell &cell) const {
    return FieldCell(x_ - cell.x(), y_ - cell.y());
}

FieldCell &FieldCell::operator=(const FieldCell &cell) {
    x_ = cell.x();
    y_ = cell.y();
    return *this;
}

bool FieldCell::operator==(const FieldCell &cell) const {
    return (x_ == cell.x() && y_ == cell.y());
}

FieldCell::FieldCell(const int &index) {
    x_ = index % static_cast<int>(defaultFieldSize);
    y_ = index / static_cast<int>(defaultFieldSize);
}

//! Battlefield !//

Battlefield::Battlefield(const int &initSize) {
    field = new FieldState[initSize * initSize];
    fieldSize = initSize;
    for (unsigned int cellIndex = 0; cellIndex < fieldSize * fieldSize; ++cellIndex)
        field[cellIndex] = FieldState::NOT_EXPLORED;
}

Battlefield::~Battlefield() {
    delete [] field;
}

unsigned int Battlefield::size() const {
    return fieldSize;
}

bool Battlefield::isFieldCell(const int &x, const int &y) const {
    if (x >= 0 && x <= fieldSize - 1 && y >= 0 && y <= fieldSize - 1)
        return true;
    return false;
}

bool Battlefield::isFieldCell(const FieldCell &cell) const {
    int x = cell.x();
    int y = cell.y();
    if (x >= 0 && x <= fieldSize - 1 && y >= 0 && y <= fieldSize - 1)
        return true;
    return false;
}

void Battlefield::reset() {
    for (unsigned int cellIndex = 0; cellIndex < fieldSize * fieldSize; ++cellIndex)
        field[cellIndex] = FieldState::NOT_EXPLORED;
}

void Battlefield::printField() const {
    // Prints X-axis
    std::cout << ' ';
    for (unsigned int x = 0; x < fieldSize; ++x)
        std::cout << ' ' << x;
    std::cout << std::endl;

    for (unsigned int y = 0; y < fieldSize; ++y) {
        // Prints Y-axis
        std::cout << y;

        for (unsigned int x = 0; x < fieldSize; ++x) {
            std::cout << ' ' << static_cast<char>(field[y * fieldSize + x]);
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

FieldState & Battlefield::operator()(const int &x, const int &y) {
    return field[y * fieldSize + x];
}

FieldState &Battlefield::operator()(const FieldCell &cell) {
    return field[cell.y() * fieldSize + cell.x()];
}


