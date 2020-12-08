/**
 * Font.h
 *
 * @author Shame Boi
 */
#pragma once

#include <cstdint>

class Font {
    friend class FontTestRenderer;
private:
    struct [[gnu::packed]] symbol_t {
        struct [[gnu::packed]] row1_t {
            bool A: 1;
            bool B: 1;
            bool C: 1;
            bool D: 1;
            bool E: 1;
        } row1;

        struct [[gnu::packed]] row2_t {
            bool A: 1;
            bool B: 1;
            bool C: 1;
            bool D: 1;
            bool E: 1;
            bool F: 1;
        } row2;

        struct [[gnu::packed]] row3_t {
            bool A: 1;
            bool B: 1;
            bool C: 1;
            bool D: 1;
            bool E: 1;
            bool F: 1;
            bool G: 1;
        } row3;

        struct [[gnu::packed]] row4_t {
            bool A: 1;
            bool B: 1;
            bool C: 1;
            bool D: 1;
            bool E: 1;
            bool F: 1;
        } row4;

        struct [[gnu::packed]] row5_t {
            bool A: 1;
            bool B: 1;
            bool C: 1;
            bool D: 1;
            bool E: 1;
        } row5;
    };

    static symbol_t const Alphabet[28];
};



