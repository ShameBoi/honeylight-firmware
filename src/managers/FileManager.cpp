/**
 * FileManager.cpp
 *
 * @author Shame Boi
 */

#include <cstring>

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

bool FileManager::hasExtension(File & file, char const *const extension) {
    char const * const name = file.name();
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
    Serial.print("Loading pattern ");
    Serial.println(activePatternIndex);

    bool found = false;
    size_t count = 0;
    File currentPattern;
    while ((currentPattern = root.openNextFile())) {
        if (currentPattern.isDirectory()) {
            if (activePatternIndex == count) {
                rendererManager->getFileRenderer().setPatternFrameCount(0);
                rendererManager->getLoadingBarRenderer().setPercent(25);
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
        return State::ParsePattern;
    }

    Serial.println("Pattern not found!");
    rendererManager->showRainbowRenderer();
    return State::Idle;
}

FileManager::State FileManager::parsePattern() {
    if (!patternToParse) {
        return State::Idle;
    }
    FileRenderer & renderer = rendererManager->getFileRenderer();
    File patternFrameToParse;
    if ((patternFrameToParse = patternToParse.openNextFile()) && renderer.getPatternFrameCount() < HONEYLIGHT_MAX_PATTERN_FRAMES) {
        if (!patternFrameToParse.isDirectory() && hasExtension(patternFrameToParse, BMP_EXTENSION)) {
            processPossibleFrameFile(patternFrameToParse);
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
        Serial.print("Loaded ");
        Serial.print(renderer.getPatternFrameCount());
        Serial.print(" frames in ");
        Serial.print(millis() - loadStartMsec);
        Serial.println("ms");
        rendererManager->showFileRenderer();
    } else {
        Serial.println("Error: no valid frames loaded...");
        rendererManager->showRainbowRenderer();
    }
    return State::Idle;
}

bool FileManager::processPossibleFrameFile(File & file) {
    FileRenderer & renderer = rendererManager->getFileRenderer();
    char const * entryName = file.name();
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
        Serial.print("Frame number can't be zero, discarding: ");
        Serial.println(entryName);
        return false;
    }
    if (frameNumber > HONEYLIGHT_MAX_PATTERN_FRAMES) {
        Serial.print("Frame number ");
        Serial.print(frameNumber);
        Serial.print(" too large, discarding: ");
        Serial.println(entryName);
        return false;
    }

    if (transitionFrames > HONEYLIGHT_MAX_TRANSITION_FRAMES) {
        Serial.print("Transition frames ");
        Serial.print(transitionFrames);
        Serial.print(" too large, discarding: ");
        Serial.println(entryName);
        return false;
    }

    frame_t * const frame = renderer.getPatternFrame(frameNumber - 1);
    if (frame == nullptr) {
        return false;
    }

    frame->frameNumber = frameNumber;
    frame->transitionFrames = transitionFrames;
    frame->fadeNext = isFadeNext;

    if (!parseFrame(file, frame)) {
        return false;
    }

    if (frameNumber >= renderer.getPatternFrameCount()) {
        renderer.setPatternFrameCount(frameNumber);
    }

    Serial.print("Loaded frame: ");
    Serial.print(entryName);
    Serial.print(" - { ");
    Serial.print(frame->frameNumber);
    Serial.print(", ");
    Serial.print(frame->transitionFrames);
    Serial.print(", ");
    Serial.print(frame->fadeNext ? 'F' : 'N');
    Serial.println(" }");
    return true;
}

bool FileManager::parseFrame(File & entry, frame_t * const dest) {
    Serial.print("Parsing frame file: ");
    Serial.println(entry.name());

    size_t width, height;
    static ImageReader imageReader;

    if (imageReader.readBMP(decodedFileBuff, sizeof(decodedFileBuff) / sizeof(*decodedFileBuff), &width, &height, entry)) {
        Serial.print("Error decoding BMP");
        return false;
    }

    Serial.print("Remapping pixels...");
    for (size_t row = 0; row < HONEYLIGHT_DISPLAY_ROWS; ++row) {
        size_t const rowXOffset = getXStartForRow(row);
        size_t const rowYOffset = row * ROW_SPACING;
        size_t const rowStart = (width * (rowYOffset + 2)) + rowXOffset;
        size_t pixelStartByte;
        for (size_t col = 0; col < display_buffer_t::getRowLength(row); ++col) {
            pixelStartByte = (rowStart + (col * COLUMN_SPACING));
            if (pixelStartByte > (width * height * sizeof(rgba_t))) {
                Serial.println("Calculated position larger than buffer");
                return false;
            }

            uint8_t const red = decodedFileBuff[pixelStartByte].red;
            uint8_t const green = decodedFileBuff[pixelStartByte].green;
            uint8_t const blue = decodedFileBuff[pixelStartByte].blue;
            uint8_t const alpha = decodedFileBuff[pixelStartByte].alpha;
            uint8_t const brightness = (alpha / 255.0) * HONEYLIGHT_MAX_BRIGHTNESS;
            dest->data.set(row, col, color_t(brightness, red, green, blue));
        }
    }

    Serial.println(" Done.");

    return true;
}
