/**
 * UIManager.cpp
 *
 * @author Shame Boi
 */

#include <honeylight/debug.h>
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
    filePatternsMenuLength = fileManager->getPatternCount();
    menuIndex = fileManager->getActivePatternIndex();
}

bool UIManager::hasWork() {
    // If this returns false, flag has been cleared by interrupt since we last checked
    buttonPressed = !buttonFlag.setFlag();

    // If we've gotten +- at least 4 pulses, the knob has advanced a tick
    knobMoved = (knob.read() - previousKnobPosition) / 4;

    return buttonPressed || knobMoved != 0 || hasMenuTimedOut();
}

void UIManager::work() {
    if (buttonPressed) {
        handleButtonPressed();
        return;
    }

    if (knobMoved != 0) {
        handleKnobMoved();
        return;
    }

    if (hasMenuTimedOut()) {
        handleMenuTimeout();
    }
}

void UIManager::openMenu() {
    menuActive = true;
    filePatternsMenuLength = fileManager->getPatternCount();
    switch (rendererManager->getActiveRenderer()) {
        case RendererType::File:
            menuIndex = fileManager->getActivePatternIndex();
            break;

        case RendererType::Menu:
        case RendererType::LoadingBar:
        case RendererType::FontTest:
        case RendererType::Rainbow:
        case RendererType::None:
            menuIndex = filePatternsMenuLength;
            break;

        case RendererType::BlankWhite:
            menuIndex = filePatternsMenuLength + 1;
            break;
    }
    rendererManager->getMenuRenderer().setFilePatternEntries(filePatternsMenuLength);
    rendererManager->getMenuRenderer().setBuiltInEntries(BuiltInPatternsMenuLength);
    rendererManager->getMenuRenderer().setDisplayedChar(getCharForMenuEntry());
    rendererManager->getMenuRenderer().setHighlightedEntry(menuIndex);
    rendererManager->showMenuRenderer();
}

void UIManager::reset() {
    buttonPressed = false;
    menuActive = false;
    knobMoved = 0;
    previousKnobPosition = knob.read();
    switch (rendererManager->getActiveRenderer()) {
        case RendererType::File:
            menuIndex = fileManager->getActivePatternIndex();
            break;

        case RendererType::Menu:
        case RendererType::LoadingBar:
        case RendererType::FontTest:
        case RendererType::Rainbow:
        case RendererType::None:
            menuIndex = filePatternsMenuLength;
            break;

        case RendererType::BlankWhite:
            menuIndex = filePatternsMenuLength + 1;
            break;
    }
}

void UIManager::handleButtonPressed() {
    if (menuIndex < filePatternsMenuLength) {
        fileManager->loadPattern(menuIndex);
    } else if (menuIndex < (BuiltInPatternsMenuLength + filePatternsMenuLength)) {
        switch (menuIndex - filePatternsMenuLength) {
            default:
            case 0:
                rendererManager->showRainbowRenderer();
                break;

            case 1:
                rendererManager->showBlankWhiteRenderer();
                break;
        }
    } else {
        // TODO: Handle settings menu
    }
    reset();
}

void UIManager::handleKnobMoved() {
    lastInputMillis = millis();
    if (!menuActive) {
        openMenu();
    }
    previousKnobPosition = knob.read();
    size_t const totalMenuLength = getTotalMenuLength();
    if (totalMenuLength > 1) {
        if (knobMoved > 0) {
            menuIndex = (menuIndex + knobMoved) % totalMenuLength;
        } else {
            auto const absKnobMoved = static_cast<size_t>(-knobMoved);
            if (menuIndex < absKnobMoved) {
                menuIndex = totalMenuLength - (absKnobMoved - menuIndex);
            } else {
                menuIndex -= absKnobMoved;
            }
        }
    } else {
        menuIndex = 0;
    }
    knobMoved = 0;
    rendererManager->getMenuRenderer().setHighlightedEntry(menuIndex);
    rendererManager->getMenuRenderer().setDisplayedChar(getCharForMenuEntry());
}

void UIManager::handleMenuTimeout() {
    DBGLN("Menu timeout");
    reset();
    rendererManager->showPreviousRenderer();
}

char UIManager::getCharForMenuEntry() const {
    if (menuIndex < filePatternsMenuLength) {
        return 'P';
    }

    if ((menuIndex - filePatternsMenuLength) < BuiltInPatternsMenuLength) {
        switch ((menuIndex - filePatternsMenuLength)) {
            default:
            case 0:
                return 'R';

            case 1:
                return 'W';
        }
    }

    return 'S';
}
