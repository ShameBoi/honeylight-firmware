/**
 * Honeylight.cpp
 *
 * @author Shame Boi
 */

#include <Arduino.h>

#include <honeylight/Honeylight.h>

void Honeylight::init() {
    #ifdef HONEYLIGHT_DEBUG
    Serial.begin(115200);
    #ifdef HONEYLIGHT_STARTUP_WAIT
    uint32_t const startMillis = millis();
    while (!Serial) {
        if (millis() - startMillis > 5000U) {
            break;
        }
    }
    #endif
    #endif

    rendererManager.setDefaultRenderer(RendererType::FontTest);
    rendererManager.begin();
    fileManager.begin();
    uiManager.begin();
}

void Honeylight::loop() {
    for (Manager * manager : managers) {
        if (manager->hasWork()) {
            manager->work();
        }
    }
}
