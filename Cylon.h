#pragma once

#include <FastLED.h>

class Cylon : public Pattern {
public:
  Cylon(CRGB* ledsArray, int numLeds)
  {
    leds = ledsArray;
    numLeds = numLeds;
  }
  void initialize() override
  {
    hue = 150;
    reverse = false;
    idx = 0;
    leds[0] = CHSV(hue, 255, 255);
  }

  void fadeall() { for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(250); } }

  void update() override
  {
    
    // Loop semi-quickly through each LED, changing the hue slightly for each one.
    //for(int i = 0; i < NUM_LEDS; i++) {
      idx = reverse ? idx - 1 : idx + 1;
      //int idx = reverse ? 255 - i : i;
      leds[idx] = CHSV(hue++, 255, 255);
      FastLED.show(); 
      fadeall();
      delay(10);
    //}
    
    if (idx == 0) {
      reverse = false;
    }
    else if (idx == 255) {
      reverse = true;
    }

  }

private:
  CRGB* leds;
  int numLeds;
  uint idx = 0;
  uint8_t hue = 0;
  bool reverse = false;

};
