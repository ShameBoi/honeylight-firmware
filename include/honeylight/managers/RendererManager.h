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

class RendererManager : public Manager {
private:
    Display display;

    FileRenderer fileRenderer;
    LoadingBarRenderer loadingBarRenderer;
    RainbowRenderer rainbowRenderer;
    BlankWhiteRenderer blankWhiteRenderer;
    MenuRenderer menuRenderer;
    FontTestRenderer fontTestRenderer;

    Renderer * selectedRenderer = nullptr;
    RendererType previousRendererType = RendererType::None;
    RendererType selectedRendererType = RendererType::None;
    RendererType defaultRendererType = RendererType::BlankWhite;
    uint32_t lastFrameMillis = 0;
    bool errorWithRenderer = false;

    void renderFrame();

    void writeFrame();

    void showRenderer(RendererType type, Renderer * renderer);

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
        showRenderer(RendererType::File);
    }

    inline void showLoadingBarRenderer() {
        showRenderer(RendererType::LoadingBar);
    }

    inline void showRainbowRenderer() {
        showRenderer(RendererType::Rainbow);
    }

    inline void showBlankWhiteRenderer() {
        showRenderer(RendererType::BlankWhite);
    }

    inline void showMenuRenderer() {
        showRenderer(RendererType::Menu);
    }

    inline void showFontTestRenderer() {
        showRenderer(RendererType::FontTest);
    }

    inline void showDefaultRenderer() {
        showRenderer(defaultRendererType);
    }

    RendererType getActiveRenderer() const {
        return selectedRendererType;
    }

    RendererType getPreviousRenderer() const {
        return selectedRendererType;
    }

    void setDefaultRenderer(RendererType type);

    void showRenderer(RendererType type);

    void showPreviousRenderer();

    void begin() override;

    bool hasWork() override;

    void work() override;
};



