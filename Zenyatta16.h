#pragma once

#include <FastLED.h>


static const uint32_t background = 0x050505;
static const uint32_t outline = 0x000000;

static const uint32_t image_data_Zenyatta[256] PROGMEM = {
    // ∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙
    // ∙∙∙∙∙▓▓▓▓▓▓▓∙∙∙∙
    // ∙∙∙∙▓▓▓▓▓▓▓▓▓∙∙∙
    // ∙∙∙∙▓▓▓▓▓▓▓▓▓∙∙∙
    // ∙∙∙∙▓▓▓▓▓▓▓▓▓∙∙∙
    // ∙∙∙∙▓▓▓▓▓▓▓▓▓∙∙∙
    // ∙∙∙∙▓▓▓▓▓▓▓▓▓∙∙∙
    // ∙∙▓∙▓▓▓▓▓▓▓▓▓∙∙∙
    // ∙∙∙∙▓∙∙▓▓▓∙∙▓∙▓∙
    // ∙∙∙∙∙▓▓▓▓▓▓▓∙∙∙∙
    // ∙▓∙∙∙∙∙∙∙∙∙∙∙∙∙∙
    // ∙∙∙∙∙∙∙▓▓▓∙∙∙∙∙▓
    // ∙∙∙▓∙∙▓▓▓▓▓∙∙▓∙∙
    // ∙∙∙∙∙∙∙▓▓▓∙∙∙∙∙∙
    // ∙∙∙∙∙∙∙▒▒▒∙∙∙∙∙∙
    // ∙∙∙∙∙∙▓▓▒▓▓∙∙∙∙∙
    background, background, background, background, background, outline, outline, outline, outline, outline, outline, outline, background, background, background, background, 
    background, background, background, background, outline, 0xa2a2a2, 0xa2a2a2, 0xa2a2a2, 0xa2a2a2, 0xa2a2a2, 0xa2a2a2, 0xa2a2a2, outline, background, background, background, 
    background, background, background, outline, 0xa2a2a2, 0xa2a2a2, 0x13faff, 0xa2a2a2, 0x13faff, 0xa2a2a2, 0x13faff, 0xa2a2a2, 0xa2a2a2, outline, background, background, 
    background, background, background, outline, 0xa2a2a2, 0xa2a2a2, 0xa2a2a2, 0xa2a2a2, 0xa2a2a2, 0xa2a2a2, 0xa2a2a2, 0xa2a2a2, 0xa2a2a2, outline, background, background, 
    background, background, background, outline, 0xa2a2a2, 0xa2a2a2, 0x13faff, 0xa2a2a2, 0x13faff, 0xa2a2a2, 0x13faff, 0xa2a2a2, 0xa2a2a2, outline, background, background, 
    background, background, background, outline, 0xa2a2a2, 0xa2a2a2, 0xa2a2a2, 0xa2a2a2, 0xa2a2a2, 0xa2a2a2, 0xa2a2a2, 0xa2a2a2, 0xa2a2a2, outline, background, background, 
    background, background, background, outline, 0xa2a2a2, 0xa2a2a2, 0x13faff, 0xa2a2a2, 0x13faff, 0xa2a2a2, 0x13faff, 0xa2a2a2, 0xa2a2a2, outline, background, background, 
    background, background, 0xffbf13, outline, 0xa2a2a2, 0xa2a2a2, 0xa2a2a2, 0xa2a2a2, 0xa2a2a2, 0xa2a2a2, 0xa2a2a2, 0xa2a2a2, 0xa2a2a2, outline, background, background, 
    background, background, background, outline, 0xa2a2a2, outline, outline, 0xa2a2a2, 0xa2a2a2, 0xa2a2a2, outline, outline, 0xa2a2a2, outline, 0xffbf13, background, 
    background, background, background, background, outline, 0xa2a2a2, 0xa2a2a2, 0xffbf13, 0xffbf13, 0xffbf13, 0xa2a2a2, 0xa2a2a2, outline, background, background, background, 
    background, 0xffbf13, background, background, background, outline, outline, outline, outline, outline, outline, outline, background, background, background, background, 
    background, background, background, background, background, background, outline, 0xa2a2a2, 0xa2a2a2, 0xa2a2a2, outline, background, background, background, background, 0xffbf13, 
    background, background, background, 0xffbf13, background, outline, 0xa2a2a2, 0xa2a2a2, 0xa2a2a2, 0xa2a2a2, 0xa2a2a2, outline, background, 0xffbf13, background, background, 
    background, background, background, background, outline, background, outline, 0xa2a2a2, 0xffbf13, 0xa2a2a2, outline, background, outline, background, background, background, 
    background, background, background, background, background, outline, outline, 0xff3713, 0xff3713, 0xff3713, outline, outline, background, background, background, background, 
    background, background, background, background, background, outline, 0xffbf13, 0xffbf13, 0xff3713, 0xffbf13, 0xffbf13, outline, background, background, background, background
};

class Zenyatta16 : public Pattern {
public:
  Zenyatta16(CRGB* ledsArray, int numLeds)
  {
    leds = ledsArray;
    numLeds = numLeds;
  }
  void initialize() override
  {
  }

  void drawBitmap()
  {
    // Loop through each LED and set it to the correct pixel from the bitmap.
    // LEDs start at bottom left, and pixels start at top right.
    for (uint i = 0; i < 256; i++) {
      leds[Utils::XY(i % kMatrixWidth, i / kMatrixWidth)] = CRGB(image_data_Zenyatta[255 - i]);
    }

    FastLED.show();
  }

  void update() override
  {
    drawBitmap();
    delay(20);
  }

private:
  CRGB* leds;
  int numLeds;
};
