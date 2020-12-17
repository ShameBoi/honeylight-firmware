/**
 * Font.h
 *
 * @author Shame Boi
 */
#pragma once

#include <cstdint>

#include <honeylight/config.h>
#include <honeylight/display_types.h>
#include <honeylight/data/DisplayBuffer.h>

class Font {
public:
    static constexpr uint8_t const SymbolWidth = 7;

private:
    static constexpr uint8_t getSymbolRowWidth(uint8_t const rowNumber) {
        switch (rowNumber) {
            case 0:
            case 4:
            default:
                return SymbolWidth - 2;

            case 1:
            case 3:
                return SymbolWidth - 1;

            case 2:
                return SymbolWidth;
        }
    }

    struct [[gnu::packed]] row1_t {
        bool A: 1;
        bool B: 1;
        bool C: 1;
        bool D: 1;
        bool E: 1;
        unsigned: 3;
    };

    struct [[gnu::packed]] row2_t {
        bool A: 1;
        bool B: 1;
        bool C: 1;
        bool D: 1;
        bool E: 1;
        bool F: 1;
        unsigned: 2;
    };

    struct [[gnu::packed]] row3_t {
        bool A: 1;
        bool B: 1;
        bool C: 1;
        bool D: 1;
        bool E: 1;
        bool F: 1;
        bool G: 1;
        unsigned: 1;
    };

    struct [[gnu::packed]] row4_t {
        bool A: 1;
        bool B: 1;
        bool C: 1;
        bool D: 1;
        bool E: 1;
        bool F: 1;
        unsigned: 2;
    };

    struct [[gnu::packed]] row5_t {
        bool A: 1;
        bool B: 1;
        bool C: 1;
        bool D: 1;
        bool E: 1;
        unsigned: 3;
    };

    struct [[gnu::packed]] rows_t {
        row1_t row1;
        row2_t row2;
        row3_t row3;
        row4_t row4;
        row5_t row5;
    };

    union [[gnu::packed]] symbol_t {
        rows_t asRows;
        uint8_t asBytes[HONEYLIGHT_DISPLAY_ROWS];

        symbol_t(): asBytes{0} {}

        explicit symbol_t(rows_t const rows): asRows(rows) {}

        symbol_t(row1_t const row1,
                 row2_t const row2,
                 row3_t const row3,
                 row4_t const row4,
                 row5_t const row5)
                 : asRows{row1, row2, row3, row4, row5} {}

        bool getRowBit(uint8_t const row, uint8_t const bit) const {
            if (bit > 7) {
                return false;
            }
            return (asBytes[row >= HONEYLIGHT_DISPLAY_ROWS ? HONEYLIGHT_DISPLAY_ROWS : row] & (1U << bit)) != 0;
        }
    };

    static symbol_t const Alphabet[26];

    static symbol_t const Numerals[10];

    static symbol_t const Blank;

    static symbol_t const & getSymbolFor(char character, bool * error);

public:
    static bool renderCharacter(char character, color_t const & color, uint8_t xOffset, DisplayBuffer * buffer);

    Font() = delete;
};



