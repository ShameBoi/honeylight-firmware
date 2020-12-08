/**
 * RendererManager.h
 *
 * @author Shame Boi
 */
#pragma once

#include <cstdint>

#include <honeylight/Display.h>
#include <honeylight/renderers/Renderer.h>
#include <honeylight/renderers/FileRenderer.h>
#include <honeylight/renderers/LoadingBarRenderer.h>
#include <honeylight/renderers/RainbowRenderer.h>
#include <honeylight/renderers/BlankWhiteRenderer.h>

class RendererManager {
private:
    Display display;

    FileRenderer fileRenderer;
    LoadingBarRenderer loadingBarRenderer;
    RainbowRenderer rainbowRenderer;
    BlankWhiteRenderer blankWhiteRenderer;

    Renderer * selectedRenderer = nullptr;
    uint32_t lastFrameMillis = 0;
    bool errorWithRenderer = false;

    void renderFrame();

    void writeFrame();

    void showRenderer(Renderer * renderer);

public:
    RendererManager();

    inline FileRenderer & getFileRenderer() {
        return fileRenderer;
    }

    inline LoadingBarRenderer & getLoadingBarRenderer() {
        return loadingBarRenderer;
    }

    inline void showFileRenderer() {
        showRenderer(&fileRenderer);
    }

    inline void showLoadingBarRenderer() {
        showRenderer(&loadingBarRenderer);
    }

    inline void showRainbowRenderer() {
        showRenderer(&rainbowRenderer);
    }

    inline void showBlankWhiteRenderer() {
        showRenderer(&blankWhiteRenderer);
    }

    void begin();

    bool hasWork();

    void work();
};



