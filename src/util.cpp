/**
 * util.cpp
 *
 * @author Shame Boi
 */

#include <honeylight/util.h>

/**
 * Converts a color from HSV to RGB.
 *
 * @param h hue, as a number between 0 and 360
 * @param s saturation, as a number between 0 and 255
 * @param v value, as a number between 0 and 255
 */
color_t Util::hsvToRgb(uint16_t const h, uint8_t const s, uint8_t const v) {
    uint8_t const f = (h % 60) * 255 / 60;
    uint8_t const p = (255 - s) * v / 255;
    uint8_t const q = (255 - f * s / 255) * v / 255;
    uint8_t const t = (255 - (255 - f) * s / 255) * v / 255;

    switch((h / 60) % 6) {
        case 0:
            return { v, t, p };
        case 1:
            return { q, v, p };
        case 2:
            return { p, v, t };
        case 3:
            return { p, q, v };
        case 4:
            return { t, p, v };
        case 5:
        default:
            return { v, p, q };
    }
}
