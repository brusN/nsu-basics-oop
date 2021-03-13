#include "tritset.h"

namespace tritClass {

    TritValue operator&(const TritValue &value1, const TritValue &value2) {
        if (value1 == FALSE || value2 == FALSE)
            return FALSE;

        if (value1 == UNKNOWN || value2 == UNKNOWN)
            return UNKNOWN;

        return TRUE;
    }

    TritValue operator|(const TritValue &value1, const TritValue &value2) {
        if (value1 == TRUE || value2 == TRUE)
            return TRUE;

        if (value1 == UNKNOWN || value2 == UNKNOWN)
            return UNKNOWN;

        return FALSE;
    }

    TritValue operator!(const TritValue &value) {
        if (value == UNKNOWN)
            return UNKNOWN;

        if (value == TRUE)
            return FALSE;

        return TRUE;
    }

    std::ostream &operator<<(std::ostream &os, const TritValue &value) {
        switch (value) {
            case TRUE:
                os << "TRUE";
                break;

            case FALSE:
                os << "FALSE";
                break;

            default:
                os << "UNKNOWN";
                break;
        }
        return os;
    }
}