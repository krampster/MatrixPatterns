#pragma once

#include <FastLED.h>
#include <Adafruit_LIS3DH.h>    // For accelerometer
#include <Adafruit_PixelDust.h> // For simulation

#define ACCEL_ADDR 0x18 // Accelerometer I2C address
#define N_GRAINS     16 // Number of grains of sand
#define MAX_FPS      45 // Maximum redraw rate, frames/second

// Sand object, last 2 args are accelerometer scaling and grain elasticity
Adafruit_PixelDust sand(WIDTH, HEIGHT, N_GRAINS, 1, 10); // was 128

// Since we're not using the GFX library, it's necessary to buffer the
// display contents ourselves (8 bits/pixel with the Charlieplex drivers).
uint8_t pixelBuf[WIDTH * HEIGHT];

Adafruit_LIS3DH accel      = Adafruit_LIS3DH();

class HeartDust : public Pattern {
public:
  HeartDust(CRGB* ledsArray, int numLeds)
  {
    leds = ledsArray;
    numLeds = numLeds;
  }
  void err(int x) {
    uint8_t i;
    pinMode(LED_BUILTIN, OUTPUT);       // Using onboard LED
    for(i=1;;i++) {                     // Loop forever...
      digitalWrite(LED_BUILTIN, i & 1); // LED on/off blink to alert user
      delay(x);
    }
  }
  void initialize() override
  {
    uint8_t i, j, bytes;

    if(!sand.begin())            err(1000); // Slow blink = malloc error
    if(!accel.begin(ACCEL_ADDR)) err(250);  // Fast blink = I2C error
    sand.clear();

    accel.setRange(LIS3DH_RANGE_4_G); // Select accelerometer +/- 4G range
    
    // Draw the heart obstacle for sand to move around
    for (uint i = 0; i < 256; i++) {
      if (image_data_Heart[255 - i] != 0x000000) {
        sand.setPixel(i % kMatrixWidth, i / kMatrixWidth);
      }
    }

    sand.randomize(); // Initialize random sand positions
    drawBitmap();
  }

  void drawBitmap()
  {
    // Loop through each LED and set it to the correct pixel from the bitmap.
    // LEDs start at bottom left, and pixels start at top right.
    for (uint i = 0; i < 256; i++) {
      leds[Utils::XY(i % kMatrixWidth, i / kMatrixWidth)] = CRGB(image_data_Heart[255 - i]);
    }

    FastLED.show();
  }

  void update() override
  {
    // Limit the animation frame rate to MAX_FPS.  Because the subsequent sand
    // calculations are non-deterministic (don't always take the same amount
    // of time, depending on their current states), this helps ensure that
    // things like gravity appear constant in the simulation.
    if ( t > millis() ) return;
    t = millis() + (1000 / MAX_FPS);

    // Erase old grain positions in pixelBuf[]
    uint8_t     i;
    dimension_t x, y;
    for(i=0; i<N_GRAINS; i++) {
      sand.getPosition(i, &x, &y);
      leds[Utils::XY(x,y)] = CRGB::Black;
    }

    // Read accelerometer...
    accel.read();
    // Run one frame of the simulation
    // X & Y axes are flipped around here to match physical mounting
    sand.iterate(accel.y, -accel.x, accel.z);

    // Draw new grain positions in pixelBuf[]
    int hueDelta = 255 / N_GRAINS;
    for(i = 0; i < N_GRAINS; i++) {
      sand.getPosition(i, &x, &y);
      leds[Utils::XY(x,y)] = CHSV(i * hueDelta, 255, 200);
    }

    FastLED.show();
  }

private:
  unsigned long t = 0;
  CRGB* leds;
  int numLeds;
};
