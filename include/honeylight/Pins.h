/**
 * Pins.h
 *
 * @author Shame Boi
 */
#pragma once

#include <cstdint>

namespace Pin {
    constexpr static uint8_t const EncoderPhaseA = 2;
    constexpr static uint8_t const EncoderPhaseB = 3;
    constexpr static uint8_t const EncoderSwitch = 4;

    constexpr static uint8_t const LEDData = 26;
    constexpr static uint8_t const LEDClock = 27;
}
