/**
 * FileRenderer.cpp
 *
 * @author Shame Boi
 */

#include <honeylight/renderers/FileRenderer.h>

bool FileRenderer::renderTo(DisplayBuffer * const buffer) {
    if (activePatternFrame == nullptr) {
        activePatternFrame = getPatternFrame(currentPatternFrame);
    }

    if (!activePatternFrame->isFadeNext() || getPatternFrameCount() <= 1) {
        *buffer = activePatternFrame->getData();
    } else {
        Frame const * nextFrame;
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
    if (currentTransitionFrame > activePatternFrame->getTransitionFrames()) {
        currentTransitionFrame = 0;
        currentPatternFrame++;
        if (currentPatternFrame >= getPatternFrameCount()) {
            currentPatternFrame = 0;
        }
        activePatternFrame = getPatternFrame(currentPatternFrame);
    }
    return true;
}

bool FileRenderer::renderFadeTransitionFrame(Frame const * const fadeFromPatternFrame,
                                             Frame const * const fadeToPatternFrame,
                                             DisplayBuffer * const dest) const {
    float const percentFaded = static_cast<float>(currentTransitionFrame) /
                               static_cast<float>(fadeFromPatternFrame->getTransitionFrames());
    for (size_t pixelIndex = 0; pixelIndex < DisplayBuffer::Length; ++pixelIndex) {
        color_t const * const fadeFrom = fadeFromPatternFrame->getData().getConstBuffer() + pixelIndex;
        color_t const * const fadeTo = fadeToPatternFrame->getData().getConstBuffer() + pixelIndex;
        color_delta_t delta = fadeTo->delta(*fadeFrom);
        delta.brightness *= percentFaded;
        delta.red *= percentFaded;
        delta.green *= percentFaded;
        delta.blue *= percentFaded;
        dest->getBuffer()[pixelIndex] = fadeFrom->add(delta);
    }
    return true;
}
