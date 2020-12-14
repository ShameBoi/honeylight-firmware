/**
 * RendererManager.h
 *
 * @author Shame Boi
 */
#pragma once

#include <cstdint>

#include <honeylight/managers/Manager.h>
#include <honeylight/Display.h>
#include <honeylight/renderers/Renderer.h>
#include <honeylight/renderers/FileRenderer.h>
#include <honeylight/renderers/LoadingBarRenderer.h>
#include <honeylight/renderers/RainbowRenderer.h>
#include <honeylight/renderers/BlankWhiteRenderer.h>
#include <honeylight/renderers/MenuRenderer.h>
#include <honeylight/renderers/FontTestRenderer.h>

enum class RendererType {
    File,
    LoadingBar,
    Rainbow,
    BlankWhite,
    Menu,
    FontTest
};

class RendererManager : public Manager {
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
    Renderer * defaultRenderer = &blankWhiteRenderer;
    uint32_t lastFrameMillis = 0;
    bool errorWithRenderer = false;

    void renderFrame();

    void writeFrame();

    void showRenderer(Renderer * renderer);

public:
    RendererManager();

    ~RendererManager() override = default;

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

    inline void showDefaultRenderer() {
        showRenderer(defaultRenderer);
    }

    void setDefaultRenderer(RendererType type);

    void showRenderer(RendererType type);

    void showPreviousRenderer();

    void begin() override;

    bool hasWork() override;

    void work() override;
};



