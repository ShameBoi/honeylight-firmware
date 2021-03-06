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
#include <honeylight/data/Frame.h>
#include <honeylight/managers/Manager.h>
#include <honeylight/managers/RendererManager.h>

class FileManager : public Manager {
private:
    constexpr static char const * const PATTERN_DIRECTORY = "/patterns";
    constexpr static char const * const BMP_EXTENSION = ".bmp";
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

    static size_t getPossibleFrameFileCount(File & patternDirectory);

    enum class State {
        Error = 0,
        Idle,
        LoadPattern,
        ParsePattern
    };

    RendererManager * const rendererManager;
    State state = State::Error;
    File root;
    bool patternCountLoaded = false;
    bool patternLoaded = false;
    size_t patternCount = 0;
    size_t activePatternIndex = 0;
    rgba_t decodedFileBuff[HONEYLIGHT_IMAGE_BUFFER_SIZE / sizeof(rgba_t)] = {};
    File patternToParse;
    uint32_t loadStartMsec = 0;
    size_t checkedPossiblePatternFiles = 0;
    size_t possiblePatternFrames = 0;

    State doLoadPattern();

    State parsePattern();

    bool parseFrame(File & entry, Frame * dest);

    bool processPossibleFrameFile(File & file);

public:
    explicit FileManager(RendererManager & rendererManager) : rendererManager(&rendererManager) {};

    ~FileManager() override = default;

    size_t getPatternCount();

    inline size_t getActivePatternIndex() const {
        return activePatternIndex;
    }

    void begin() override;

    bool hasWork() override;

    void work() override;

    bool loadPattern(size_t index);
};



