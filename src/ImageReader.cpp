/**
 * ImageReader.cpp
 *
 * @author Shame Boi
 */

#include <Arduino.h>
#include <honeylight/ImageReader.h>

static constexpr uint8_t const BMP_HEADER_MIN_SIZE = 54U;

uint8_t ImageReader::bufferedReadByte(File & file, uint8_t * errorCode) {
    if (readBufferPos >= readBufferLen) {
        readBufferLen = file.readBytes(readBuffer, sizeof(readBuffer));
        if (readBufferLen <= 0) {
            Serial.println("Premature end of file");
            *errorCode = 1;
            return 0;
        }
        readBufferPos = 0;
    }
    return readBuffer[readBufferPos++];
}

unsigned ImageReader::readBMP(rgba_t * const imageData,
                              size_t const imageDataLength,
                              size_t * const width,
                              size_t * const height,
                              File & file) {
    if(file.size() < BMP_HEADER_MIN_SIZE) {
        Serial.println("Malformed BMP: Too Small");
        return -1;
    }

    static char headerBuffer[BMP_HEADER_MIN_SIZE] = {0};

    size_t bytesRead = file.readBytes(headerBuffer, BMP_HEADER_MIN_SIZE);

    if (bytesRead < BMP_HEADER_MIN_SIZE) {
        Serial.println("Error reading BMP");
        return -1;
    }

    if(headerBuffer[0] != 'B' || headerBuffer[1] != 'M') {
        Serial.println("Malformed BMP: No Magic");
        return -1;
    }

    size_t const pixelDataOffset = headerBuffer[10] + 256 * headerBuffer[11];
    *width = headerBuffer[18] + headerBuffer[19] * 256;
    *height = headerBuffer[22] + headerBuffer[23] * 256;

    if(headerBuffer[28] != 24 && headerBuffer[28] != 32) {
        Serial.println("Malformed BMP: Unsupported Channels");
        return -1;
    }

    uint8_t const numChannels = headerBuffer[28] / 8;

    uint32_t scanlineBytes = (*width) * numChannels;
    if(scanlineBytes % 4 != 0) {
        scanlineBytes = (scanlineBytes / 4) * 4 + 4;
    }

    uint32_t const dataSize = scanlineBytes * (*height);
    if(file.size() < dataSize + pixelDataOffset) {
        Serial.println("Malformed BMP: Too Small for Pixels");
        return -1;
    }

    size_t const neededLength = (*width) * (*height);

    if (neededLength > imageDataLength) {
        Serial.println("BMP Too Big for Output Buffer");
        return -1;
    }

    file.seek(pixelDataOffset);
    resetBuffer();

    uint8_t error = 0;
    for(uint32_t y = 0; y < (*height); y++) {
        size_t bytesReadThisLine = 0;
        for (uint32_t x = 0; x < (*width); x++) {

            // BMP is flipped upside-down relative to how we work
            size_t newpos = (((*height) - y - 1) * (*width)) + x;

            // BMP is either BGR or BGRA, so we have to reorder the bytes
            if (numChannels == 3) {
                imageData[newpos].blue = bufferedReadByte(file, &error);
                if (error) {
                    return -1;
                }
                imageData[newpos].green = bufferedReadByte(file, &error);
                if (error) {
                    return -1;
                }
                imageData[newpos].red = bufferedReadByte(file, &error);
                if (error) {
                    return -1;
                }
                imageData[newpos].alpha = 0xFFU;
            } else {
                imageData[newpos].blue = bufferedReadByte(file, &error);
                if (error) {
                    return -1;
                }
                imageData[newpos].green = bufferedReadByte(file, &error);
                if (error) {
                    return -1;
                }
                imageData[newpos].red = bufferedReadByte(file, &error);
                if (error) {
                    return -1;
                }
                imageData[newpos].alpha = bufferedReadByte(file, &error);
                if (error) {
                    return -1;
                }
            }
            bytesReadThisLine += numChannels;
        }

        // BMP pads out any row that isn't a multiple of 4
        uint8_t missingPadding = 4 - (bytesReadThisLine % 4);
        if (missingPadding != 4) {
            while (missingPadding) {
                bufferedReadByte(file, &error);
                if (error) {
                    return -1;
                }
                ++bytesReadThisLine;
                --missingPadding;
            }
        }
    }
    return 0;
}
