/**
 * RowBuffer.h
 *
 * @author Shame Boi
 */
#pragma once

#include <cstdint>
#include <cstddef>

class Row {
private:
    color_t *const buffer;

protected:
    inline color_t & atRawIndex(size_t const index) {
        if (index >= length()) {
            return buffer[endIndex()];
        }
        return buffer[index];
    }

    inline color_t const & getRawIndex(size_t const index) const {
        if (index >= length()) {
            return buffer[endIndex()];
        }
        return buffer[index];
    }

    inline size_t endIndex() const {
        return length() - 1;
    }

    explicit Row(color_t *const buffer): buffer(buffer) {}

public:
    inline color_t &operator[](size_t const index) {
        return at(index);
    }

    inline color_t const &operator[](size_t const index) const {
        return get(index);
    }

    virtual ~Row() = default;

    virtual size_t length() const = 0;

    virtual color_t &at(size_t index) = 0;

    virtual color_t const &get(size_t index) const = 0;

    virtual bool set(size_t index, color_t const &value) = 0;
};
