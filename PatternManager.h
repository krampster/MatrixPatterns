// PatternManager.h
#ifndef PATTERN_MANAGER_H
#define PATTERN_MANAGER_H

#include "Pattern.h"
#include "PatternConfig.h"

class PatternManager {
public:
  PatternManager(const PatternConfig* patternConfigs, int numPatterns, CRGB* leds, int numLeds);
  void initialize();
  void update();
  void nextPattern();
  void switchPatternHourly();
  void setPattern(uint newIndex);

  unsigned long timeDelay = 3600 * 1000; // 3600 000 milliseconds = 1 hour

private:
  Pattern* currentPattern;
  unsigned long lastSwitchTime;
  const PatternConfig* patternConfigs;
  uint numPatterns;
  uint currentPatternIndex;
  CRGB* leds;
  int numLeds;
};

#endif
