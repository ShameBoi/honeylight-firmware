/**
 * Honeylight.h
 *
 * @author Shame Boi
 */
#pragma once

#include <cstdint>
#include <cstddef>

#include <honeylight/config.h>
#include <honeylight/util.h>
#include <honeylight/display_types.h>
#include <honeylight/managers/RendererManager.h>
#include <honeylight/managers/FileManager.h>
#include <honeylight/renderers/FileRenderer.h>
#include <honeylight/renderers/LoadingBarRenderer.h>
#include <honeylight/renderers/RainbowRenderer.h>
#include <honeylight/managers/UIManager.h>

class Honeylight {
private:
    RendererManager rendererManager;
    FileManager fileManager;
    UIManager uiManager;

public:
    Honeylight() : rendererManager(), fileManager(rendererManager), uiManager(fileManager, rendererManager) {};

    void init();

    void loop();
};



