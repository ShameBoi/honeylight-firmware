/**
 * FileMenu.h
 *
 * @author Shame Boi
 */
#pragma once

#include <cstdint>
#include <cstddef>

#include <honeylight/display_types.h>
#include <honeylight/renderers/Renderer.h>
#include <honeylight/menus/Menu.h>

class BuiltInMenu: public Menu {
private:
    static constexpr size_t const BuiltInPatterns = 2;
    size_t index = 0;
    RendererType selectedBuiltInRenderer = RendererType::Rainbow;

    void setSelectedBuiltInRenderer();

public:
    void next() override;

    void previous() override;

    inline RendererType getSelectedRenderer() const {
        return selectedBuiltInRenderer;
    }

    char getMenuDisplayCharacter() const override {
        switch (index) {
            case 0:
                return 'R';
            case 1:
                return 'W';
            default:
                return 'E';
        }
    }

    inline size_t getSize() const override {
        return BuiltInPatterns;
    }

    inline size_t getIndex() const override {
        return index;
    }

    inline void setIndex(size_t const newIndex) override {
        index = newIndex;
        setSelectedBuiltInRenderer();
    }

    inline color_t const &getMenuColor() const override {
        return Color::Cyan;
    }

    inline Menu::Type getType() const override {
        return Menu::Type::BuiltIn;
    };
};



