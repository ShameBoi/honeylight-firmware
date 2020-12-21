/**
 * Menu.h
 *
 * @author Shame Boi
 */
#pragma once

#include <cstdint>
#include <cstddef>

#include <honeylight/display_types.h>

class Menu {
private:

public:
    enum class Type : uint8_t {
        FilePatterns = 0,
        BuiltIn = 1
    };

    Menu() = default;
    virtual ~Menu() = default;

    virtual void next() = 0;

    virtual void previous() = 0;

    virtual size_t getSize() const = 0;

    virtual size_t getIndex() const = 0;

    virtual void setIndex(size_t index) = 0;

    virtual color_t const & getMenuColor() const = 0;

    virtual char getMenuDisplayCharacter() const = 0;

    virtual color_t const & getSelectedItemColor() const {
        return Color::Violet;
    }

    virtual bool hasNext() const {
        return (getIndex() + 1) < getSize();
    };

    virtual bool hasPrevious() const {
        return getIndex() > 0;
    }

    virtual bool isPaged() const {
        return false;
    };

    virtual size_t getCurrentPage() const {
        return 0;
    };

    virtual size_t getCurrentPageIndex() const {
        return getIndex();
    };

    virtual size_t getCurrentPageSize() const {
        return getSize();
    };

    virtual size_t getTotalPages() const {
        return 1;
    };

    virtual Menu::Type getType() const = 0;
};



