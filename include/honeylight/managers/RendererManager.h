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
#include <honeylight/renderers/MenuRenderer.h>
#include <honeylight/renderers/FontTestRenderer.h>

class RendererManager {
private:
    Display display;

    FileRenderer fileRenderer;
    LoadingBarRenderer loadingBarRenderer;
    RainbowRenderer rainbowRenderer;
    BlankWhiteRenderer blankWhiteRenderer;
    MenuRenderer menuRenderer;
    FontTestRenderer fontTestRenderer;

    Renderer * previousRenderer = nullptr;
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

    inline MenuRenderer & getMenuRenderer() {
        return menuRenderer;
    }

    inline void showFileRenderer() {
        fileRenderer.reset();
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

    inline void showMenuRenderer() {
        showRenderer(&menuRenderer);
    }

    inline void showFontTestRenderer() {
        showRenderer(&fontTestRenderer);
    }

    void showPreviousRenderer();

    void begin();

    bool hasWork();

    void work();
};



