/**
 * RendererManager.cpp
 *
 * @author Shame Boi
 */

#include <Arduino.h>
#include <SPI.h>

#include <honeylight/debug.h>
#include <honeylight/managers/RendererManager.h>

RendererManager::RendererManager() : display(&SPI1) {}

void RendererManager::begin() {
    display.begin();
    showDefaultRenderer();
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
            DBGLN("Render error!");
        }
        showRainbowRenderer();
        errorWithRenderer = false;
    }
}

void RendererManager::writeFrame() {
    display.swap();
    display.write();
}

void RendererManager::showRenderer(RendererType const type, Renderer * const renderer) {
    previousRendererType = selectedRendererType;
    selectedRenderer = renderer;
    selectedRendererType = type;
    DBG("Selected Renderer: ");
    DBGLN(selectedRenderer->getName());
}

void RendererManager::showPreviousRenderer() {
    if (previousRendererType == RendererType::None) {
        return;
    }
    showRenderer(previousRendererType);
}

void RendererManager::setDefaultRenderer(RendererType const type) {
    if (type == RendererType::None) {
        return;
    }
    defaultRendererType = type;
}

void RendererManager::showRenderer(RendererType const type) {
    switch(type) {
        case RendererType::File:
            fileRenderer.reset();
            showRenderer(type, &fileRenderer);
            break;
        case RendererType::LoadingBar:
            showRenderer(type, &loadingBarRenderer);
            break;
        case RendererType::Rainbow:
            showRenderer(type, &rainbowRenderer);
            break;
        case RendererType::BlankWhite:
            showRenderer(type, &blankWhiteRenderer);
            break;
        case RendererType::Menu:
            showRenderer(type, &menuRenderer);
            break;
        case RendererType::FontTest:
            showRenderer(type, &fontTestRenderer);
            break;
        case RendererType::None:
            showDefaultRenderer();
            break;
    }
}
