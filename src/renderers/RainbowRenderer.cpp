/**
 * RainbowRenderer.cpp
 *
 * @author Shame Boi
 */

#include <Arduino.h>

#include <honeylight/config.h>
#include <honeylight/util.h>
#include <honeylight/renderers/RainbowRenderer.h>

bool RainbowRenderer::renderTo(display_buffer_t *const buffer) {
    uint8_t const time = millis() >> 4U;

    for(uint16_t col = 0; col < HONEYLIGHT_DISPLAY_COLS; col++) {
        uint8_t p = time - col * 8;
        color_t color = Util::hsvToRgb((uint32_t)p * 359 / 256, 255, 255);

        for (uint8_t row = 0; row < HONEYLIGHT_DISPLAY_ROWS; row++) {
            buffer->set(row, col, color);
        }
    }
    return true;
}
