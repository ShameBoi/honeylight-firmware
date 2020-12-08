/**
 * LoadingBarRenderer.h
 *
 * @author Shame Boi
 */
#pragma once

#include <honeylight/renderers/Renderer.h>

class LoadingBarRenderer : public Renderer {
private:
    color_t const background = Color::Black;
    color_t const foreground = Color::Purple;
    uint8_t percent = 0;

public:
    bool renderTo(display_buffer_t *buffer) override;

    inline void setPercent(uint8_t const newPercent) {
        percent = newPercent;
    }

    const char *getName() const override {
        return "Loading Bar";
    }
};
