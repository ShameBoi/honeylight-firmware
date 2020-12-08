/**
 * Honeylight.cpp
 *
 * @author Shame Boi
 */

#include <Arduino.h>

#include <honeylight/Honeylight.h>

void Honeylight::init() {
    Serial.begin(115200);
    uint32_t const startMillis = millis();
    while (!Serial) {
        if (millis() - startMillis > 5000U) {
            break;
        }
    }

    rendererManager.begin();
    fileManager.begin();
    uiManager.begin();

    rendererManager.showRainbowRenderer();
}

void Honeylight::loop() {
    if (uiManager.hasWork()) {
        uiManager.work();
    }

    if (fileManager.hasWork()) {
        fileManager.work();
    }

    if (rendererManager.hasWork()) {
        rendererManager.work();
    }
}
