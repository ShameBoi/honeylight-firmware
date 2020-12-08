/**
 * FileRenderer.h
 *
 * @author Shame Boi
 */
#pragma once

#include <cstdint>
#include <cstddef>

#include <honeylight/display_types.h>
#include <honeylight/renderers/Renderer.h>

class FileRenderer : public Renderer {
private:
    uint32_t currentPatternFrame = 0;
    uint32_t currentTransitionFrame = 0;
    frame_t patternFrames[HONEYLIGHT_MAX_PATTERN_FRAMES];
    size_t patternFrameCount = 0;
    frame_t const * activePatternFrame = nullptr;

    bool renderFadeTransitionFrame(frame_t const * fadeFromPatternFrame,
                                   frame_t const * fadeToPatternFrame,
                                   display_buffer_t * dest) const;
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

    inline frame_t * getPatternFrame(uint32_t const index) {
        if (index >= HONEYLIGHT_MAX_PATTERN_FRAMES) {
            return nullptr;
        }
        return &patternFrames[index];
    }

    bool renderTo(display_buffer_t *buffer) override;

    const char *getName() const override {
        return "File";
    }
};



