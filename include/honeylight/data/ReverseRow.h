/**
 * ReverseRow.h
 *
 * @author Shame Boi
 */
#pragma once

#include <cstdint>
#include <cstddef>

#include <honeylight/data/Row.h>

template<size_t RowLength>
struct ReverseRow : public Row {
public:
    constexpr static size_t const Length = RowLength;

    explicit ReverseRow(color_t *const buffer) : Row(buffer) {}

    ~ReverseRow() override = default;

    inline size_t length() const override {
        return RowLength;
    }

    inline color_t &at(size_t const index) override {
        return atRawIndex(index >= endIndex() ? 0 : endIndex() - index);
    }

    inline color_t const &get(size_t index) const override {
        return getRawIndex(index >= endIndex() ? 0 : endIndex() - index);
    }

    inline bool set(size_t const index, color_t const &value) override {
        if (index > endIndex()) {
            return false;
        }
        atRawIndex(endIndex() - index) = value;
        return true;
    }
};
