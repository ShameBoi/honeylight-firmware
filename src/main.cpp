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
    honeylight.init();
}

[[gnu::used]]
void loop() {
    honeylight.loop();
}
