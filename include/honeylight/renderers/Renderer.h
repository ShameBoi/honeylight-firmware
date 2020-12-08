/**
 * Renderer.h
 *
 * @author Shame Boi
 */
#pragma once


#include <honeylight/display_types.h>

class Renderer {
public:
    Renderer() = default;

    virtual ~Renderer() = default;

    virtual char const * getName() const = 0;

    virtual bool renderTo(display_buffer_t * buffer) = 0;
};



