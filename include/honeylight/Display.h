/**
 * Display.h
 *
 * @author Shame Boi
 */
#pragma once

#include <Arduino.h>
#include <SPI.h>
#include <honeylight/display_types.h>

class Display {
private:
    SPIClass * const spi;

    display_buffer_t bufferA = {};

    display_buffer_t bufferB = {};

    display_buffer_t * live = &bufferA;

    display_buffer_t * standby = &bufferB;

    void startDataFrame();

public:
    explicit Display(SPIClass * const spi) : spi(spi) {}

    void begin();
    void end();
    void write();

    inline display_buffer_t * getBuffer() {
        return standby;
    }

    inline display_buffer_t * swap() {
        display_buffer_t * oldLive = live;
        live = standby;
        standby = oldLive;
        return oldLive;
    }
};
