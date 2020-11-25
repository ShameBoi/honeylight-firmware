/**
 * config.h
 *
 * @author Shame Boi
 */
#pragma once

#ifndef HONEYLIGHT_FPS
#define HONEYLIGHT_FPS 30
#endif

#ifndef HONEYLIGHT_DEFAULT_BRIGHTNESS
#define HONEYLIGHT_DEFAULT_BRIGHTNESS 5
#endif

#ifndef HONEYLIGHT_MAX_BRIGHTNESS
#define HONEYLIGHT_MAX_BRIGHTNESS 10
#endif


constexpr uint16_t const MillisecondsPerFrame = (1.0 / static_cast<float>(HONEYLIGHT_FPS)) * 1000U;
