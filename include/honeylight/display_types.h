/**
 * display_types.h
 *
 * @author Shame Boi
 */
#pragma once

#include <cstdint>
#include <cstddef>

#include <honeylight/config.h>

struct [[gnu::packed]] color_t {
    unsigned brightness: 5;
    unsigned padding: 3;

    uint8_t blue;
    uint8_t green;
    uint8_t red;

    color_t() = default;

    color_t(uint8_t const brightness, uint8_t const blue, uint8_t const green, uint8_t const red)
            : brightness(brightness > HONEYLIGHT_MAX_BRIGHTNESS ? HONEYLIGHT_MAX_BRIGHTNESS : brightness),
            padding(0b111U),
            blue(blue),
            green(green),
            red(red) {}

    color_t(uint8_t const blue, uint8_t const green, uint8_t const red)
            : brightness(HONEYLIGHT_DEFAULT_BRIGHTNESS),
              padding(0b111U),
              blue(blue),
              green(green),
              red(red) {}
};

template<size_t RowLength>
struct display_row_t {
private:
    constexpr static size_t const endIndex = RowLength - 1;
    color_t * const buffer;

public:
    constexpr static inline size_t length() {
        return RowLength;
    }

    explicit display_row_t(color_t * const buffer) : buffer(buffer) {}

    inline color_t & operator[](size_t index) {
        return buffer[index > endIndex ? endIndex : index];
    }

    inline bool set(size_t index, color_t const & value) {
        if (index > endIndex) {
            return false;
        }
        buffer[index] = value;
        return true;
    }
};

template<size_t RowLength>
struct display_reversed_row_t {
private:
    constexpr static size_t const endIndex = RowLength - 1;
    color_t * const buffer;

public:
    constexpr static inline size_t length() {
        return RowLength;
    }

    explicit display_reversed_row_t(color_t * const buffer) : buffer(buffer) {}

    inline color_t & operator[](size_t index) {
        return buffer[endIndex - (index > endIndex ? endIndex : index)];
    }

    inline bool set(size_t index, color_t const & value) {
        if (index > endIndex) {
            return false;
        }
        buffer[endIndex - index] = value;
        return true;
    }

};

struct display_data_t {
public:
    using RowA = display_row_t<14>;
    using RowB = display_reversed_row_t<15>;
    using RowC = display_row_t<16>;
    using RowD = display_reversed_row_t<15>;
    using RowE = display_row_t<14>;

    constexpr static size_t const length = RowA::length() +
                                           RowB::length() +
                                           RowC::length() +
                                           RowD::length() +
                                           RowE::length();

private:
    color_t buffer[length] = {};

    RowA rowA = RowA(buffer);
    RowB rowB = RowB(buffer + RowA::length());
    RowC rowC = RowC(buffer + (RowA::length() + RowB::length()));
    RowD rowD = RowD(buffer + (RowA::length() + RowB::length() + RowC::length()));
    RowE rowE = RowE(buffer + (RowA::length() + RowB::length() + RowC::length() + RowD::length()));

public:
    color_t & operator[](size_t index) {
        if (index >= length) {
            index = length - 1;
        }

        size_t cumulative = 0;
        if (index < RowA::length()) {
            return rowA[index];
        }

        cumulative += RowA::length();
        if (index < (cumulative + RowB::length())) {
            return rowB[index - cumulative];
        }

        cumulative += RowB::length();
        if (index < (cumulative + RowC::length())) {
            return rowC[index - cumulative];
        }

        cumulative += RowC::length();
        if (index < (cumulative + RowD::length())) {
            return rowD[index - cumulative];
        }

        cumulative += RowD::length();
        return rowE[index - cumulative];
    }

    color_t & get(size_t rowNum, size_t const index) {
        if (rowNum > 4) {
            rowNum = 4;
        }

        switch (rowNum) {
            default:
            case 0:
                return rowA[index];

            case 1:
                return rowB[index];

            case 2:
                return rowC[index];

            case 3:
                return rowD[index];

            case 4:
                return rowE[index];
        }
    }

    bool set(size_t const rowNum, size_t const index, color_t const & val) {
        if (rowNum > 4) {
            return false;
        }

        switch (rowNum) {
            default:
            case 0:
                return rowA.set(index, val);

            case 1:
                return rowB.set(index, val);

            case 2:
                return rowC.set(index, val);

            case 3:
                return rowD.set(index, val);

            case 4:
                return rowE.set(index, val);
        }
    }

    constexpr static size_t getRowLength(size_t rowNum) {
        if (rowNum > 4) {
            rowNum = 4;
        }

        switch (rowNum) {
            default:
            case 0:
                return RowA::length();

            case 1:
                return RowB::length();

            case 2:
                return RowC::length();

            case 3:
                return RowD::length();

            case 4:
                return RowE::length();
        }
    }

    color_t * getBuffer() {
        return buffer;
    }
};
