/**
 * DisplayBuffer.cpp.c
 *
 * @author Shame Boi
 */

#include <cstring>
#include <honeylight/data/DisplayBuffer.h>

DisplayBuffer::DisplayBuffer(DisplayBuffer const &other) {
    memcpy(buffer, other.buffer, LengthInBytes);
}

DisplayBuffer &DisplayBuffer::operator=(DisplayBuffer const &other) {
    if (&other != this) {
        memcpy(buffer, other.buffer, LengthInBytes);
    }
    return *this;
}

color_t &DisplayBuffer::at(size_t index) {
    if (index >= Length) {
        index = Length - 1;
    }

    size_t cumulative = 0;
    for (Row * const curRow : rows) {
        if (index < (cumulative + curRow->length())) {
            return curRow->at(index - cumulative);
        }
        cumulative += curRow->length();
    }

    return rows[HONEYLIGHT_DISPLAY_ROWS - 1]->at(rows[HONEYLIGHT_DISPLAY_ROWS - 1]->length() - 1);
}


Row &DisplayBuffer::atRow(size_t row) {
    if (row >= HONEYLIGHT_DISPLAY_ROWS) {
        return *rows[HONEYLIGHT_DISPLAY_ROWS - 1];
    }

    return *rows[row];
}

color_t const &DisplayBuffer::get(size_t index) const {
    if (index >= Length) {
        index = Length - 1;
    }

    size_t cumulative = 0;
    for (Row * const curRow : rows) {
        if (index < (cumulative + curRow->length())) {
            return curRow->at(index - cumulative);
        }
        cumulative += curRow->length();
    }
    return rows[HONEYLIGHT_DISPLAY_ROWS - 1]->at(getRowLength(HONEYLIGHT_DISPLAY_ROWS - 1) - 1);
}

Row const &DisplayBuffer::getRow(size_t const row) const {
    if (row >= HONEYLIGHT_DISPLAY_ROWS) {
        return *(rows[HONEYLIGHT_DISPLAY_ROWS - 1]);
    }

    return *(rows[row]);
}

bool DisplayBuffer::set(size_t const index, color_t const &val) {
    if (index >= Length) {
        return false;
    }
    at(index) = val;
    return true;
}

bool DisplayBuffer::set(size_t const row, size_t const col, color_t const &val) {
    if (row >= HONEYLIGHT_DISPLAY_ROWS) {
        return false;
    }
    if (col >= getRowLength(row)) {
        return false;
    }
    at(row, col) = val;
    return true;
}

void DisplayBuffer::setAll(color_t const &val) {
    for (auto &iter : buffer) {
        iter = val;
    }
}
