#ifndef PATTERN_CONFIG_H
#define PATTERN_CONFIG_H

#include <FastLED.h>
class Pattern;  // Forward declaration

struct PatternConfig {
  const char* name;
  uint8_t brightness; // Leave 0 for default.
  Pattern* (*createInstance)(CRGB*, int);
};

#endif
