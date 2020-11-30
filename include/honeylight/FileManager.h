/**
 * FileManager.h
 *
 * @author Shame Boi
 */
#pragma once

#include <cstdint>
#include <cstddef>

#include <Arduino.h>
#include <SD.h>

#include <honeylight/config.h>
#include <honeylight/display_types.h>

class FileManager {
private:
    constexpr static char const * const PATTERN_DIRECTORY = "/patterns";
    constexpr static char const * const BMP_EXTENSION = ".BMP";
    constexpr static size_t const COLUMN_SPACING = 6;
    constexpr static size_t const ROW_SPACING = 6;

    File root;
    File currentPattern;
    size_t patternIndex = 0;
    frame_t patternFrames[HONEYLIGHT_MAX_PATTERN_FRAMES];
    size_t patternFrameCount = 0;
    rgba_t decodedFileBuff[HONEYLIGHT_IMAGE_BUFFER_SIZE / sizeof(rgba_t)] = {};

    static bool hasExtension(File & file, char const * extension);

    constexpr static uint8_t getXStartForRow(uint8_t const rowNumber) {
        switch (rowNumber) {
            default:
            case 0:
            case 4:
                return 8;

            case 1:
            case 3:
                return 5;

            case 2:
                return 2;
        }
    }

    bool parseFrame(File & entry, frame_t * dest);

    bool processPossibleFrameFile(File & file);

public:
    FileManager();

    void begin();

    inline bool isPatternLoaded() const {
        return patternFrameCount != 0;
    }

    inline size_t getPatternFrameCount() const {
        return patternFrameCount;
    }

    inline frame_t const * getPatternFrame(size_t const frame) const {
        if (frame >= patternFrameCount) {
            return patternFrames + (patternFrameCount - 1);
        }
        return patternFrames + frame;
    }

    size_t getPatternCount();

    bool loadPattern(size_t index);

    bool parsePattern();
};



