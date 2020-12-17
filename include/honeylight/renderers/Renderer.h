/**
 * Renderer.h
 *
 * @author Shame Boi
 */
#pragma once

#include <honeylight/data/DisplayBuffer.h>

class Renderer {
public:
    Renderer() = default;

    virtual ~Renderer() = default;

    virtual char const * getName() const = 0;

    virtual bool renderTo(DisplayBuffer * buffer) = 0;
};



