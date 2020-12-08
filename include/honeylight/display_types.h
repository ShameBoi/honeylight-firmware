/**
 * display_types.h
 *
 * @author Shame Boi
 */
#pragma once

#include <cstdint>
#include <cstddef>

#include <honeylight/config.h>

struct color_delta_t {
    int16_t brightness;
    int16_t blue;
    int16_t green;
    int16_t red;

    color_delta_t() = default;

    color_delta_t(int16_t const brightness, int16_t const red, int16_t const green, int16_t const blue)
            : brightness(brightness), blue(blue), green(green), red(red) {}

};


struct [[gnu::packed]] rgba_t {
    uint8_t red: 8;
    uint8_t green: 8;
    uint8_t blue: 8;
    uint8_t alpha: 8;
};

struct [[gnu::packed]] bgra_t {
    uint8_t blue: 8;
    uint8_t green: 8;
    uint8_t red: 8;
    uint8_t alpha: 8;
};

struct [[gnu::packed]] bgr_t {
    uint8_t blue: 8;
    uint8_t green: 8;
    uint8_t red: 8;
};

struct [[gnu::packed]] color_t {
    unsigned brightness: 5;
    unsigned padding: 3;

    uint8_t blue;
    uint8_t green;
    uint8_t red;

    color_t() = default;

    constexpr color_t(uint8_t const brightness, uint8_t const red, uint8_t const green, uint8_t const blue)
            : brightness(brightness > HONEYLIGHT_MAX_BRIGHTNESS ? HONEYLIGHT_MAX_BRIGHTNESS : brightness),
              padding(0b111U),
              blue(blue),
              green(green),
              red(red) {}

    constexpr color_t(uint8_t const red, uint8_t const green, uint8_t const blue)
            : color_t(HONEYLIGHT_DEFAULT_BRIGHTNESS, red, green, blue) {}

    explicit color_t(rgba_t const & rgba)
        : color_t((rgba.alpha / 255.0) * HONEYLIGHT_MAX_BRIGHTNESS, rgba.red, rgba.green, rgba.blue) {}

    color_delta_t delta(color_t const &other) const;

    color_t add(color_delta_t const &delta) const;
};

template<size_t RowLength>
struct display_row_t {
private:
    constexpr static size_t const endIndex = RowLength - 1;
    color_t *const buffer;

public:
    constexpr static inline size_t length() {
        return RowLength;
    }

    explicit display_row_t(color_t *const buffer) : buffer(buffer) {}

    inline color_t &operator[](size_t index) {
        return buffer[index > endIndex ? endIndex : index];
    }

    inline bool set(size_t index, color_t const &value) {
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
    color_t *const buffer;

public:
    constexpr static inline size_t length() {
        return RowLength;
    }

    explicit display_reversed_row_t(color_t *const buffer) : buffer(buffer) {}

    inline color_t &operator[](size_t index) {
        return buffer[endIndex - (index > endIndex ? endIndex : index)];
    }

    inline bool set(size_t index, color_t const &value) {
        if (index > endIndex) {
            return false;
        }
        buffer[endIndex - index] = value;
        return true;
    }

};

struct display_buffer_t {
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
    inline color_t &operator[](size_t const index) {
        return get(index);
    }

    color_t &get(size_t index) {
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

    color_t &get(size_t row, size_t const col) {
        if (row > 4) {
            row = 4;
        }

        switch (row) {
            default:
            case 0:
                return rowA[col];

            case 1:
                return rowB[col];

            case 2:
                return rowC[col];

            case 3:
                return rowD[col];

            case 4:
                return rowE[col];
        }
    }

    bool set(size_t index, color_t const &color) {
        if (index >= length) {
            return false;
        }
        get(index) = color;
        return true;
    }

    bool set(size_t const row, size_t const col, color_t const &val) {
        if (row > 4) {
            return false;
        }

        switch (row) {
            default:
            case 0:
                return rowA.set(col, val);

            case 1:
                return rowB.set(col, val);

            case 2:
                return rowC.set(col, val);

            case 3:
                return rowD.set(col, val);

            case 4:
                return rowE.set(col, val);
        }
    }

    void setAll(rgba_t const &val) {
        setAll(color_t(val));
    }

    void setAll(color_t const &val) {
        for(auto & iter : buffer) {
            iter = val;
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

    inline color_t *getBuffer() {
        return buffer;
    }

    color_t const *getConstBuffer() const {
        return buffer;
    }
};

struct frame_t {
    uint32_t frameNumber = 0;
    uint32_t transitionFrames = 0;
    bool fadeNext = false;
    display_buffer_t data;
};

namespace Color {
    constexpr static color_t const White = color_t(0xFF, 0xFF, 0xFF);
    constexpr static color_t const Black = color_t(0x00, 0x00, 0x00);
    constexpr static color_t const Red = color_t(0xFF, 0x00, 0x00);
    constexpr static color_t const Green = color_t(0x00, 0xFF, 0x00);
    constexpr static color_t const Blue = color_t(0x00, 0x00, 0xFF);
    constexpr static color_t const Purple = color_t(0x7F, 0x00, 0x7F);
}
