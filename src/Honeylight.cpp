/**
 * Honeylight.cpp
 *
 * @author Shame Boi
 */

#include <Arduino.h>
#include <SPI.h>

#include <honeylight/Honeylight.h>

Honeylight::Honeylight() : display(&SPI1), activeColors(display.getBuffer()) {

}

void Honeylight::init() {
    Serial.begin(115200);
    uint32_t const startMillis = millis();
    while (!Serial) {
        if (millis() - startMillis > 5000U) {
            break;
        }
    }

    display.begin();
    fileManager.begin();
    Serial.print("Found ");
    Serial.print(fileManager.getPatternCount());
    Serial.println(" patterns.");
}

void Honeylight::loop() {
    if (!errorWithPattern && !fileManager.isPatternLoaded()) {
        errorWithPattern = !loadAndParsePattern();
    }
    renderFrame();
    writeFrame();
}

void Honeylight::write() {
    activeColors = display.swap();
    display.write();
}

void Honeylight::writeFrame() {
    write();
    uint32_t const delta = millis() - lastFrameMillis;
    if (delta < MillisecondsPerFrame) {
        delay(MillisecondsPerFrame - delta);
    } else {
        Serial.print("Frame took too long to render: ");
        Serial.print(delta);
        Serial.println("ms");
    }
    lastFrameMillis = millis();
}

void Honeylight::renderFrame() {
    if (!errorWithPattern) {
        errorWithPattern = !renderPatternFrame();
    }

    if (errorWithPattern) {
        renderTestPatternFrame();
    }
}

bool Honeylight::loadAndParsePattern() {
    if (!fileManager.loadPattern(selectedPatternIndex)) {
        return false;
    }
    if (!fileManager.parsePattern()) {
        return false;
    }
    lastFrameMillis = millis();
    return true;
}

bool Honeylight::renderPatternFrame() {
    if (activePatternFrame == nullptr) {
        activePatternFrame = fileManager.getPatternFrame(currentPatternFrame);
    }


    if (!activePatternFrame->fadeNext || fileManager.getPatternFrameCount() <= 1) {
        memcpy(display.getBuffer(),
               activePatternFrame->data.getConstBuffer(),
               display_buffer_t::length * sizeof(color_t));
    } else {
        frame_t const * nextFrame;
        if ((currentPatternFrame + 1) >= fileManager.getPatternFrameCount()) {
            nextFrame = fileManager.getPatternFrame(0);
        } else {
            nextFrame = fileManager.getPatternFrame(currentPatternFrame + 1);
        }
        if (!renderFadeTransitionFrame(activePatternFrame, nextFrame, display.getBuffer())) {
            return false;
        }
    }

    ++currentTransitionFrame;
    if (currentTransitionFrame > activePatternFrame->transitionFrames) {
        currentTransitionFrame = 0;
        currentPatternFrame++;
        if (currentPatternFrame >= fileManager.getPatternFrameCount()) {
            currentPatternFrame = 0;
        }
        activePatternFrame = fileManager.getPatternFrame(currentPatternFrame);
    }
    return true;
}

bool Honeylight::renderFadeTransitionFrame(frame_t const * fadeFromPatternFrame,
                                           frame_t const * fadeToPatternFrame,
                                           display_buffer_t * const dest) const {
    float const percentFaded = static_cast<float>(currentTransitionFrame) /
            static_cast<float>(fadeFromPatternFrame->transitionFrames);
    for (size_t pixelIndex = 0; pixelIndex < display_buffer_t::length; ++pixelIndex) {
        color_t const * fadeFrom = fadeFromPatternFrame->data.getConstBuffer() + pixelIndex;
        color_t const * fadeTo = fadeToPatternFrame->data.getConstBuffer() + pixelIndex;
        color_delta_t delta = fadeTo->delta(*fadeFrom);
        delta.brightness *= percentFaded;
        delta.red *= percentFaded;
        delta.green *= percentFaded;
        delta.blue *= percentFaded;
        dest->getBuffer()[pixelIndex] = fadeFrom->add(delta);
    }
    return true;
}

void Honeylight::renderTestPatternFrame() {
    uint8_t const time = millis() >> 4U;

    for(uint16_t col = 0; col < 16; col++) {
        uint8_t p = time - col * 8;
        color_t color = Util::hsvToRgb((uint32_t)p * 359 / 256, 255, 255);

        for (uint8_t row = 0; row < 5; row++) {
            activeColors->set(row, col, color);
        }
    }
}
