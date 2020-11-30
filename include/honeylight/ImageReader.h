/**
 * ImageReader.h
 *
 * @author Shame Boi
 */
#pragma once

#include <cstdint>
#include <cstddef>
#include <Arduino.h>
#include <SD.h>

#include <honeylight/display_types.h>

class ImageReader {
private:
    char readBuffer[512] = {0};
    size_t readBufferPos = 0;
    size_t readBufferLen = 0;

    inline void resetBuffer() {
        readBufferPos = 0;
        readBufferLen = 0;
    }

    uint8_t bufferedReadByte(File & file, uint8_t * errorCode);

public:
    size_t readBMP(rgba_t * imageData,
                   size_t imageDataLength,
                   size_t * width,
                   size_t * height,
                   File & file);
};
