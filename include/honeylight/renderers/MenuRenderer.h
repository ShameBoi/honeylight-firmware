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
    color_t const BackgroundColor = Color::Black;
    color_t const MenuItemColor = Color::Blue;
    color_t const SelectedItemColor = Color::Purple;

    size_t totalEntries = 0;
    size_t highlightedEntry = 0;
public:
    MenuRenderer() = default;

    ~MenuRenderer() override = default;

    inline void setTotalEntries(size_t const newTotalEntries) {
        totalEntries = newTotalEntries;
    }

    inline void setHighlightedEntry(size_t const newHighlightedEntry) {
        highlightedEntry = newHighlightedEntry;
    }

    const char *getName() const override {
        return "Menu";
    }

    bool renderTo(display_buffer_t * buffer) override;
};



