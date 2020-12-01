/**
 * FileManager.cpp
 *
 * @author Shame Boi
 */

#include <honeylight/FileManager.h>
#include <honeylight/ImageReader.h>

void FileManager::begin() {
    if (!SD.begin(BUILTIN_SDCARD)) {
        return;
    }

    if (SD.exists(PATTERN_DIRECTORY)) {
        root = SD.open(PATTERN_DIRECTORY);
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

bool FileManager::loadPattern(size_t const index, FilePattern * const dest) {
    if (!root) {
        return false;
    }

    Serial.print("Loading pattern ");
    Serial.println(index);

    bool found = false;
    size_t count = 0;
    File currentPattern;
    while ((currentPattern = root.openNextFile())) {
        Serial.print("Checking: ");
        Serial.println(currentPattern.name());
        if (currentPattern.isDirectory()) {
            if (index == count) {
                activePatternIndex = index;
                found = true;
                break;
            }
            ++count;
        }
        currentPattern.close();
    }

    bool result = false;
    if (found) {
        Serial.print("Loaded pattern: ");
        Serial.println(currentPattern.name());
        result = parsePattern(currentPattern, dest);
    }

    currentPattern.close();
    root.rewindDirectory();
    return result;
}

bool FileManager::parsePattern(File & pattern, FilePattern * const dest) {
    if (!pattern) {
        return false;
    }
    dest->setPatternFrameCount(0);
    File curEntry;
    while ((curEntry = pattern.openNextFile()) && dest->getPatternFrameCount() < HONEYLIGHT_MAX_PATTERN_FRAMES) {
        if (!curEntry.isDirectory() && hasExtension(curEntry, BMP_EXTENSION)) {
            Serial.print("Checking possible frame: ");
            Serial.println(curEntry.name());
            processPossibleFrameFile(curEntry, dest);
        }
        curEntry.close();
    }
    Serial.print("Loaded ");
    Serial.print(dest->getPatternFrameCount());
    Serial.println(" frames.");
    return true;
}


bool FileManager::processPossibleFrameFile(File & file, FilePattern * const dest) {
    char const * entryName = file.name();

    char const * underscorePos = strchr(entryName, '_');
    if (!underscorePos) {
        Serial.print("Bad name for frame: ");
        Serial.println(entryName);
        return false;
    }

    char const * periodPos = strchr(underscorePos, '.');
    if (!periodPos) {
        Serial.print("Bad name for frame: ");
        Serial.println(entryName);
        return false;
    }

    static char parseBuffer[5] = {0};

    size_t const framePartLen = underscorePos - entryName;
    if (framePartLen <= 0 || framePartLen > 3) {
        Serial.print("Bad name for frame: ");
        Serial.println(entryName);
        return false;
    }

    strncpy(parseBuffer, entryName, framePartLen);
    parseBuffer[framePartLen] = '\0';
    uint32_t const frameIndex = strtoul(parseBuffer, nullptr, 10) - 1;
    if (frameIndex >= HONEYLIGHT_MAX_PATTERN_FRAMES) {
        Serial.print("Too many frames, discarding: ");
        Serial.println(entryName);
        return false;
    }

    underscorePos++;
    size_t const periodPartLen = periodPos - underscorePos;
    if (periodPartLen <= 0 || periodPartLen > 4) {
        Serial.print("Bad name for frame: ");
        Serial.println(entryName);
        return false;
    }

    bool const isFadeNext = underscorePos[periodPartLen - 1] == 'F';
    if (isFadeNext) {
        if ((periodPartLen - 1) <= 0) {
            Serial.print("Bad name for frame: ");
            Serial.println(entryName);
            return false;
        }
        strncpy(parseBuffer, underscorePos, periodPartLen - 1);
        parseBuffer[periodPartLen - 1] = '\0';
    } else {
        strncpy(parseBuffer, underscorePos, periodPartLen);
        parseBuffer[periodPartLen] = '\0';
    }

    uint32_t const transitionFrames = strtoul(parseBuffer, nullptr, 10);

    frame_t * const frame = dest->getPatternFrame(frameIndex);
    if (frame == nullptr) {
        return false;
    }

    frame->frameNumber = frameIndex + 1;
    frame->transitionFrames = transitionFrames;
    frame->fadeNext = isFadeNext;

    if (!parseFrame(file, frame)) {
        Serial.print("Error loading frame: ");
        Serial.println(entryName);
        return false;
    }

    if (frameIndex >= dest->getPatternFrameCount()) {
        dest->setPatternFrameCount(frameIndex + 1);
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
    for (size_t row = 0; row < 5; ++row) {
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
