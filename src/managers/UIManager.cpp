/**
 * UIManager.cpp
 *
 * @author Shame Boi
 */

#include <honeylight/managers/UIManager.h>

UIManager * UIManager::interruptContext = nullptr;

void UIManager::onButtonPress() {
    if (interruptContext) {
        interruptContext->buttonFlag.clearFlag();
    }
}

void UIManager::begin() {
    knob.setInitConfig();
    knob.EncConfig.enableReverseDirection = true;
    knob.init();
    pinMode(Pin::EncoderSwitch, INPUT);
    interruptContext = this;
    attachInterrupt(digitalPinToInterrupt(Pin::EncoderSwitch), &onButtonPress, FALLING);
    menuLength = fileManager->getPatternCount();
    menuIndex = fileManager->getActivePatternIndex();
}

bool UIManager::hasWork() {
    // If this returns false, flag has been cleared by interrupt since we last checked
    buttonPressed = !buttonFlag.setFlag();

    // If we've gotten +- at least 4 pulses, the knob has advanced a tick
    knobMoved = (knob.read() - previousKnobPosition) / 4;

    return buttonPressed || knobMoved != 0;
}

void UIManager::work() {
    if (buttonPressed) {
        buttonPressed = false;
        fileManager->loadPattern(menuIndex);
    }

    if (knobMoved != 0) {
        previousKnobPosition = knob.read();
        if (menuLength > 1) {
            if (knobMoved > 0) {
                menuIndex = (menuIndex + knobMoved) % menuLength;
            } else {
                auto const absKnobMoved = static_cast<size_t>(-knobMoved);
                if (menuIndex < absKnobMoved) {
                    menuIndex = menuLength - (absKnobMoved - menuIndex);
                } else {
                    menuIndex -= absKnobMoved;
                }
            }
        } else {
            menuIndex = 0;
        }
        knobMoved = 0;
    }
}
