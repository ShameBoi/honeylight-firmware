/**
 * Honeylight.cpp
 *
 * @author Shame Boi
 */

#include <Arduino.h>
#include <SPI.h>

#include <honeylight/Honeylight.h>

Honeylight::Honeylight() : display(&SPI1), activeBuffer(display.getBuffer()) {

}

void Honeylight::init() {
    Serial.begin(115200);
    uint32_t const startMillis = millis();
    while (!Serial) {
        if (millis() - startMillis > 5000U) {
            break;
        }
    }

    selectedPattern = &loadingBarPattern;
    display.begin();
    fileManager.begin();
    Serial.print("Found ");
    Serial.print(fileManager.getPatternCount());
    Serial.println(" patterns.");
}

void Honeylight::loop() {
    if (!errorWithPattern && !filePattern.isPatternLoaded()) {
        errorWithPattern = !loadAndParsePattern();
    }
    renderFrame();
    writeFrame();
}

void Honeylight::write() {
    activeBuffer = display.swap();
    display.write();
}

void Honeylight::writeFrame() {
    write();
    uint32_t const delta = millis() - lastFrameMillis;
    if (delta < MillisecondsPerFrame) {
        delay(MillisecondsPerFrame - delta);
    } else {
        Serial.print("Frame took too long to render: ");
        Serial.print(delta);
        Serial.println("ms");
    }
    lastFrameMillis = millis();
}

void Honeylight::renderFrame() {
    if (selectedPattern && !errorWithPattern) {
        errorWithPattern = !selectedPattern->renderTo(activeBuffer);
    }

    if (!selectedPattern || errorWithPattern) {
        renderTestPatternFrame();
    }
}

bool Honeylight::loadAndParsePattern() {
    if (!fileManager.loadPattern(selectedPatternIndex, &filePattern)) {
        return false;
    }
    selectedPattern = &filePattern;
    lastFrameMillis = millis();
    return true;
}

void Honeylight::renderTestPatternFrame() {
    uint8_t const time = millis() >> 4U;

    for(uint16_t col = 0; col < 16; col++) {
        uint8_t p = time - col * 8;
        color_t color = Util::hsvToRgb((uint32_t)p * 359 / 256, 255, 255);

        for (uint8_t row = 0; row < 5; row++) {
            activeBuffer->set(row, col, color);
        }
    }
}
