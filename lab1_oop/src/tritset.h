#ifndef LAB1_OOOP_TRITSET_H
#define LAB1_OOOP_TRITSET_H

#include <iostream>
#include <cstring>
#include <unordered_map>

namespace tritClass {
    using uint = unsigned int;

    constexpr size_t tritInUint = sizeof(uint) * 4; // number of trites in one uint
    enum TritValue { UNKNOWN, FALSE, TRUE };

    TritValue operator& (const TritValue& value1, const TritValue& value2); // tested
    TritValue operator| (const TritValue& value1, const TritValue& value2); // tested
    TritValue operator! (const TritValue& value); // tested

    std::ostream & operator<< (std::ostream &os, const TritValue& value); // tested

    class TritSet {
    private:
        uint *array;
        size_t size;
        size_t array_length;
        size_t lastIndex;
        size_t initSize;

        uint *getArray() const; // doesn't require to test
        void setTrit(size_t index, TritValue value); // tested with operator[]
        TritValue getTrit(size_t index) const; // tested with setTrit
        void realloc(size_t newSize); // tested

    public:
        class Trit;
        friend class TritSet::Trit;

        explicit TritSet(size_t size); // doesn't require to test
        ~TritSet(); // doesn't require to test

        size_t getSize() const; // tested
        size_t getArrayLength() const; // tested
        size_t getLastIndex() const; // tested
        size_t getInitSize() const;
        size_t capacity() const; // tested

        TritSet(TritSet const &set); // tested
        TritSet &operator=(TritSet const &set); // tested

        TritValue operator[](size_t index) const; // doesn't require test, because operator calls another method - getTrit, which was tested
        Trit operator[](size_t index); // tested

        TritSet operator! () const; // tested
        TritSet operator& (const TritSet& set) const; // tested
        TritSet operator| (const TritSet& set) const; // tested

        void shrink(); // tested
        size_t cardinatily(TritValue value) const; // tested
        std::unordered_map <TritValue, size_t, std::hash<TritValue>> cardinatily() const; // tested
        void trim(size_t index); // tested
        size_t length() const; // doesn't require test

        TritSet::Trit begin();
        TritSet::Trit end();

        friend std::ostream & operator<< (std::ostream &os, const TritSet &set); // tested

        class Trit {
        private:
            TritSet &set;
            size_t index;

        public:
            Trit(TritSet &set, size_t index);
            TritValue value() const;

            // For construction (for auto i : set)
            Trit &operator++();
            Trit operator*() const;

            bool operator!=(const Trit &trit) const;
            bool operator==(const Trit &trit) const;
            bool operator==(const TritValue &value) const;

            Trit &operator=(TritValue value); // tested
            Trit &operator=(Trit value); // tested

            TritValue operator!() const; // tested

            TritValue operator& (TritValue trit) const; // tested
            TritValue operator& (const Trit &trit) const; // tested

            TritValue operator| (TritValue trit) const; // tested
            TritValue operator| (const Trit& trit) const; // tested


            friend std::ostream & operator<< (std::ostream& os, const TritSet::Trit& trit); // tested
        };
    };
}

#endif //LAB1_OOOP_TRITSET_H
