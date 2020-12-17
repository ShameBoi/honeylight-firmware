/**
 * DisplayBuffer.h
 *
 * @author Shame Boi
 */
#pragma once

#include <cstdint>
#include <cstddef>

#include <honeylight/display_types.h>
#include <honeylight/data/ForwardRow.h>
#include <honeylight/data/ReverseRow.h>

class DisplayBuffer {
private:
    using RowA = ForwardRow<HONEYLIGHT_DISPLAY_COLS - 2>;
    using RowB = ReverseRow<HONEYLIGHT_DISPLAY_COLS - 1>;
    using RowC = ForwardRow<HONEYLIGHT_DISPLAY_COLS>;
    using RowD = ReverseRow<HONEYLIGHT_DISPLAY_COLS - 1>;
    using RowE = ForwardRow<HONEYLIGHT_DISPLAY_COLS - 2>;

public:
    constexpr static size_t const Length = RowA::Length +
                                           RowB::Length +
                                           RowC::Length +
                                           RowD::Length +
                                           RowE::Length;

    constexpr static size_t const LengthInBytes = Length * sizeof(color_t);

private:
    color_t buffer[Length] = {};
    RowA rowA = RowA(buffer);
    RowB rowB = RowB(buffer + RowA::Length);
    RowC rowC = RowC(buffer + (RowA::Length + RowB::Length));
    RowD rowD = RowD(buffer + (RowA::Length + RowB::Length + RowC::Length));
    RowE rowE = RowE(buffer + (RowA::Length + RowB::Length + RowC::Length + RowD::Length));

    Row * rows[HONEYLIGHT_DISPLAY_ROWS] = { &rowA, &rowB, &rowC, &rowD, &rowE };

public:
    constexpr static size_t getRowLength(size_t const rowNum) {
        if (rowNum >= HONEYLIGHT_DISPLAY_ROWS) {
            return RowE::Length;
        }

        switch (rowNum) {
            default:
            case 0:
                return RowA::Length;

            case 1:
                return RowB::Length;

            case 2:
                return RowC::Length;

            case 3:
                return RowD::Length;

            case 4:
                return RowE::Length;
        }
    }

    DisplayBuffer() = default;

    DisplayBuffer(DisplayBuffer const & other);

    inline color_t *getBuffer() {
        return buffer;
    }

    color_t const *getConstBuffer() const {
        return buffer;
    }

    inline color_t &operator[](size_t const index) {
        return at(index);
    }

    inline void setAll(rgba_t const &val) {
        setAll(color_t(val));
    }

    DisplayBuffer & operator=(DisplayBuffer const & other);

    color_t &at(size_t index);

    inline color_t &at(size_t row, size_t col) {
        return atRow(row).at(col);
    }

    Row &atRow(size_t row);

    color_t const &get(size_t index) const;

    inline color_t const &get(size_t row, size_t col) const {
        return getRow(row).get(col);
    }

    Row const &getRow(size_t row) const;

    bool set(size_t index, color_t const &val);

    bool set(size_t row, size_t col, color_t const &val);

    void setAll(color_t const &val);
};
