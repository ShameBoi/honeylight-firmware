/**
 * main.cpp
 *
 * @author Shame Boi
 */
#include <Arduino.h>

#include <honeylight/Honeylight.h>

static Honeylight honeylight;


[[gnu::used]]
void setup() {
    Serial.begin(9600);

    honeylight.init();
}

/*
//while (!Serial) {
//    ; // wait for serial port to connect.
//}


void printSpaces(uint8_t num);
void printDirectory(File dir, uint8_t numSpaces);

*/

[[gnu::used]]
void loop() {
    honeylight.loop();
}
