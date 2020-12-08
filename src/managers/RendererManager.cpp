/**
 * RendererManager.cpp
 *
 * @author Shame Boi
 */

#include <Arduino.h>
#include <SPI.h>

#include <honeylight/managers/RendererManager.h>

RendererManager::RendererManager() : display(&SPI1) {}

void RendererManager::begin() {
    display.begin();
    showBlankWhiteRenderer();
}

bool RendererManager::hasWork() {
    uint32_t const delta = millis() - lastFrameMillis;
    return delta >= MillisecondsPerFrame;
}

void RendererManager::work() {
    lastFrameMillis = millis();
    renderFrame();
    writeFrame();
}

void RendererManager::renderFrame() {
    if (selectedRenderer && !errorWithRenderer) {
        errorWithRenderer = !selectedRenderer->renderTo(display.getBuffer());
    }

    if (!selectedRenderer || errorWithRenderer) {
        if (errorWithRenderer) {
            Serial.println("Render error!");
        }
        showRainbowRenderer();
        errorWithRenderer = false;
    }
}

void RendererManager::writeFrame() {
    display.swap();
    display.write();
}

void RendererManager::showRenderer(Renderer * const renderer) {
    previousRenderer = selectedRenderer;
    selectedRenderer = renderer;
    Serial.print("Selected Renderer: ");
    Serial.println(selectedRenderer->getName());
}

void RendererManager::showPreviousRenderer() {
    if (previousRenderer == nullptr) {
        return;
    }
    showRenderer(previousRenderer);
}
