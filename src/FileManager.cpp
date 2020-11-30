/**
 * FileManager.cpp
 *
 * @author Shame Boi
 */

#include <lodepng.h>
#include <honeylight/FileManager.h>

FileManager::FileManager() {

}

void FileManager::begin() {
    if (!SD.begin(BUILTIN_SDCARD)) {
        return;
    }

    if (SD.exists(PATTERN_DIRECTORY)) {
        root = SD.open(PATTERN_DIRECTORY);
    }
}

size_t FileManager::getPatternCount() {
    if (!root) {
        return 0;
    }

    size_t count = 0;

    File curEntry;
    while ((curEntry = root.openNextFile())) {
        if (curEntry.isDirectory()) {
            ++count;
        }
        curEntry.close();
    }
    root.rewindDirectory();
    return count;
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
    if (!root) {
        return false;
    }

    if (currentPattern) {
        currentPattern.close();
    }


    Serial.print("Loading pattern ");
    Serial.println(index);

    size_t count = 0;
    File curEntry;
    while ((curEntry = root.openNextFile())) {
        Serial.print("Checking: ");
        Serial.println(curEntry.name());
        if (curEntry.isDirectory()) {
            if (index == count) {
                patternIndex = index;
                currentPattern = curEntry;
                Serial.print("Loaded pattern: ");
                Serial.println(currentPattern.name());
                root.rewindDirectory();
                return true;
            }
            ++count;
        }
        curEntry.close();
    }
    root.rewindDirectory();
    return false;
}

bool FileManager::parsePattern() {
    if (!currentPattern) {
        return false;
    }
    patternFrameCount = 0;
    File curEntry;
    while ((curEntry = currentPattern.openNextFile()) && patternFrameCount < HONEYLIGHT_MAX_PATTERN_FRAMES) {
        if (!curEntry.isDirectory() && hasExtension(curEntry, PNG_EXTENSION)) {
            Serial.print("Checking possible frame: ");
            Serial.println(curEntry.name());
            processPossibleFrameFile(curEntry);
        }
        curEntry.close();
    }
    Serial.print("Loaded ");
    Serial.print(patternFrameCount);
    Serial.println(" frames.");
    return true;
}


bool FileManager::processPossibleFrameFile(File & file) {
    String entryName = String(file.name());
    size_t const underscoreIndex = entryName.indexOf('_');
    if (underscoreIndex <= 0) {
        Serial.print("Bad name for frame: ");
        Serial.println(entryName);
        return false;
    }

    size_t const periodIndex = entryName.lastIndexOf('.');
    if (periodIndex <= 0 || periodIndex < underscoreIndex) {
        Serial.print("Bad name for frame: ");
        Serial.println(entryName);
        return false;
    }

    String frameNumberString = entryName.substring(0, underscoreIndex);
    String transitionString = entryName.substring(underscoreIndex + 1, periodIndex);
    uint32_t const frameIndex = strtoul(frameNumberString.c_str(), nullptr, 10) - 1;
    if (frameIndex >= HONEYLIGHT_MAX_PATTERN_FRAMES) {
        Serial.print("Too many frames, discarding: ");
        Serial.println(entryName);
        return false;
    }

    bool const isFadeNext = transitionString.endsWith('F');
    if (isFadeNext) {
        transitionString.remove(transitionString.length() - 1);
    }

    if (transitionString.length() < 1) {
        Serial.print("Bad name for frame: ");
        Serial.println(entryName);
        return false;
    }

    uint32_t const transitionFrames = strtoul(transitionString.c_str(), nullptr, 10);

    frame_t & frame = patternFrames[frameIndex];
    frame.frameNumber = frameIndex + 1;
    frame.transitionFrames = transitionFrames;
    frame.fadeNext = isFadeNext;

    if (!parseFrame(file, &frame)) {
        Serial.print("Error loading frame: ");
        Serial.println(entryName);
        return false;
    }

    if (frameIndex >= patternFrameCount) {
        patternFrameCount = frameIndex + 1;
    }

    Serial.print("Loaded frame: ");
    Serial.println(entryName);
    return true;
}

bool FileManager::parseFrame(File & entry, frame_t * const dest) {
    Serial.print("Parsing frame file: ");
    Serial.println(entry.name());

    static uint8_t fileDecodeWorkBuff[HONEYLIGHT_IMAGE_BUFFER_SIZE] = {0};
    size_t readBytes = entry.read(patternFileLoadBuff, sizeof(patternFileLoadBuff));
    if (readBytes <= 0) {
        return false;
    }

    Serial.print("Read bytes: ");
    Serial.println(readBytes);
    delay(10);

    unsigned width, height;
    unsigned error;

    error = lodepng_decode32(decodedFileBuff,
                             sizeof(decodedFileBuff),
                             fileDecodeWorkBuff,
                             sizeof(fileDecodeWorkBuff),
                             &width,
                             &height,
                             patternFileLoadBuff,
                             readBytes);

    if (error) {
        Serial.print("Error decoding PNG: ");
        Serial.println(error);
        return false;
    }

    auto * const castImage = reinterpret_cast<rgba_t *>(decodedFileBuff);

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
            uint8_t const red = castImage[pixelStartByte].red;
            uint8_t const green = castImage[pixelStartByte].green;
            uint8_t const blue = castImage[pixelStartByte].blue;
            uint8_t const alpha = castImage[pixelStartByte].alpha;
            uint8_t const brightness = (alpha / 255.0) * HONEYLIGHT_MAX_BRIGHTNESS;
            dest->data.set(row, col, color_t(brightness, red, green, blue));
        }
    }

    return true;
}

uint8_t FileManager::convertHexByte(char const *str) {
    static char buff[3] = {0};

    buff[0] = str[0];
    buff[1] = str[1];
    return strtoul(buff, nullptr, 16);
}
