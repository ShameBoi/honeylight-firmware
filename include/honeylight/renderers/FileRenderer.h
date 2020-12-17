/**
 * FileRenderer.h
 *
 * @author Shame Boi
 */
#pragma once

#include <cstdint>
#include <cstddef>

#include <honeylight/display_types.h>
#include <honeylight/data/Frame.h>
#include <honeylight/renderers/Renderer.h>

class FileRenderer : public Renderer {
private:
    uint32_t currentPatternFrame = 0;
    uint32_t currentTransitionFrame = 0;
    Frame patternFrames[HONEYLIGHT_MAX_PATTERN_FRAMES];
    size_t patternFrameCount = 0;
    Frame const * activePatternFrame = nullptr;

    bool renderFadeTransitionFrame(Frame const * fadeFromPatternFrame,
                                   Frame const * fadeToPatternFrame,
                                   DisplayBuffer * dest) const;
public:

    ~FileRenderer() override = default;

    inline void reset() {
        currentPatternFrame = 0;
        currentTransitionFrame = 0;
        activePatternFrame = nullptr;
    }

    inline bool isPatternLoaded() const {
        return patternFrameCount != 0;
    }

    inline size_t getPatternFrameCount() const {
        return patternFrameCount;
    }

    inline void setPatternFrameCount(size_t const val) {
        patternFrameCount = val;
    }

    inline Frame * getPatternFrame(uint32_t const index) {
        if (index >= HONEYLIGHT_MAX_PATTERN_FRAMES) {
            return nullptr;
        }
        return &patternFrames[index];
    }

    bool renderTo(DisplayBuffer *buffer) override;

    const char *getName() const override {
        return "File";
    }
};



