/**
 * FontTestRenderer.h
 *
 * @author Shame Boi
 */
#pragma once

#include <cstdint>
#include <Arduino.h>

#include <honeylight/renderers/Renderer.h>
#include <honeylight/renderers/Font.h>

class FontTestRenderer : public Renderer {
private:
    constexpr static uint32_t const TimeToShowCharMS = 1000U;

    color_t const background = Color::Black;

    color_t const foreground = Color::Violet;

    uint32_t lastMillis = 0;

    char character = 'Z';

    char numeral = '9';

public:
    FontTestRenderer() = default;

    ~FontTestRenderer() override = default;

    bool renderTo(DisplayBuffer * const buffer) override {
        if ((millis() - lastMillis) >= TimeToShowCharMS) {
            ++character;
            if (character > 'Z') {
                character = 'A';
            }

            ++numeral;
            if (numeral > '9') {
                numeral = '0';
            }
            lastMillis = millis();
        }

        buffer->setAll(background);
        if (!Font::renderCharacter(character, foreground, 0, buffer)) {
            return false;
        }

        if (!Font::renderCharacter(numeral, foreground, HONEYLIGHT_DISPLAY_COLS / 2, buffer)) {
            return false;
        }

        return true;
    }

    const char *getName() const override {
        return "Font Test";
    }
};
