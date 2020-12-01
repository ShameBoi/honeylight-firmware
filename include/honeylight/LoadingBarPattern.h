/**
 * LoadingBarPattern.h
 *
 * @author Shame Boi
 */
#pragma once

#include <honeylight/Pattern.h>

class LoadingBarPattern : public Pattern {
private:
    rgba_t const background = {0x00, 0x00, 0x00, 0x00};
    rgba_t const foreground = { 0x7F, 0x00, 0x7F, 0xFF};
    uint8_t percent = 0;

public:
    bool renderTo(display_buffer_t *buffer) override;

    inline void setPercent(uint8_t const newPercent) {
        percent = newPercent;
    }
};
