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
    color_t const foreground = Color::Violet;
    uint8_t percent = 0;

public:
    bool renderTo(DisplayBuffer *buffer) override;

    inline void setPercent(uint8_t const newPercent) {
        percent = newPercent;
    }

    const char *getName() const override {
        return "Loading Bar";
    }
};
