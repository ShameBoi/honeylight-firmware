/**
 * BlankWhiteRenderer.h
 *
 * @author Shame Boi
 */
#pragma once


#include <cstdint>

#include <honeylight/renderers/Renderer.h>

class BlankWhiteRenderer : public Renderer {
public:
    BlankWhiteRenderer() = default;

    ~BlankWhiteRenderer() override = default;

    bool renderTo(DisplayBuffer * buffer) override;

    const char *getName() const override {
        return "Blank White";
    }
};



