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
#define HONEYLIGHT_MAX_BRIGHTNESS 5
#endif


#ifndef HONEYLIGHT_MAX_PATTERN_FRAMES
#define HONEYLIGHT_MAX_PATTERN_FRAMES (HONEYLIGHT_FPS * 3U)
#endif

#ifndef HONEYLIGHT_IMAGE_BUFFER_SIZE
#define HONEYLIGHT_IMAGE_BUFFER_SIZE 16384
#endif


constexpr uint16_t const MillisecondsPerFrame = (1.0 / static_cast<float>(HONEYLIGHT_FPS)) * 1000U;

