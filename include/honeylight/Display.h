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

    display_data_t bufferA = {};

    display_data_t bufferB = {};

    display_data_t * live = &bufferA;

    display_data_t * standby = &bufferB;

    void startDataFrame();

public:
    explicit Display(SPIClass * const spi) : spi(spi) {}

    void begin();
    void end();
    void write();

    inline display_data_t * getBuffer() {
        return standby;
    }

    inline display_data_t * swap() {
        display_data_t * oldLive = live;
        live = standby;
        standby = oldLive;
        return oldLive;
    }
};
