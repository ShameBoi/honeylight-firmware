/**
 * Font.cpp
 *
 * @author Shame Boi
 */

#include <cctype>
#include <honeylight/config.h>
#include <honeylight/renderers/Font.h>

bool Font::renderCharacter(char const character,
                           color_t const &color,
                           uint8_t const xOffset,
                           DisplayBuffer *const buffer) {
    if (xOffset >= (HONEYLIGHT_DISPLAY_COLS - SymbolWidth)) {
        return false;
    }
    bool error = false;
    symbol_t const & symbol = getSymbolFor(character, &error);
    if (error) {
        return false;
    }

    for (uint8_t rowIndex = 0; rowIndex < HONEYLIGHT_DISPLAY_ROWS; ++rowIndex) {
        Row & row = buffer->atRow(rowIndex);
        for (uint8_t colIndex = 0; colIndex < getSymbolRowWidth(rowIndex); ++colIndex) {
            if (symbol.getRowBit(rowIndex, colIndex)) {
                row[xOffset + colIndex] = color;
            }
        }
    }

    return true;
}

Font::symbol_t const & Font::getSymbolFor(char const character, bool * const error) {
    if (isalpha(character)) {
        auto const index = static_cast<uint8_t>(toupper(character) - 'A');
        if (index >= (sizeof(Alphabet) / sizeof(*Alphabet))) {
            *error = true;
            return Blank;
        }
        *error = false;
        return Alphabet[index];
    } else if (isdigit(character)) {
        auto const index = static_cast<uint8_t>(character - '0');
        if (index >= (sizeof(Numerals) / sizeof(*Numerals))) {
            *error = true;
            return Blank;
        }
        *error = false;
        return Numerals[index];
    }
    *error = true;
    return Blank;
}

// == Font characters defined below ==

// Defines that just map to true/false to make the designs a bit clearer
#define OO true
#define xx false

// These just serve to align the row definitions the same way they're aligned on Honeylight
#define FirstRow_____(valA, valB, valC, valD, valE) { valA, valB, valC, valD, valE }
#define SecondRow__(valA, valB, valC, valD, valE, valF) { valA, valB, valC, valD, valE, valF }
#define ThirdRow_(valA, valB, valC, valD, valE, valF, valG) { valA, valB, valC, valD, valE, valF, valG }
#define FourthRow__(valA, valB, valC, valD, valE, valF) { valA, valB, valC, valD, valE, valF }
#define FifthRow_____(valA, valB, valC, valD, valE) { valA, valB, valC, valD, valE }

Font::symbol_t const Font::Alphabet[] = {
        // A
        {
                FirstRow_____(xx, xx, OO, xx, xx),
                SecondRow__(xx, xx, OO, OO, xx, xx),
                ThirdRow_(xx, xx, OO, xx, OO, xx, xx),
                FourthRow__(xx, OO, OO, OO, OO, xx),
                FifthRow_____(OO, xx, xx, xx, OO)
        },
        // B
        {
                FirstRow_____(xx, xx, OO, OO, xx),
                SecondRow__(xx, xx, OO, xx, OO, xx),
                ThirdRow_(xx, xx, OO, OO, OO, xx, xx),
                FourthRow__(xx, OO, xx, OO, xx, xx),
                FifthRow_____(OO, OO, OO, xx, xx)
        },
        // C
        {
                FirstRow_____(xx, OO, OO, OO, xx),
                SecondRow__(xx, OO, xx, xx, xx, xx),
                ThirdRow_(xx, OO, xx, xx, xx, xx, xx),
                FourthRow__(xx, OO, xx, xx, xx, xx),
                FifthRow_____(xx, OO, OO, OO, xx)
        },
        // D
        {
                FirstRow_____(xx, xx, OO, OO, xx),
                SecondRow__(xx, xx, OO, xx, OO, xx),
                ThirdRow_(xx, xx, OO, xx, xx, OO, xx),
                FourthRow__(xx, OO, xx, xx, OO, xx),
                FifthRow_____(OO, OO, OO, OO, xx)
        },
        // E
        {
                FirstRow_____(xx, OO, OO, OO, xx),
                SecondRow__(xx, OO, xx, xx, xx, xx),
                ThirdRow_(xx, OO, OO, OO, xx, xx, xx),
                FourthRow__(xx, OO, xx, xx, xx, xx),
                FifthRow_____(xx, OO, OO, OO, xx)
        },
        // F
        {
                FirstRow_____(xx, xx, OO, OO, OO),
                SecondRow__(xx, xx, OO, xx, xx, xx),
                ThirdRow_(xx, xx, OO, OO, OO, xx, xx),
                FourthRow__(xx, OO, xx, xx, xx, xx),
                FifthRow_____(OO, xx, xx, xx, xx)
        },
        // G
        {
                FirstRow_____(xx, OO, OO, OO, xx),
                SecondRow__(xx, OO, xx, xx, xx, xx),
                ThirdRow_(xx, OO, xx, OO, OO, OO, xx),
                FourthRow__(xx, OO, xx, xx, OO, xx),
                FifthRow_____(xx, OO, OO, OO, xx)
        },
        // H
        {
                FirstRow_____(xx, OO, xx, OO, xx),
                SecondRow__(xx, OO, xx, xx, OO, xx),
                ThirdRow_(xx, OO, OO, OO, OO, OO, xx),
                FourthRow__(xx, OO, xx, xx, OO, xx),
                FifthRow_____(xx, OO, xx, OO, xx)
        },
        // I
        {
                FirstRow_____(xx, xx, OO, OO, OO),
                SecondRow__(xx, xx, xx, OO, xx, xx),
                ThirdRow_(xx, xx, xx, OO, xx, xx, xx),
                FourthRow__(xx, xx, OO, xx, xx, xx),
                FifthRow_____(OO, OO, OO, xx, xx)
        },
        // J
        {
                FirstRow_____(xx, xx, xx, xx, OO),
                SecondRow__(xx, xx, xx, xx, OO, xx),
                ThirdRow_(xx, xx, OO, xx, OO, xx, xx),
                FourthRow__(xx, OO, xx, OO, xx, xx),
                FifthRow_____(xx, OO, OO, xx, xx)
        },
        // K
        {
                FirstRow_____(xx, xx, OO, xx, OO),
                SecondRow__(xx, xx, OO, xx, OO, xx),
                ThirdRow_(xx, xx, OO, OO, OO, xx, xx),
                FourthRow__(xx, OO, xx, OO, xx, xx),
                FifthRow_____(OO, xx, xx, OO, xx)
        },
        // L
        {
                FirstRow_____(xx, xx, OO, xx, xx),
                SecondRow__(xx, xx, OO, xx, xx, xx),
                ThirdRow_(xx, xx, OO, xx, xx, xx, xx),
                FourthRow__(xx, OO, xx, xx, xx, xx),
                FifthRow_____(OO, OO, OO, xx, xx)
        },
        // M
        {
                FirstRow_____(xx, OO, xx, OO, xx),
                SecondRow__(xx, OO, OO, OO, OO, xx),
                ThirdRow_(xx, OO, xx, OO, xx, OO, xx),
                FourthRow__(xx, OO, xx, xx, OO, xx),
                FifthRow_____(xx, OO, xx, OO, xx)
        },
        // N
        {
                FirstRow_____(xx, OO, xx, OO, xx),
                SecondRow__(xx, OO, OO, xx, OO, xx),
                ThirdRow_(xx, OO, xx, OO, xx, OO, xx),
                FourthRow__(xx, OO, xx, OO, OO, xx),
                FifthRow_____(xx, OO, xx, OO, xx)
        },
        // O
        {
                FirstRow_____(xx, OO, OO, OO, xx),
                SecondRow__(xx, OO, xx, xx, OO, xx),
                ThirdRow_(xx, OO, xx, xx, xx, OO, xx),
                FourthRow__(xx, OO, xx, xx, OO, xx),
                FifthRow_____(xx, OO, OO, OO, xx)
        },
        // P
        {
                FirstRow_____(xx, xx, OO, OO, OO),
                SecondRow__(xx, xx, OO, xx, OO, xx),
                ThirdRow_(xx, xx, OO, OO, OO, xx, xx),
                FourthRow__(xx, OO, xx, xx, xx, xx),
                FifthRow_____(OO, xx, xx, xx, xx)
        },
        // Q
        {
                FirstRow_____(xx, OO, OO, OO, xx),
                SecondRow__(xx, OO, xx, xx, OO, xx),
                ThirdRow_(xx, OO, xx, xx, xx, OO, xx),
                FourthRow__(xx, OO, xx, OO, OO, xx),
                FifthRow_____(xx, OO, OO, OO, OO)
        },
        // R
        {
                FirstRow_____(xx, xx, OO, OO, OO),
                SecondRow__(xx, xx, OO, xx, OO, xx),
                ThirdRow_(xx, xx, OO, OO, OO, xx, xx),
                FourthRow__(xx, OO, OO, xx, xx, xx),
                FifthRow_____(OO, xx, OO, xx, xx)
        },
        // S
        {
                FirstRow_____(xx, OO, OO, OO, xx),
                SecondRow__(xx, OO, xx, xx, xx, xx),
                ThirdRow_(xx, xx, OO, OO, OO, xx, xx),
                FourthRow__(xx, xx, xx, xx, OO, xx),
                FifthRow_____(xx, OO, OO, OO, xx)
        },
        // T
        {
                FirstRow_____(xx, xx, OO, OO, OO),
                SecondRow__(xx, xx, xx, OO, xx, xx),
                ThirdRow_(xx, xx, xx, OO, xx, xx, xx),
                FourthRow__(xx, xx, OO, xx, xx, xx),
                FifthRow_____(xx, OO, xx, xx, xx)
        },
        // U
        {
                FirstRow_____(xx, OO, xx, OO, xx),
                SecondRow__(xx, OO, xx, xx, OO, xx),
                ThirdRow_(xx, OO, xx, xx, xx, OO, xx),
                FourthRow__(xx, OO, xx, xx, OO, xx),
                FifthRow_____(xx, OO, OO, OO, xx)
        },
        // V
        {
                FirstRow_____(OO, xx, xx, xx, OO),
                SecondRow__(xx, OO, xx, xx, OO, xx),
                ThirdRow_(xx, xx, OO, xx, OO, xx, xx),
                FourthRow__(xx, xx, OO, OO, xx, xx),
                FifthRow_____(xx, xx, OO, xx, xx)
        },
        // W
        {
                FirstRow_____(xx, OO, xx, OO, xx),
                SecondRow__(xx, OO, xx, xx, OO, xx),
                ThirdRow_(xx, OO, xx, OO, xx, OO, xx),
                FourthRow__(xx, OO, OO, OO, OO, xx),
                FifthRow_____(xx, OO, xx, OO, xx)
        },
        // X
        {
                FirstRow_____(xx, OO, xx, OO, xx),
                SecondRow__(xx, xx, OO, OO, xx, xx),
                ThirdRow_(xx, xx, xx, OO, xx, xx, xx),
                FourthRow__(xx, xx, OO, OO, xx, xx),
                FifthRow_____(xx, OO, xx, OO, xx)
        },
        // Y
        {
                FirstRow_____(xx, OO, xx, OO, xx),
                SecondRow__(xx, xx, OO, OO, xx, xx),
                ThirdRow_(xx, xx, xx, OO, xx, xx, xx),
                FourthRow__(xx, xx, OO, xx, xx, xx),
                FifthRow_____(xx, OO, xx, xx, xx)
        },
        // Z
        {
                FirstRow_____(xx, OO, OO, OO, xx),
                SecondRow__(xx, xx, xx, OO, xx, xx),
                ThirdRow_(xx, xx, xx, OO, xx, xx, xx),
                FourthRow__(xx, xx, OO, xx, xx, xx),
                FifthRow_____(xx, OO, OO, OO, xx)
        }
};

Font::symbol_t const Font::Numerals[] = {
        // 0
        {
                FirstRow_____(xx, OO, OO, OO, xx),
                SecondRow__(xx, OO, xx, OO, OO, xx),
                ThirdRow_(xx, OO, xx, OO, xx, OO, xx),
                FourthRow__(xx, OO, OO, xx, OO, xx),
                FifthRow_____(xx, OO, OO, OO, xx)
        },
        // 1
        {
                FirstRow_____(xx, xx, xx, OO, OO),
                SecondRow__(xx, xx, xx, xx, OO, xx),
                ThirdRow_(xx, xx, xx, xx, OO, xx, xx),
                FourthRow__(xx, xx, xx, OO, xx, xx),
                FifthRow_____(xx, OO, OO, OO, xx)
        },
        // 2
        {
                FirstRow_____(xx, OO, OO, xx, xx),
                SecondRow__(xx, OO, xx, OO, xx, xx),
                ThirdRow_(xx, xx, xx, OO, xx, xx, xx),
                FourthRow__(xx, xx, OO, xx, xx, xx),
                FifthRow_____(xx, OO, OO, OO, xx)
        },
        // 3
        {
                FirstRow_____(xx, OO, OO, OO, xx),
                SecondRow__(xx, OO, xx, xx, OO, xx),
                ThirdRow_(xx, xx, xx, OO, OO, xx, xx),
                FourthRow__(xx, OO, xx, xx, OO, xx),
                FifthRow_____(xx, OO, OO, OO, xx)
        },
        // 4
        {
                FirstRow_____(xx, xx, OO, xx, OO),
                SecondRow__(xx, xx, OO, xx, OO, xx),
                ThirdRow_(xx, xx, OO, OO, OO, xx, xx),
                FourthRow__(xx, xx, xx, OO, xx, xx),
                FifthRow_____(xx, xx, OO, xx, xx)
        },
        // 5
        {
                FirstRow_____(xx, xx, OO, OO, OO),
                SecondRow__(xx, xx, OO, xx, xx, xx),
                ThirdRow_(xx, xx, OO, OO, xx, xx, xx),
                FourthRow__(xx, xx, xx, OO, xx, xx),
                FifthRow_____(OO, OO, OO, xx, xx)
        },
        // 6
        {
                FirstRow_____(xx, OO, OO, OO, xx),
                SecondRow__(xx, OO, xx, xx, xx, xx),
                ThirdRow_(xx, OO, OO, OO, OO, xx, xx),
                FourthRow__(xx, OO, xx, xx, OO, xx),
                FifthRow_____(xx, OO, OO, OO, xx)
        },
        // 7
        {
                FirstRow_____(xx, OO, OO, OO, OO),
                SecondRow__(xx, xx, xx, xx, OO, xx),
                ThirdRow_(xx, xx, xx, xx, OO, xx, xx),
                FourthRow__(xx, xx, xx, OO, xx, xx),
                FifthRow_____(xx, xx, OO, xx, xx)
        },
        // 8
        {
                FirstRow_____(xx, OO, OO, OO, xx),
                SecondRow__(xx, OO, xx, xx, OO, xx),
                ThirdRow_(xx, xx, OO, OO, OO, xx, xx),
                FourthRow__(xx, OO, xx, xx, OO, xx),
                FifthRow_____(xx, OO, OO, OO, xx)
        },
        // 9
        {
                FirstRow_____(xx, OO, OO, OO, xx),
                SecondRow__(xx, OO, xx, xx, OO, xx),
                ThirdRow_(xx, xx, OO, OO, OO, xx, xx),
                FourthRow__(xx, xx, xx, OO, xx, xx),
                FifthRow_____(xx, xx, OO, xx, xx)
        }
};

Font::symbol_t const Font::Blank = {
        FirstRow_____(xx, xx, xx, xx, xx),
        SecondRow__(xx, xx, xx, xx, xx, xx),
        ThirdRow_(xx, xx, xx, xx, xx, xx, xx),
        FourthRow__(xx, xx, xx, xx, xx, xx),
        FifthRow_____(xx, xx, xx, xx, xx)
};
