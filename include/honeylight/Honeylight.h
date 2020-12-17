/**
 * Honeylight.h
 *
 * @author Shame Boi
 */
#pragma once

#include <cstdint>
#include <cstddef>

#include <honeylight/config.h>
#include <honeylight/managers/Manager.h>
#include <honeylight/managers/RendererManager.h>
#include <honeylight/managers/FileManager.h>
#include <honeylight/managers/UIManager.h>

class Honeylight {
private:
    RendererManager rendererManager;
    FileManager fileManager;
    UIManager uiManager;
    Manager * const managers[3];

public:
    Honeylight()
            : rendererManager(),
              fileManager(rendererManager),
              uiManager(fileManager, rendererManager),
              managers{&uiManager, &fileManager, &rendererManager} {};

    void init();

    void loop();
};



