/**
 * UIManager.h
 *
 * @author Shame Boi
 */
#pragma once

#include <cstdint>
#include <Arduino.h>
#include <QuadEncoder.h>

#include <honeylight/Pins.h>
#include <honeylight/atomic.h>
#include <honeylight/managers/Manager.h>
#include <honeylight/managers/FileManager.h>
#include <honeylight/menus/FileMenu.h>
#include <honeylight/menus/BuiltInMenu.h>

class UIManager : public Manager {
private:
    constexpr static uint8_t const EncoderChannel = 1;
    constexpr static uint32_t const InputTimeoutMS = 5000U;

    static UIManager *interruptContext;

    static void onButtonPress();

    FileManager *const fileManager;

    RendererManager *const rendererManager;

    FileRenderer * fileRenderer = nullptr;

    MenuRenderer * menuRenderer = nullptr;

    FileMenu * fileMenu = nullptr;

    BuiltInMenu * builtInMenu = nullptr;

    Menu * activeMenuPtr = nullptr;

    Menu::Type activeMenu = Menu::Type::FilePatterns;

    atomic_flag_t volatile buttonFlag;

    QuadEncoder knob;

    int32_t previousKnobPosition = 0;

    bool menuActive = false;

    bool buttonPressed = false;

    int32_t knobMoved = 0;

    uint32_t lastInputMillis = 0;

    void openMenu();

    void reset();

    inline bool hasMenuTimedOut() const {
        return menuActive && (millis() - lastInputMillis) > InputTimeoutMS;
    }

    inline Menu * getMenuPtrForActiveMenu() {
        switch (activeMenu) {
            case Menu::Type::FilePatterns:
                return fileMenu;

            default:
            case Menu::Type::BuiltIn:
                return builtInMenu;
        }
    }

    void handleButtonPressed();

    void handleKnobMoved();

    void handleMenuTimeout();

public:
    UIManager(FileManager &fileManager, RendererManager & rendererManager)
            : fileManager(&fileManager),
              rendererManager(&rendererManager),
              buttonFlag(true),
              knob(EncoderChannel, Pin::EncoderPhaseA, Pin::EncoderPhaseB) {}

    ~UIManager() override = default;

    void begin() override;

    bool hasWork() override;

    void work() override;
};



