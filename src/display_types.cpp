/**
 * display_types.cpp
 *
 * @author Shame Boi
 */

#include <honeylight/util.h>
#include <honeylight/display_types.h>

color_delta_t color_t::delta(color_t const &other) const {
    return color_delta_t(
            static_cast<int16_t>(static_cast<int16_t>(brightness) - static_cast<int16_t>(other.brightness)),
            static_cast<int16_t>(static_cast<int16_t>(red) - static_cast<int16_t>(other.red)),
            static_cast<int16_t>(static_cast<int16_t>(green) - static_cast<int16_t>(other.green)),
            static_cast<int16_t>(static_cast<int16_t>(blue) - static_cast<int16_t>(other.blue))
    );
}

color_t color_t::add(color_delta_t const &delta) const {
    return color_t(Util::addAndSetNegativeToZero(brightness, delta.brightness),
                   Util::addAndSetNegativeToZero(red, delta.red),
                   Util::addAndSetNegativeToZero(green, delta.green),
                   Util::addAndSetNegativeToZero(blue, delta.blue));
}


