/**
 * Honeylight.h
 *
 * @author Shame Boi
 */
#pragma once

#include <cstdint>
#include <cstddef>

#include <honeylight/config.h>
#include <honeylight/util.h>
#include <honeylight/display_types.h>
#include <honeylight/Display.h>
#include <honeylight/FileManager.h>

class Honeylight {
private:
    Display display;
    display_buffer_t * activeColors;
    uint32_t lastFrameMillis = 0;
    uint32_t selectedPatternIndex = 0;
    uint32_t currentPatternFrame = 0;
    uint32_t currentTransitionFrame = 0;

    frame_t const * activePatternFrame = nullptr;

    bool errorWithPattern = false;

    FileManager fileManager;

    void write();
    void writeFrame();
    void renderFrame();
    bool loadAndParsePattern();

    bool renderPatternFrame();

    bool renderFadeTransitionFrame(frame_t const * fadeFromPatternFrame,
                                   frame_t const * fadeToPatternFrame,
                                   display_buffer_t * dest) const;

    void renderTestPatternFrame();

public:
    Honeylight();

    void init();

    void loop();
};



