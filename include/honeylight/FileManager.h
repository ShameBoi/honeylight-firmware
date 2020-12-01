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
#include <honeylight/FilePattern.h>

class FileManager {
private:
    constexpr static char const * const PATTERN_DIRECTORY = "/patterns";
    constexpr static char const * const BMP_EXTENSION = ".BMP";
    constexpr static size_t const COLUMN_SPACING = 6;
    constexpr static size_t const ROW_SPACING = 6;

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

    static bool hasExtension(File & file, char const * extension);

    File root;
    bool patternCountLoaded = false;
    size_t patternCount = 0;
    size_t activePatternIndex = 0;
    rgba_t decodedFileBuff[HONEYLIGHT_IMAGE_BUFFER_SIZE / sizeof(rgba_t)] = {};

    bool parseFrame(File & entry, frame_t * dest);

    bool processPossibleFrameFile(File & file, FilePattern * dest);

    bool parsePattern(File & pattern, FilePattern * dest);

public:
    FileManager() = default;

    size_t getPatternCount();

    inline size_t getActivePatternIndex() const {
        return activePatternIndex;
    }

    void begin();

    bool loadPattern(size_t index, FilePattern * dest);
};



