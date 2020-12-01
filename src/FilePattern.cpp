/**
 * FilePattern.cpp
 *
 * @author Shame Boi
 */

#include <cstring>
#include <honeylight/FilePattern.h>

bool FilePattern::renderTo(display_buffer_t * const buffer) {
    if (activePatternFrame == nullptr) {
        activePatternFrame = getPatternFrame(currentPatternFrame);
    }

    if (!activePatternFrame->fadeNext || getPatternFrameCount() <= 1) {
        memcpy(buffer->getBuffer(),
               activePatternFrame->data.getConstBuffer(),
               display_buffer_t::length * sizeof(color_t));
    } else {
        frame_t const * nextFrame;
        if ((currentPatternFrame + 1) >= getPatternFrameCount()) {
            nextFrame = getPatternFrame(0);
        } else {
            nextFrame = getPatternFrame(currentPatternFrame + 1);
        }
        if (!renderFadeTransitionFrame(activePatternFrame, nextFrame, buffer)) {
            return false;
        }
    }

    ++currentTransitionFrame;
    if (currentTransitionFrame > activePatternFrame->transitionFrames) {
        currentTransitionFrame = 0;
        currentPatternFrame++;
        if (currentPatternFrame >= getPatternFrameCount()) {
            currentPatternFrame = 0;
        }
        activePatternFrame = getPatternFrame(currentPatternFrame);
    }
    return true;
}

bool FilePattern::renderFadeTransitionFrame(frame_t const * const fadeFromPatternFrame,
                                            frame_t const * const fadeToPatternFrame,
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
