/**
 * Frame.h
 *
 * @author Shame Boi
 */
#pragma once

#include <cstdint>
#include <cstddef>

#include <honeylight/data/DisplayBuffer.h>

class Frame {
private:
    uint32_t frameNumber = 0;
    uint32_t transitionFrames = 0;
    bool fadeNext = false;
    DisplayBuffer data;

public:
    Frame() = default;

    inline uint32_t getFrameNumber() const {
        return frameNumber;
    }

    inline void setFrameNumber(uint32_t const newFrameNumber) {
        frameNumber = newFrameNumber;
    }

    inline uint32_t getTransitionFrames() const {
        return transitionFrames;
    }

    inline void setTransitionFrames(uint32_t newTransitionFrames) {
        transitionFrames = newTransitionFrames;
    }

    inline bool isFadeNext() const {
        return fadeNext;
    }

    inline void setFadeNext(bool const newFadeNext) {
        fadeNext = newFadeNext;
    }

    inline DisplayBuffer const &getData() const {
        return data;
    }

    inline DisplayBuffer &getData() {
        return data;
    }

    inline void setData(DisplayBuffer const & newData) {
        data = newData;
    }
};
