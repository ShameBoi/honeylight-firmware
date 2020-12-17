/**
 * display_types.h
 *
 * @author Shame Boi
 */
#pragma once

#include <cstdint>
#include <cstddef>

#include <honeylight/config.h>

struct color_delta_t {
    int16_t brightness;
    int16_t blue;
    int16_t green;
    int16_t red;

    color_delta_t() = default;

    color_delta_t(int16_t const brightness, int16_t const red, int16_t const green, int16_t const blue)
            : brightness(brightness), blue(blue), green(green), red(red) {}

};

struct [[gnu::packed]] rgba_t {
    uint8_t red: 8;
    uint8_t green: 8;
    uint8_t blue: 8;
    uint8_t alpha: 8;

    rgba_t() = default;

    rgba_t(uint8_t const red, uint8_t const green, uint8_t const blue, uint8_t const alpha)
            : red(red),
              green(green),
              blue(blue),
              alpha(alpha) {}
};

struct [[gnu::packed]] bgra_t {
    uint8_t blue: 8;
    uint8_t green: 8;
    uint8_t red: 8;
    uint8_t alpha: 8;

    bgra_t() = default;

    bgra_t(uint8_t const blue, uint8_t const green, uint8_t const red, uint8_t const alpha)
            : blue(blue),
              green(green),
              red(red),
              alpha(alpha) {}
};

struct [[gnu::packed]] bgr_t {
    uint8_t blue: 8;
    uint8_t green: 8;
    uint8_t red: 8;

    bgr_t() = default;

    bgr_t(uint8_t const blue, uint8_t const green, uint8_t const red)
            : blue(blue),
              green(green),
              red(red) {}
};

struct [[gnu::packed]] color_t {
    unsigned brightness: 5;
    unsigned padding: 3;

    uint8_t blue: 8;
    uint8_t green: 8;
    uint8_t red: 8;

    color_t() = default;

    constexpr color_t(uint8_t const brightness, uint8_t const red, uint8_t const green, uint8_t const blue)
            : brightness(brightness > HONEYLIGHT_MAX_BRIGHTNESS ? HONEYLIGHT_MAX_BRIGHTNESS : brightness),
              padding(0b111U),
              blue(blue),
              green(green),
              red(red) {}

    constexpr color_t(uint8_t const red, uint8_t const green, uint8_t const blue)
            : color_t(HONEYLIGHT_DEFAULT_BRIGHTNESS, red, green, blue) {}

    explicit color_t(rgba_t const &rgba)
            : color_t((rgba.alpha / 255.0) * HONEYLIGHT_MAX_BRIGHTNESS, rgba.red, rgba.green, rgba.blue) {}

    color_delta_t delta(color_t const &other) const;

    color_t add(color_delta_t const &delta) const;
};

namespace Color {
    constexpr static color_t const White = color_t(0xFF, 0xFF, 0xFF);
    constexpr static color_t const Black = color_t(0x00, 0x00, 0x00);
    constexpr static color_t const Red = color_t(0xFF, 0x00, 0x00);
    constexpr static color_t const Orange = color_t(0xFF, 0x7F, 0x00);
    constexpr static color_t const Yellow = color_t(0xFF, 0xFF, 0x00);
    constexpr static color_t const Green = color_t(0x00, 0xFF, 0x00);
    constexpr static color_t const Cyan = color_t(0x00, 0xFF, 0xFF);
    constexpr static color_t const Blue = color_t(0x00, 0x00, 0xFF);
    constexpr static color_t const Violet = color_t(0x7F, 0x00, 0xFF);
    constexpr static color_t const Magenta = color_t(0xFF, 0x00, 0xFF);
}
