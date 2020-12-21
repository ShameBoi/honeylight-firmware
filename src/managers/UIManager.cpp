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
    fileRenderer = &rendererManager->getFileRenderer();
    menuRenderer = &rendererManager->getMenuRenderer();
    fileMenu = &menuRenderer->getFileMenu();
    builtInMenu = &menuRenderer->getBuiltInMenu();
    attachInterrupt(digitalPinToInterrupt(Pin::EncoderSwitch), &onButtonPress, FALLING);
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
    fileMenu->init(fileManager);
    switch (rendererManager->getActiveRenderer()) {
        case RendererType::File:
            activeMenu = Menu::Type::FilePatterns;
            break;

        case RendererType::Menu:
        case RendererType::LoadingBar:
        case RendererType::FontTest:
        case RendererType::BlankWhite:
        case RendererType::Rainbow:
        case RendererType::None:
            activeMenu = Menu::Type::BuiltIn;
            break;
    }

    activeMenuPtr = getMenuPtrForActiveMenu();
    menuRenderer->setActiveMenu(activeMenu);
    menuRenderer->setDisplayedChar(activeMenuPtr->getMenuDisplayCharacter());
    rendererManager->showMenuRenderer();
}

void UIManager::reset() {
    buttonPressed = false;
    menuActive = false;
    knobMoved = 0;
    previousKnobPosition = knob.read();
}

void UIManager::handleButtonPressed() {
    switch (activeMenu) {
        case Menu::Type::FilePatterns:
        default:
            fileManager->loadPattern(fileMenu->getIndex());
            break;

        case Menu::Type::BuiltIn:
            rendererManager->showRenderer(builtInMenu->getSelectedRenderer());
            break;
    }

    reset();
}

void UIManager::handleKnobMoved() {
    lastInputMillis = millis();
    if (!menuActive) {
        openMenu();
    }
    previousKnobPosition = knob.read();
    if (knobMoved > 0) {
        while (knobMoved > 0) {
            if (activeMenuPtr->hasNext()) {
                activeMenuPtr->next();
            } else {
                do {
                    activeMenu = static_cast<Menu::Type>((static_cast<uint8_t>(activeMenu) + 1) % 2);
                    activeMenuPtr = getMenuPtrForActiveMenu();
                } while (activeMenuPtr->getSize() <= 0);
                activeMenuPtr->setIndex(0);
            }
            --knobMoved;
        }
    } else {
        auto absKnobMoved = static_cast<size_t>(-knobMoved);
        while (absKnobMoved > 0) {
            if (activeMenuPtr->hasPrevious()) {
                activeMenuPtr->previous();
            } else {
                do {
                    activeMenu = static_cast<Menu::Type>(static_cast<uint8_t>(activeMenu) == 0 ? 1 : (static_cast<uint8_t>(activeMenu) - 1));
                    activeMenuPtr = getMenuPtrForActiveMenu();
                } while (activeMenuPtr->getSize() <= 0);
                if (activeMenuPtr->getSize() > 0) {
                    activeMenuPtr->setIndex(activeMenuPtr->getSize() - 1);
                }
            }
            --absKnobMoved;
        }
    }
    knobMoved = 0;
    menuRenderer->setActiveMenu(activeMenu);
    menuRenderer->setDisplayedChar(activeMenuPtr->getMenuDisplayCharacter());
}

void UIManager::handleMenuTimeout() {
    DBGLN("Menu timeout");
    reset();
    rendererManager->showPreviousRenderer();
}
