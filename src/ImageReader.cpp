/**
 * ImageReader.cpp
 *
 * @author Shame Boi
 */

#include <Arduino.h>
#include <honeylight/debug.h>
#include <honeylight/util.h>
#include <honeylight/ImageReader.h>

static constexpr uint8_t const BMP_HEADER_MIN_SIZE = 54U;

uint8_t ImageReader::bufferedReadBytes(File &file, void * dest, size_t length) {
    size_t writePos = 0;
    size_t remainingBuffer, amountToCopy;

    while (length > 0) {
        remainingBuffer = readBufferLen - readBufferPos;
        amountToCopy = Util::min(remainingBuffer, length);

        if (amountToCopy > 0) {
            memcpy(reinterpret_cast<char *>(dest) + writePos, readBuffer + readBufferPos, amountToCopy);
            writePos += amountToCopy;
            length -= amountToCopy;
            readBufferPos += amountToCopy;
        }

        if (length > 0) {
            readBufferLen = file.readBytes(readBuffer, sizeof(readBuffer));
            if (readBufferLen <= 0) {
                DBGLN("Premature end of file");
                return 1;
            }
            readBufferPos = 0;
        }
    }

    return 0;
}

unsigned ImageReader::readBMP(rgba_t * const imageData,
                              size_t const imageDataLength,
                              size_t * const width,
                              size_t * const height,
                              File & file) {
    if(file.size() < BMP_HEADER_MIN_SIZE) {
        DBGLN("Malformed BMP: Too Small");
        return -1;
    }

    static char headerBuffer[BMP_HEADER_MIN_SIZE] = {0};

    size_t bytesRead = file.readBytes(headerBuffer, BMP_HEADER_MIN_SIZE);

    if (bytesRead < BMP_HEADER_MIN_SIZE) {
        DBGLN("Error reading BMP");
        return -1;
    }

    if(headerBuffer[0] != 'B' || headerBuffer[1] != 'M') {
        DBGLN("Malformed BMP: No Magic");
        return -1;
    }

    size_t const pixelDataOffset = headerBuffer[10] + 256 * headerBuffer[11];
    *width = headerBuffer[18] + headerBuffer[19] * 256;
    *height = headerBuffer[22] + headerBuffer[23] * 256;

    if(headerBuffer[28] != 24 && headerBuffer[28] != 32) {
        DBGLN("Malformed BMP: Unsupported Channels");
        return -1;
    }

    uint8_t const numChannels = headerBuffer[28] / 8;

    uint32_t scanlineBytes = (*width) * numChannels;
    if(scanlineBytes % 4 != 0) {
        scanlineBytes = (scanlineBytes / 4) * 4 + 4;
    }

    uint32_t const dataSize = scanlineBytes * (*height);
    if(file.size() < dataSize + pixelDataOffset) {
        DBGLN("Malformed BMP: Too Small for Pixels");
        return -1;
    }

    size_t const neededLength = (*width) * (*height);

    if (neededLength > imageDataLength) {
        DBGLN("BMP Too Big for Output Buffer");
        return -1;
    }

    file.seek(pixelDataOffset);
    resetBuffer();

    if (!preLoadBuffer(file)) {
        return -1;
    }

    uint8_t error = 0;
    for(uint32_t y = 0; y < (*height); y++) {
        size_t bytesReadThisLine = 0;
        for (uint32_t x = 0; x < (*width); x++) {

            // BMP is flipped upside-down relative to how we work
            size_t newpos = (((*height) - y - 1) * (*width)) + x;

            // BMP is either BGR or BGRA, so we have to reorder the bytes
            if (numChannels == 3) {
                bgr_t pixel;
                if (bufferedReadBytes(file, &pixel, sizeof(bgr_t)) != 0) {
                    return -1;
                }
                imageData[newpos].blue = pixel.blue;
                imageData[newpos].green = pixel.green;
                imageData[newpos].red = pixel.red;
                imageData[newpos].alpha = 0xFFU;
            } else {
                bgra_t pixel;
                if (bufferedReadBytes(file, &pixel, sizeof(bgra_t)) != 0) {
                    return -1;
                }
                imageData[newpos].blue = pixel.blue;
                imageData[newpos].green = pixel.green;
                imageData[newpos].red = pixel.red;
                imageData[newpos].alpha = pixel.alpha;
            }
            bytesReadThisLine += numChannels;
        }

        // BMP pads out any row that isn't a multiple of 4
        uint8_t missingPadding = 4 - (bytesReadThisLine % 4);
        if (missingPadding < 4) {
            uint8_t discard[3] = {0};
            if (bufferedReadBytes(file, discard, missingPadding) != 0) {
                return -1;
            }
        }
    }

    return 0;
}

bool ImageReader::preLoadBuffer(File &file) {
    readBufferLen = file.readBytes(readBuffer, sizeof(readBuffer));
    if (readBufferLen <= 0) {
        DBGLN("Premature end of file");
        return false;
    }
    readBufferPos = 0;
    return true;
}
