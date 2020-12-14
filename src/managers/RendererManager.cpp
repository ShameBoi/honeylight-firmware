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

void RendererManager::showRenderer(Renderer * const renderer) {
    previousRenderer = selectedRenderer;
    selectedRenderer = renderer;
    DBG("Selected Renderer: ");
    DBGLN(selectedRenderer->getName());
}

void RendererManager::showPreviousRenderer() {
    if (previousRenderer == nullptr) {
        return;
    }
    showRenderer(previousRenderer);
}

void RendererManager::setDefaultRenderer(RendererType const type) {
    switch(type) {
        case RendererType::File:
            defaultRenderer = &fileRenderer;
            break;
        case RendererType::LoadingBar:
            defaultRenderer = &loadingBarRenderer;
            break;
        case RendererType::Rainbow:
            defaultRenderer = &rainbowRenderer;
            break;
        case RendererType::BlankWhite:
            defaultRenderer = &blankWhiteRenderer;
            break;
        case RendererType::Menu:
            defaultRenderer = &menuRenderer;
            break;
        case RendererType::FontTest:
            defaultRenderer = &fontTestRenderer;
            break;
    }
}

void RendererManager::showRenderer(RendererType const type) {
    switch(type) {
        case RendererType::File:
            showFileRenderer();
            break;
        case RendererType::LoadingBar:
            showLoadingBarRenderer();
            break;
        case RendererType::Rainbow:
            showRainbowRenderer();
            break;
        case RendererType::BlankWhite:
            showBlankWhiteRenderer();
            break;
        case RendererType::Menu:
            showMenuRenderer();
            break;
        case RendererType::FontTest:
            showFontTestRenderer();
            break;
    }
}
