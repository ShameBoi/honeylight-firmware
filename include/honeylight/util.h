/**
 * util.h
 *
 * @author Shame Boi
 */
#pragma once

#include <honeylight/display_types.h>

class Util {
public:
    Util() = delete;

    static color_t hsvToRgb(uint16_t h, uint8_t s, uint8_t v);

    template<typename UIntType, typename IntType>
    static UIntType addAndSetNegativeToZero(UIntType const valueOne, IntType const valueTwo) {
        IntType tempResult = static_cast<IntType>(valueOne) + valueTwo;
        if (tempResult < 0) {
            return 0;
        } else {
            return static_cast<UIntType>(tempResult);
        }
    }
};
