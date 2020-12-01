/**
 * LoadingBarPattern.cpp
 *
 * @author Shame Boi
 */

#include <honeylight/LoadingBarPattern.h>

bool LoadingBarPattern::renderTo(display_buffer_t * const buffer) {
    buffer->setAll(background);

    uint8_t const colorToCol = (16.0 * (percent / 100.0));
    color_t foregroundColor = color_t(foreground);
    for(uint8_t col = 0; col < colorToCol; col++) {
        for (uint8_t row = 0; row < 5; row++) {
            buffer->set(row, col, foregroundColor);
        }
    }

    return false;
}
