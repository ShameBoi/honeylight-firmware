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

class UIManager : public Manager {
private:
    constexpr static uint8_t const EncoderChannel = 1;
    constexpr static uint32_t const InputTimeoutMS = 5000U;
    constexpr static size_t const BuiltInPatternsMenuLength = 2;
    constexpr static size_t const SettingsMenuLength = 0;

    static UIManager *interruptContext;

    static void onButtonPress();

    FileManager *const fileManager;

    RendererManager *const rendererManager;

    atomic_flag_t volatile buttonFlag;

    QuadEncoder knob;

    int32_t previousKnobPosition = 0;

    bool menuActive = false;

    bool buttonPressed = false;

    int32_t knobMoved = 0;

    size_t menuIndex = 0;

    size_t filePatternsMenuLength = 0;

    uint32_t lastInputMillis = 0;

    enum class MenuSection {
        FilePatterns,
        BuiltIn,
        Settings
    } menuSection = MenuSection::FilePatterns;

    void openMenu();

    void reset();

    inline bool hasMenuTimedOut() const {
        return menuActive && (millis() - lastInputMillis) > InputTimeoutMS;
    }

    void handleButtonPressed();

    void handleKnobMoved();

    void handleMenuTimeout();

    char getCharForMenuEntry() const;

    inline size_t getTotalMenuLength() const {
        return filePatternsMenuLength + BuiltInPatternsMenuLength + SettingsMenuLength;
    }

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



