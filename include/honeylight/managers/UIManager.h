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
#include <honeylight/managers/FileManager.h>

class UIManager {
private:
    constexpr static uint8_t const EncoderChannel = 1;

    static UIManager *interruptContext;

    static void onButtonPress();

    FileManager *const fileManager;

    RendererManager *const rendererManager;

    atomic_flag_t volatile buttonFlag;

    QuadEncoder knob;

    int32_t previousKnobPosition = 0;

    bool buttonPressed = false;

    int32_t knobMoved = 0;

    size_t menuIndex = 0;

    size_t menuLength = 0;

public:
    explicit UIManager(FileManager &fileManager, RendererManager & rendererManager)
            : fileManager(&fileManager),
              rendererManager(&rendererManager),
              buttonFlag(true),
              knob(EncoderChannel, Pin::EncoderPhaseA, Pin::EncoderPhaseB) {}

    void begin();

    bool hasWork();

    void work();
};


