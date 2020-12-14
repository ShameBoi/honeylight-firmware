/**
 * debug.h
 *
 * @author Shame Boi
 */

#pragma once

#include <Arduino.h>

#ifdef HONEYLIGHT_DEBUG

#define DBG(x) Serial.print(x)
#define DBGLN(x) Serial.println(x)

#else

#define DBG(x)
#define DBGLN(x)

#endif
