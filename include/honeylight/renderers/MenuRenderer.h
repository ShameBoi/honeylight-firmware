/**
 * MenuRenderer.h
 *
 * @author Shame Boi
 */
#pragma once

#include <cstdint>

#include <honeylight/renderers/Renderer.h>
#include <honeylight/renderers/Font.h>

class MenuRenderer : public Renderer {
    constexpr static size_t const MenuWidth = HONEYLIGHT_DISPLAY_COLS / 2;
    constexpr static size_t const PatternMenuHeight = 2;
    constexpr static size_t const PatternMenuRow = 0;
    constexpr static size_t const BuiltInMenuHeight = 2;
    constexpr static size_t const BuiltInMenuRow = 2;

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
    color_t const FilePatternMenuItemColor = Color::Blue;
    color_t const BuiltInMenuItemColor = Color::Cyan;
    color_t const SettingsMenuItemColor = Color::Green;
    color_t const SelectedItemColor = Color::Violet;

    size_t filePatternEntries = 0;
    size_t builtInEntries = 0;
    size_t highlightedEntry = 0;
    char displayedChar = '\0';

public:
    MenuRenderer() = default;

    ~MenuRenderer() override = default;

    inline void setDisplayedChar(char const newDisplayedChar) {
        displayedChar = newDisplayedChar;
    }

    inline void setFilePatternEntries(size_t const newFilePatternEntries) {
        filePatternEntries = newFilePatternEntries;
    }

    inline void setBuiltInEntries(size_t const newBuiltInEntries) {
        builtInEntries = newBuiltInEntries;
    }

    inline void setHighlightedEntry(size_t const newHighlightedEntry) {
        highlightedEntry = newHighlightedEntry;
    }

    const char *getName() const override {
        return "Menu";
    }

    bool renderTo(DisplayBuffer * buffer) override;
};



