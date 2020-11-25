/**
 * Honeylight.cpp
 *
 * @author Shame Boi
 */

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

#include <honeylight/Honeylight.h>

Honeylight::Honeylight() : display(&SPI1), activeColors(display.getBuffer()) {

}

void Honeylight::init() {
    display.begin();
    fileManager.begin();
}

void Honeylight::loop() {
    renderFrame();
    writeFrame();
}

void Honeylight::write() {
    activeColors = display.swap();
    display.write();
}

void Honeylight::writeFrame() {
    write();
    uint32_t const delta = millis() - lastFrameMillis;
    if (delta < MillisecondsPerFrame) {
        delay(MillisecondsPerFrame - delta);
    }
    lastFrameMillis = millis();
}

void Honeylight::renderFrame() {
    renderTestPatternFrame();
}

void Honeylight::renderTestPatternFrame() {
    uint8_t const time = millis() >> 4U;

    for(uint16_t col = 0; col < 16; col++) {
        uint8_t p = time - col * 8;
        color_t color = hsvToRgb((uint32_t)p * 359 / 256, 255, 255);

        for (uint8_t row = 0; row < 5; row++) {
            activeColors->set(row, col, color);
        }
    }
}
