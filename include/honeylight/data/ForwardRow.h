/**
 * ForwardRow.h
 *
 * @author Shame Boi
 */
#pragma once

#include <cstdint>
#include <cstddef>

#include <honeylight/data/Row.h>

template<size_t RowLength>
struct ForwardRow : public Row {
public:
    constexpr static size_t const Length = RowLength;

    explicit ForwardRow(color_t *const buffer) : Row(buffer) {}

    ~ForwardRow() override = default;

    size_t length() const override {
        return RowLength;
    }

    inline color_t &at(size_t const index) override {
        return atRawIndex(index);
    }

    inline color_t const &get(size_t index) const override {
        return getRawIndex(index);
    }

    inline bool set(size_t const index, color_t const &value) override {
        if (index > endIndex()) {
            return false;
        }
        atRawIndex(index) = value;
        return true;
    }
};
