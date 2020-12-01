/**
 * Pattern.h
 *
 * @author Shame Boi
 */
#pragma once


#include <honeylight/display_types.h>

class Pattern {
public:
    Pattern() = default;

    virtual ~Pattern() = default;

    virtual bool renderTo(display_buffer_t * buffer) = 0;
};



