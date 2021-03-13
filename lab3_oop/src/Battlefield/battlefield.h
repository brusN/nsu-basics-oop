#ifndef NEWLAB3_BATTLEFIELD_H
#define NEWLAB3_BATTLEFIELD_H

// This represent the values of the cells of the player fields
enum class FieldState {
    NOT_EXPLORED = ' ', EXPLORED = '.', HIT = 'X', DEFEAT = 'D', MY_SHIP = '#', UNKNOWN = '?'
};

// Default field in battleship game is 10x10
static const unsigned int defaultFieldSize = 10;

// Cell representation of coordinates
class FieldCell {
private:
    int x_;
    int y_;
public:
    FieldCell(const int &index);
    FieldCell(const int &x, const int &y);
    FieldCell(const FieldCell &cell);
    int x() const;
    int y() const;
    FieldCell operator+(const FieldCell &cell) const;
    FieldCell operator-(const FieldCell &cell) const;
    FieldCell &operator=(const FieldCell &cell);
    bool operator==(const FieldCell &cell) const;
};

// Enemy and self fields
class Battlefield {
private:
    FieldState *field;
    unsigned int fieldSize;

public:
    explicit Battlefield(const int &initSize = defaultFieldSize);
    ~Battlefield();
    unsigned int size() const;
    bool isFieldCell(const int &x, const int &y) const; // in other words, checking for array out of bounds
    bool isFieldCell(const FieldCell &cell) const;
    FieldState & operator()(const int &x, const int &y);
    FieldState & operator()(const FieldCell &cell);
    void reset(); // sets all values to NOT_EXPLORED
    void printField() const;
};

#endif //NEWLAB3_BATTLEFIELD_H
