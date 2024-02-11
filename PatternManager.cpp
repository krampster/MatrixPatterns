#include "PatternManager.h"
#include "Utils.h"

PatternManager::PatternManager(const PatternConfig* patternConfigs, int numPatterns, CRGB* leds, int numLeds)
    : patternConfigs(patternConfigs), numPatterns(numPatterns), currentPattern(nullptr), currentPatternIndex(0), leds(leds), numLeds(numLeds) {
      lastSwitchTime = millis();
    }

void PatternManager::initialize() {
  // Choose the initial pattern (e.g., the first pattern in the array)
  randomSeed(analogRead(A1));
  uint firstIndex = random(0, numPatterns);
  setPattern(firstIndex);
}

void PatternManager::update() {
  if (currentPattern != nullptr) {
    currentPattern->update();
  }
}

void PatternManager::nextPattern() {
  setPattern((currentPatternIndex + 1) % numPatterns);
}

void PatternManager::setPattern(uint newIndex) {
  if (currentPattern != nullptr) {
    delete currentPattern;
    currentPattern = nullptr;
  }

  currentPatternIndex = newIndex;
  currentPattern = patternConfigs[currentPatternIndex].createInstance(leds, numLeds);

  uint8_t brightness = patternConfigs[currentPatternIndex].brightness ? patternConfigs[currentPatternIndex].brightness : BRIGHTNESS;
  FastLED.setBrightness(brightness);

  if (currentPattern != nullptr) {
    currentPattern->initialize();
  }
}

void PatternManager::switchPatternHourly() {
  // Implement logic to switch patterns every hour
  unsigned long currentTime = millis();
  if (currentTime - lastSwitchTime >= timeDelay) {
    nextPattern();
    lastSwitchTime = currentTime;
  }
}
