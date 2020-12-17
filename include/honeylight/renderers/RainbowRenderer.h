/**
 * RainbowRenderer.h
 *
 * @author Shame Boi
 */
#pragma once

#include <cstdint>

#include <honeylight/renderers/Renderer.h>

class RainbowRenderer : public Renderer {
public:
    RainbowRenderer() = default;

    ~RainbowRenderer() override = default;

    bool renderTo(DisplayBuffer * buffer) override;

    const char *getName() const override {
        return "Rainbow";
    }
};
