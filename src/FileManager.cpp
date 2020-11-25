/**
 * FileManager.cpp
 *
 * @author Shame Boi
 */

#include <honeylight/FileManager.h>

FileManager::FileManager() {

}

void FileManager::printDirectory(File & dir, uint8_t const numSpaces) {
    while(true) {
        File entry = dir.openNextFile();
        if (! entry) {
            //Serial.println("** no more files **");
            break;
        }
        printSpaces(numSpaces);
        Serial.print(entry.name());
        if (entry.isDirectory()) {
            Serial.println("/");
            printDirectory(entry, numSpaces+2);
        } else {
            // files have sizes, directories do not
            printSpaces(48U - numSpaces - strlen(entry.name()));
            Serial.print("  ");
            Serial.println(entry.size(), DEC);
        }
        entry.close();
    }
}

void FileManager::printSpaces(uint8_t num) {
    for (int i=0; i < num; i++) {
        Serial.print(" ");
    }
}

void FileManager::begin() {
    Serial.print("Initializing SD card...");

    if (!SD.begin(BUILTIN_SDCARD)) {
        Serial.println("initialization failed!");
        return;
    }
    Serial.println("initialization done.");

    root = SD.open("/");

    printDirectory(root, 0);

    Serial.println("done!");
}
