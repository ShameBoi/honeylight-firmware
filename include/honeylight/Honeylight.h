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
#include <honeylight/FilePattern.h>
#include "LoadingBarPattern.h"

class Honeylight {
private:
    Display display;
    FileManager fileManager;
    FilePattern filePattern;
    LoadingBarPattern loadingBarPattern;

    Pattern * selectedPattern = nullptr;
    display_buffer_t * activeBuffer;
    uint32_t lastFrameMillis = 0;
    uint32_t selectedPatternIndex = 0;
    bool errorWithPattern = false;

    void write();
    void writeFrame();
    void renderFrame();
    bool loadAndParsePattern();
    void renderTestPatternFrame();

public:
    Honeylight();

    void init();

    void loop();
};



