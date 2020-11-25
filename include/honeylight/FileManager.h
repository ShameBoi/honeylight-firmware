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

class FileManager {
private:
    File root;

    void printSpaces(uint8_t num);
    void printDirectory(File & dir, uint8_t numSpaces);
public:
    FileManager();

    void begin();
};



