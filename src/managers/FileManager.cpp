/**
 * FileManager.cpp
 *
 * @author Shame Boi
 */

#include <cstring>

#include <honeylight/debug.h>
#include <honeylight/ImageReader.h>
#include <honeylight/managers/FileManager.h>

void FileManager::begin() {
    if (!SD.begin(BUILTIN_SDCARD)) {
        return;
    }

    if (SD.exists(PATTERN_DIRECTORY)) {
        root = SD.open(PATTERN_DIRECTORY);
    }

    state = State::Idle;
}

bool FileManager::hasWork() {
    return state != State::Idle;
}

void FileManager::work() {
    switch (state) {
        case State::LoadPattern:
            state = doLoadPattern();
            break;


        case State::ParsePattern:
            state = parsePattern();
            break;

        default:
        case State::Error:
        case State::Idle:
            break;
    }
}

size_t FileManager::getPatternCount() {
    if (!patternCountLoaded) {
        if (!root) {
            return 0;
        }

        patternCount = 0;
        File curEntry;
        while ((curEntry = root.openNextFile())) {
            if (curEntry.isDirectory()) {
                ++patternCount;
            }
            curEntry.close();
        }
        root.rewindDirectory();
        patternCountLoaded = true;
    }
    return patternCount;
}

size_t FileManager::getPossibleFrameFileCount(File &patternDirectory) {
    if (!patternDirectory) {
        return 0;
    }

    size_t patternFrameCount = 0;
    File curEntry;
    while ((curEntry = patternDirectory.openNextFile())) {
        if (!curEntry.isDirectory() && hasExtension(curEntry, BMP_EXTENSION)) {
            ++patternFrameCount;
        }
        curEntry.close();
    }
    patternDirectory.rewindDirectory();
    return patternFrameCount;
}

bool FileManager::hasExtension(File &file, char const *const extension) {
    char const *const name = file.name();
    size_t extensionLength = strlen(extension);
    size_t nameLength = strlen(name);
    if (nameLength < extensionLength) {
        return false;
    }
    return strncmp(name + (nameLength - extensionLength), extension, extensionLength) == 0;
}

bool FileManager::loadPattern(size_t const index) {
    if (!root || state != State::Idle) {
        return false;
    }
    rendererManager->getLoadingBarRenderer().setPercent(0);
    rendererManager->showLoadingBarRenderer();
    activePatternIndex = index;
    state = State::LoadPattern;
    loadStartMsec = millis();
    return true;
}


FileManager::State FileManager::doLoadPattern() {
    DBG("Loading pattern ");
    DBGLN(activePatternIndex);

    bool found = false;
    size_t count = 0;
    possiblePatternFrames = 0;
    File currentPattern;
    while ((currentPattern = root.openNextFile())) {
        if (currentPattern.isDirectory()) {
            if (activePatternIndex == count) {
                rendererManager->getFileRenderer().setPatternFrameCount(0);
                patternToParse = currentPattern;
                found = true;
                break;
            }
            ++count;
        }
        currentPattern.close();
    }
    root.rewindDirectory();

    if (found) {
        possiblePatternFrames = getPossibleFrameFileCount(patternToParse);
        checkedPossiblePatternFiles = 0;
        return State::ParsePattern;
    }

    DBGLN("Pattern not found!");
    rendererManager->showRainbowRenderer();
    return State::Idle;
}

FileManager::State FileManager::parsePattern() {
    if (!patternToParse) {
        return State::Idle;
    }
    FileRenderer &renderer = rendererManager->getFileRenderer();
    File patternFrameToParse;
    if ((patternFrameToParse = patternToParse.openNextFile()) &&
        renderer.getPatternFrameCount() < HONEYLIGHT_MAX_PATTERN_FRAMES) {
        if (!patternFrameToParse.isDirectory() && hasExtension(patternFrameToParse, BMP_EXTENSION)) {
            processPossibleFrameFile(patternFrameToParse);
            ++checkedPossiblePatternFiles;
            rendererManager->getLoadingBarRenderer()
                           .setPercent((checkedPossiblePatternFiles * 100) / possiblePatternFrames);
        }
        patternFrameToParse.close();
        return State::ParsePattern;
    }
    if (patternFrameToParse) {
        patternFrameToParse.close();
    }
    if (patternToParse) {
        patternToParse.close();
    }
    if (renderer.getPatternFrameCount() > 0) {
        DBG("Loaded ");
        DBG(renderer.getPatternFrameCount());
        DBG(" frames in ");
        DBG(millis() - loadStartMsec);
        DBGLN("ms");
        rendererManager->showFileRenderer();
    } else {
        DBGLN("Error: no valid frames loaded...");
        rendererManager->showRainbowRenderer();
    }
    return State::Idle;
}

bool FileManager::processPossibleFrameFile(File &file) {
    FileRenderer &renderer = rendererManager->getFileRenderer();
    char const *entryName = file.name();
    uint32_t frameNumber = 0, transitionFrames = 0;
    bool isFadeNext;
    char transitionModeChar = '\0';

    int result = sscanf(entryName, "%lu_%lu%c", &frameNumber, &transitionFrames, &transitionModeChar);

    if (result == 3) {
        isFadeNext = transitionModeChar == 'F';
    } else {
        return false;
    }

    if (frameNumber == 0) {
        DBG("Frame number can't be zero, discarding: ");
        DBGLN(entryName);
        return false;
    }
    if (frameNumber > HONEYLIGHT_MAX_PATTERN_FRAMES) {
        DBG("Frame number ");
        DBG(frameNumber);
        DBG(" too large, discarding: ");
        DBGLN(entryName);
        return false;
    }

    if (transitionFrames > HONEYLIGHT_MAX_TRANSITION_FRAMES) {
        DBG("Transition frames ");
        DBG(transitionFrames);
        DBG(" too large, discarding: ");
        DBGLN(entryName);
        return false;
    }

    Frame *const frame = renderer.getPatternFrame(frameNumber - 1);
    if (frame == nullptr) {
        return false;
    }

    frame->setFrameNumber(frameNumber);
    frame->setTransitionFrames(transitionFrames);
    frame->setFadeNext(isFadeNext);

    if (!parseFrame(file, frame)) {
        return false;
    }

    if (frameNumber >= renderer.getPatternFrameCount()) {
        renderer.setPatternFrameCount(frameNumber);
    }

    DBG("Loaded frame: ");
    DBG(entryName);
    DBG(" - { ");
    DBG(frame->getFrameNumber());
    DBG(", ");
    DBG(frame->getTransitionFrames());
    DBG(", ");
    DBG(frame->isFadeNext() ? 'F' : 'N');
    DBGLN(" }");
    return true;
}

bool FileManager::parseFrame(File &entry, Frame *const dest) {
    size_t width, height;
    static ImageReader imageReader;

    if (imageReader.readBMP(decodedFileBuff,
                            sizeof(decodedFileBuff) / sizeof(*decodedFileBuff),
                            &width,
                            &height,
                            entry)) {
        DBGLN("Error decoding BMP");
        return false;
    }

    for (size_t row = 0; row < HONEYLIGHT_DISPLAY_ROWS; ++row) {
        size_t const rowXOffset = getXStartForRow(row);
        size_t const rowYOffset = row * ROW_SPACING;
        size_t const rowStart = (width * (rowYOffset + 2)) + rowXOffset;
        size_t pixelStartByte;
        for (size_t col = 0; col < DisplayBuffer::getRowLength(row); ++col) {
            pixelStartByte = (rowStart + (col * COLUMN_SPACING));
            if (pixelStartByte > (width * height * sizeof(rgba_t))) {
                DBGLN("Calculated position larger than buffer");
                return false;
            }

            uint8_t const red = decodedFileBuff[pixelStartByte].red;
            uint8_t const green = decodedFileBuff[pixelStartByte].green;
            uint8_t const blue = decodedFileBuff[pixelStartByte].blue;
            uint8_t const alpha = decodedFileBuff[pixelStartByte].alpha;
            uint8_t const brightness = (alpha / 255.0) * HONEYLIGHT_MAX_BRIGHTNESS;
            dest->getData().set(row, col, color_t(brightness, red, green, blue));
        }
    }

    return true;
}
