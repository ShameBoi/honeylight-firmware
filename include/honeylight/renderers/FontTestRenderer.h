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

    color_t const foreground = Color::Purple;

    uint32_t lastMillis = 0;

    size_t charIndex = 0;


    Font::symbol_t const * curSymbol = nullptr;
public:
    FontTestRenderer() = default;

    ~FontTestRenderer() override = default;

    bool renderTo(display_buffer_t * const buffer) override {
        if (curSymbol == nullptr || millis() - lastMillis > TimeToShowCharMS) {
            lastMillis = millis();
            curSymbol = &Font::Alphabet[charIndex];
            ++charIndex;
            if (charIndex >= (sizeof(Font::Alphabet) / sizeof(*Font::Alphabet))) {
                charIndex = 0;
            }
        }

        buffer->setAll(background);
        for (uint8_t row = 0; row < HONEYLIGHT_DISPLAY_ROWS; ++row) {
            switch (row) {
                default:
                case 0: {
                    buffer->set(0, 0, curSymbol->row1.A ? foreground : background);
                    buffer->set(0, 1, curSymbol->row1.B ? foreground : background);
                    buffer->set(0, 2, curSymbol->row1.C ? foreground : background);
                    buffer->set(0, 3, curSymbol->row1.D ? foreground : background);
                    buffer->set(0, 4, curSymbol->row1.E ? foreground : background);
                } break;

                case 1: {
                    buffer->set(1, 0, curSymbol->row2.A ? foreground : background);
                    buffer->set(1, 1, curSymbol->row2.B ? foreground : background);
                    buffer->set(1, 2, curSymbol->row2.C ? foreground : background);
                    buffer->set(1, 3, curSymbol->row2.D ? foreground : background);
                    buffer->set(1, 4, curSymbol->row2.E ? foreground : background);
                    buffer->set(1, 5, curSymbol->row2.F ? foreground : background);
                } break;

                case 2: {
                    buffer->set(2, 0, curSymbol->row3.A ? foreground : background);
                    buffer->set(2, 1, curSymbol->row3.B ? foreground : background);
                    buffer->set(2, 2, curSymbol->row3.C ? foreground : background);
                    buffer->set(2, 3, curSymbol->row3.D ? foreground : background);
                    buffer->set(2, 4, curSymbol->row3.E ? foreground : background);
                    buffer->set(2, 5, curSymbol->row3.F ? foreground : background);
                    buffer->set(2, 6, curSymbol->row3.G ? foreground : background);
                } break;

                case 3: {
                    buffer->set(3, 0, curSymbol->row4.A ? foreground : background);
                    buffer->set(3, 1, curSymbol->row4.B ? foreground : background);
                    buffer->set(3, 2, curSymbol->row4.C ? foreground : background);
                    buffer->set(3, 3, curSymbol->row4.D ? foreground : background);
                    buffer->set(3, 4, curSymbol->row4.E ? foreground : background);
                    buffer->set(3, 5, curSymbol->row4.F ? foreground : background);
                } break;

                case 4: {
                    buffer->set(4, 0, curSymbol->row5.A ? foreground : background);
                    buffer->set(4, 1, curSymbol->row5.B ? foreground : background);
                    buffer->set(4, 2, curSymbol->row5.C ? foreground : background);
                    buffer->set(4, 3, curSymbol->row5.D ? foreground : background);
                    buffer->set(4, 4, curSymbol->row5.E ? foreground : background);
                } break;
            }
        }

        return true;
    }

    const char *getName() const override {
        return "Font Test";
    }
};
