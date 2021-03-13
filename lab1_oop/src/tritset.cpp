#include "tritset.h"

namespace tritClass {
    void printBinary(unsigned int num) {
        for (int i = 0; i < 4; ++i) {
            std::string temp;
            for (int j = 0; j < 8; ++j) {
                temp = std::to_string(num % 2) + temp;
                num = num >> 1;
            }
            std::cout << temp << " ";
        }
        std::cout << std::endl;
    }

    uint *TritSet::getArray() const {
        return array;
    }

    void TritSet::setTrit(size_t index, TritValue value) {
        if (value == UNKNOWN && (index > lastIndex || lastIndex == -1))
            return;

        if (index >= size)
            realloc(index + 1);

        if (value != UNKNOWN && (index > lastIndex || lastIndex == -1))
            lastIndex = index;

        uint shift = (index % tritInUint) * 2;
        size_t arrayIndex = index / tritInUint;
        uint temp = (array[arrayIndex] >> shift) & 3u; // 0x3 == 3 == 00000011
        array[arrayIndex] ^= (temp ^ static_cast<uint>(value)) << shift;

        if (value == UNKNOWN && index == lastIndex) {
            long newLastIndex = index - 1;
            while (getTrit(newLastIndex) == UNKNOWN && newLastIndex >= 0) {
                --newLastIndex;
            }
            lastIndex = newLastIndex;
        }
    }

    TritValue tritClass::TritSet::getTrit(size_t index) const {
        if (lastIndex == -1 || index > lastIndex)
            return UNKNOWN;
        uint trit_value =
                (array[index / tritInUint] >> ((index % tritInUint) * 2)) & 3u; // 3u - trit's mask 0x3 == [00000011]
        return TritValue(trit_value);
    }

    void TritSet::realloc(size_t newSize) {
        if (size == newSize)
            return;
        size_t prevSize = this->size;
        size_t prevArrayLength = this->array_length;

        size_t arrayLength = (newSize + tritInUint - 1) / tritInUint; // Высчитываем новую длину массива uint
        uint *newArray = new uint[arrayLength];

        this->size = newSize; // Записываем уже новые значения
        this->array_length = arrayLength;

        if (arrayLength > prevArrayLength)
            arrayLength = prevArrayLength;

        for (size_t i = 0; i < arrayLength; ++i)
            newArray[i] = array[i];// Делаем копию массива uint
        for (size_t i = arrayLength; i < this->array_length; ++i)
            newArray[i] = 0; // заполняем дополнительные uint значением unknown

        for (size_t i = prevSize; i < newSize; ++i) // Заполняем ненужные триты значением UNKNOWN
            setTrit(i, UNKNOWN);

        if (lastIndex > newSize) {
            long newLastIndex = newSize;
            while (getTrit(newLastIndex) == UNKNOWN && newLastIndex >= 0)
                --newLastIndex;
            lastIndex = newLastIndex;
        }

        delete [] this->array;
        this->array = newArray;
    }

    TritSet::TritSet(size_t size_) : size(size_), initSize(size_) {
        array_length = (size_ + tritInUint - 1) / tritInUint;
        array = new uint[array_length];
        lastIndex = -1;
        for (size_t i = 0; i < array_length; ++i)
            array[i] = 0;

    }

    TritSet::~TritSet() {
        delete [] array;
    }

    size_t TritSet::getSize() const {
        return size;
    }

    size_t TritSet::getArrayLength() const {
        return array_length;
    }

    size_t TritSet::getLastIndex() const {
        return lastIndex;
    }

    size_t TritSet::getInitSize() const {
        return initSize;
    }

    size_t TritSet::capacity() const {
        return tritInUint * array_length;
    }

    TritSet::TritSet(const TritSet &set) {
        size = set.size;
        array_length = set.array_length;
        lastIndex = set.lastIndex;
        initSize = set.size;
        array = new uint[array_length];
        memcpy(array, set.array, sizeof(uint) * array_length);
    }

    TritSet &tritClass::TritSet::operator=(const TritSet &set) {
        if (this != &set) {
            size = set.size;
            array_length = set.array_length;
            lastIndex = set.lastIndex;
            delete[] array;
            array = new uint[array_length];
            memcpy(array, set.array, sizeof(uint) * array_length);
        }
        return (*this);
    }

    TritValue TritSet::operator[](size_t index) const {
        return getTrit(index);
    }

    TritSet::Trit TritSet::operator[](size_t index) {
        return Trit(*this, index);
    }

    TritSet TritSet::operator!() const {
        TritSet temp(this->size);
        for (size_t i = 0; i < temp.size; ++i)
            temp[i] = !(*this)[i];

        return temp;
    }

    TritSet TritSet::operator&(const TritSet &set) const {
        size_t size1 = this->size;
        size_t size2 = set.size;
        TritSet temp(size1 > size2 ? size1 : size2);

        for (size_t i = 0; i < temp.size; ++i)
            temp[i] = (*this)[i] & set[i];

        return temp;
    }

    TritSet TritSet::operator|(const TritSet &set) const {
        size_t size1 = size;
        size_t size2 = set.size;
        TritSet temp(size1 > size2 ? size1 : size2);

        for (size_t i = 0; i < temp.size; ++i)
            temp[i] = (*this)[i] | set[i];

        return temp;
    }

    void TritSet::shrink() {
        size_t newSize;
        if (lastIndex == -1)
            newSize = initSize;
        else
            newSize = lastIndex + 1;

        realloc(newSize);
    }

    size_t TritSet::cardinatily(TritValue value) const {
        if (lastIndex == -1) {
            if (value == UNKNOWN) {
                return size;
            } else {
                return 0;
            }
        }

        size_t count = 0;
        for (size_t i = 0; i < size; ++i) {
            if (this->getTrit(i) == value)
                ++count;
        }

        return count;
    }

    std::unordered_map<TritValue, size_t, std::hash<TritValue>> TritSet::cardinatily() const {
        std::unordered_map<TritValue, size_t, std::hash<TritValue>> map;


        if (lastIndex != -1) {
            for (size_t i = 0; i < size; ++i)
                ++map[this->getTrit(i)];
        } else {
            map[UNKNOWN] = size;
        }

        return std::unordered_map<TritValue, size_t, std::hash<TritValue>>(map);
    }

    void TritSet::trim(size_t index) {
        if (index <= lastIndex) {
            if (lastIndex == -1)
                return;

            for (size_t i = index; i <= lastIndex; ++i)
                array[i] = UNKNOWN;

            lastIndex = index - 1;
        }
    }

    size_t TritSet::length() const {
        return lastIndex + 1;
    }

    TritSet::Trit TritSet::begin() {
        return TritSet::Trit(*this, 0);
    }

    TritSet::Trit TritSet::end() {
        return TritSet::Trit(*this, this->size);
    }

    std::ostream &operator<<(std::ostream &os, const TritSet &set) {
        size_t lastIndex = set.getLastIndex();
        for (size_t i = 0; i < lastIndex; ++i)
            os << set.getTrit(i) << ' ';
        os << set.getTrit(lastIndex);
        return os;
    }
}