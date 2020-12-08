/**
 * LoadingBarRenderer.cpp
 *
 * @author Shame Boi
 */

#include <honeylight/renderers/LoadingBarRenderer.h>

bool LoadingBarRenderer::renderTo(display_buffer_t * const buffer) {
    buffer->setAll(background);

    uint8_t const colorToCol = (HONEYLIGHT_DISPLAY_COLS * (percent / 100.0));
    for(uint8_t col = 0; col < colorToCol; col++) {
        for (uint8_t row = 0; row < HONEYLIGHT_DISPLAY_ROWS; row++) {
            buffer->set(row, col, foreground);
        }
    }

    return true;
}
