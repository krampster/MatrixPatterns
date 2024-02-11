#pragma once

#include <FastLED.h>
#include <iterator>

FASTLED_USING_NAMESPACE
#define FRAMES_PER_SECOND  120
#define PATTERN_CHANGE_DELAY 60



class DemoReel100 : public Pattern {
public:
  DemoReel100(CRGB* ledsArray, int numLeds)
  {
    leds = ledsArray;
    numLeds = numLeds;
  }
  void initialize() override
  {
  }

  void nextPattern()
  {
    // add one to the current pattern number, and wrap around at the end
    gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE(LocalPatterns);
  }

  void rainbow() 
  {
    // FastLED's built-in rainbow generator
    fill_rainbow_circular( leds, NUM_LEDS, gHue);
  }

  void rainbowWithGlitter() 
  {
    // built-in FastLED rainbow, plus some random sparkly glitter
    rainbow();
    addGlitter(80);
  }

  void addGlitter( fract8 chanceOfGlitter) 
  {
    if( random8() < chanceOfGlitter) {
      leds[ random16(NUM_LEDS) ] += CRGB::White;
    }
  }

  void confetti() 
  {
    // random colored speckles that blink in and fade smoothly
    fadeToBlackBy( leds, NUM_LEDS, 10);
    int pos = random16(NUM_LEDS);
    leds[pos] += CHSV( gHue + random8(64), 200, 255);
  }

  void update() override
  {
    // Call the current pattern function, updating the 'leds' array
    (this->*LocalPatterns[gCurrentPatternNumber])();

    // send the 'leds' array out to the actual LED strip
    FastLED.show();  
    // insert a delay to keep the framerate modest
    FastLED.delay(1000/FRAMES_PER_SECOND); 

    // do some periodic updates
    EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
    EVERY_N_SECONDS( PATTERN_CHANGE_DELAY ) { nextPattern(); } // change patterns periodically
  }

private:
  CRGB* leds;
  int numLeds;


  uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
  uint8_t gHue = 0; // rotating "base color" used by many of the patterns
  
  // typedef void (*SimplePatternList[])();
  // SimplePatternList gPatterns = { 
  //   DemoReel100::rainbowWithGlitter,
  //   DemoReel100::confetti
  // };
  using function_ptr = void (DemoReel100::*)();
  function_ptr LocalPatterns[2] = {&DemoReel100::rainbowWithGlitter, &DemoReel100::confetti};
};
