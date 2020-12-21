/**
 * MenuRenderer.h
 *
 * @author Shame Boi
 */
#pragma once

#include <cstdint>

#include <honeylight/renderers/Renderer.h>
#include <honeylight/renderers/Font.h>
#include <honeylight/menus/FileMenu.h>
#include <honeylight/menus/BuiltInMenu.h>

class MenuRenderer : public Renderer {
    constexpr static size_t const MenuWidth = HONEYLIGHT_DISPLAY_COLS / 2;
    constexpr static size_t const PatternMenuRow = 0;

    static constexpr size_t getMenuRowWidth(size_t const row) {
        switch (row) {
            case 0:
            case 4:
            default:
                return MenuWidth - 2;

            case 1:
            case 3:
                return MenuWidth - 1;

            case 2:
                return MenuWidth;
        }
    }

    color_t const BackgroundColor = Color::Black;
    Menu::Type activeMenu = Menu::Type::FilePatterns;
    FileMenu fileMenu;
    BuiltInMenu builtInMenu;

    Menu * menus[2] = {
            &fileMenu,
            &builtInMenu
    };

    char displayedChar = '\0';

public:
    MenuRenderer() = default;

    ~MenuRenderer() override = default;

    inline FileMenu & getFileMenu() {
        return fileMenu;
    }

    inline BuiltInMenu & getBuiltInMenu() {
        return builtInMenu;
    }

    inline void setActiveMenu(Menu::Type const menu) {
        activeMenu = menu;
    }

    inline void setDisplayedChar(char const newDisplayedChar) {
        displayedChar = newDisplayedChar;
    }

    const char *getName() const override {
        return "Menu";
    }

    bool renderTo(DisplayBuffer * buffer) override;
};



