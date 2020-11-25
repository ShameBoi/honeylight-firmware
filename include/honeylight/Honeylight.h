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
    display_data_t * activeColors;
    uint32_t lastFrameMillis = 0;

    FileManager fileManager;

    void write();
    void writeFrame();
    void renderFrame();

    void renderTestPatternFrame();

public:
    Honeylight();

    void init();

    void loop();
};



