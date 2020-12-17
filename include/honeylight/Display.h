/**
 * Display.h
 *
 * @author Shame Boi
 */
#pragma once

#include <Arduino.h>
#include <SPI.h>
#include <honeylight/data/DisplayBuffer.h>

class Display {
private:
    SPIClass * const spi;

    DisplayBuffer bufferA = {};

    DisplayBuffer bufferB = {};

    DisplayBuffer * live = &bufferA;

    DisplayBuffer * standby = &bufferB;

    void startDataFrame();

public:
    explicit Display(SPIClass * const spi) : spi(spi) {}

    void begin();

    void end();

    void write();

    inline DisplayBuffer * getBuffer() {
        return standby;
    }

    inline DisplayBuffer * swap() {
        DisplayBuffer * oldLive = live;
        live = standby;
        standby = oldLive;
        return oldLive;
    }
};
