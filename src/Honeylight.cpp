/**
 * Honeylight.cpp
 *
 * @author Shame Boi
 */

#include <Arduino.h>

#include <honeylight/Honeylight.h>

void Honeylight::init() {
    Serial.begin(115200);
    #if defined(HONEYLIGHT_STARTUP_WAIT) && defined(HONEYLIGHT_DEBUG)
    uint32_t const startMillis = millis();
    while (!Serial) {
        if (millis() - startMillis > 5000U) {
            break;
        }
    }
    #endif

    rendererManager.begin();
    fileManager.begin();
    uiManager.begin();

    rendererManager.showFontTestRenderer();
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
