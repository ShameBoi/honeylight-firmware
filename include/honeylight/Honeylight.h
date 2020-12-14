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
#include <honeylight/managers/Manager.h>
#include <honeylight/managers/RendererManager.h>
#include <honeylight/managers/FileManager.h>
#include <honeylight/managers/UIManager.h>

class Honeylight {
private:
    enum class State {
        Error,
        Idle
    };

    RendererManager rendererManager;
    FileManager fileManager;
    UIManager uiManager;
    Manager * const managers[3];

    State state = State::Error;

public:
    Honeylight()
            : rendererManager(),
              fileManager(rendererManager),
              uiManager(fileManager, rendererManager),
              managers{&uiManager, &fileManager, &rendererManager} {};

    void init();

    void loop();
};



