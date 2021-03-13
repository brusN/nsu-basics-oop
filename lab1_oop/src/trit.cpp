#include "tritset.h"

namespace tritClass {
    TritSet::Trit::Trit(TritSet &set_, size_t index_) : set(set_), index(index_) {}

    TritValue TritSet::Trit::value() const {
        return this->set.getTrit(index);
    }

    TritSet::Trit &TritSet::Trit::operator++() {
        ++index;
        return *this;
    }

    TritSet::Trit TritSet::Trit::operator*() const {
        return *this;
    }

    bool TritSet::Trit::operator!=(const TritSet::Trit &trit) const {
        if (this->index == trit.index)
            return false;
        return true;
    }

    TritSet::Trit &TritSet::Trit::operator=(TritValue value) {
        set.setTrit(index, value);
        return (*this);
    }

    TritSet::Trit &TritSet::Trit::operator=(TritSet::Trit value) {
        set[index] = value.set.getTrit(value.index);
        return (*this);
    }

    TritValue TritSet::Trit::operator!() const {
        return !this->set.getTrit(index);
    }

    TritValue TritSet::Trit::operator&(TritValue trit) const {
        return this->set.getTrit(index) & trit;
    }

    TritValue TritSet::Trit::operator&(const TritSet::Trit &trit) const {
        return this->set.getTrit(index) & trit.set.getTrit(trit.index);
    }

    TritValue TritSet::Trit::operator|(TritValue trit) const {
        return this->set.getTrit(index) | trit;
    }

    TritValue TritSet::Trit::operator|(const TritSet::Trit &trit) const {
        return this->set.getTrit(index) | trit.set.getTrit(trit.index);
    }

    bool TritSet::Trit::operator==(const TritSet::Trit &trit) const {
        if (trit.index == this->index)
            return true;
        return false;
    }

    bool TritSet::Trit::operator==(const TritValue &value) const {
        if (this->set.getTrit(index) == value)
            return true;
        return false;
    }

    std::ostream &operator<<(std::ostream &os, const TritSet::Trit &trit) {
        os << static_cast<const TritSet>(trit.set)[trit.index];
        return os;
    }
}
