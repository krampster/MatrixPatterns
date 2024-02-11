#pragma once

#include <FastLED.h>

#define MAX_DIMENSION ((kMatrixWidth>kMatrixHeight) ? kMatrixWidth : kMatrixHeight)

// The 16 bit version of our coordinates
static uint16_t x;
static uint16_t y;
static uint16_t z;

// We're using the x/y dimensions to map to the x/y pixels on the matrix.  We'll
// use the z-axis for "time".  speed determines how fast time moves forward.  Try
// 1 for a very slow moving effect, or 60 for something that ends up looking like
// water.
uint16_t speed = 2; 

// Scale determines how far apart the pixels in our noise matrix are.  Try
// changing these values around to see how it affects the motion of the display.  The
// higher the value of scale, the more "zoomed out" the noise iwll be.  A value
// of 1 will be so zoomed in, you'll mostly see solid colors.
uint16_t scale = 50;

// This is the array that we keep our computed noise values in
uint8_t noise[MAX_DIMENSION][MAX_DIMENSION];

CRGBPalette16 currentPalette( LavaColors_p );
uint8_t       colorLoop = 0;


class LavaLamp : public Pattern {
public:
  LavaLamp(CRGB* ledsArray, int numLeds)
  {
    leds = ledsArray;
    numLeds = numLeds;
  }
  void initialize() override
  {
    x = random16();
    y = random16();
    z = random16();
  }

  // Fill the x/y array of 8-bit noise values using the inoise8 function.
  void fillnoise8() {
    // If we're runing at a low "speed", some 8-bit artifacts become visible
    // from frame-to-frame.  In order to reduce this, we can do some fast data-smoothing.
    // The amount of data smoothing we're doing depends on "speed".
    uint8_t dataSmoothing = 0;
    if( speed < 50) {
      dataSmoothing = 200 - (speed * 4);
    }
    
    for(int i = 0; i < MAX_DIMENSION; i++) {
      int ioffset = scale * i;
      for(int j = 0; j < MAX_DIMENSION; j++) {
        int joffset = scale * j;
        
        uint8_t data = inoise8(x + ioffset,y + joffset,z);

        // The range of the inoise8 function is roughly 16-238.
        // These two operations expand those values out to roughly 0..255
        // You can comment them out if you want the raw noise data.
        data = qsub8(data,16);
        data = qadd8(data,scale8(data,39));

        if( dataSmoothing ) {
          uint8_t olddata = noise[i][j];
          uint8_t newdata = scale8( olddata, dataSmoothing) + scale8( data, 256 - dataSmoothing);
          data = newdata;
        }
        
        noise[i][j] = data;
      }
    }
    
    z += speed;
    
    // apply slow drift to X and Y, just for visual variation.
    x += speed / 8;
    y -= speed / 16;
  }

  void mapNoiseToLEDsUsingPalette()
  {
    static uint8_t ihue=0;
    
    for(int i = 0; i < kMatrixWidth; i++) {
      for(int j = 0; j < kMatrixHeight; j++) {
        // We use the value at the (i,j) coordinate in the noise
        // array for our brightness, and the flipped value from (j,i)
        // for our pixel's index into the color palette.

        uint8_t index = noise[j][i];
        uint8_t bri =   noise[i][j];

        // if this palette is a 'loop', add a slowly-changing base value
        if( colorLoop) { 
          index += ihue;
        }

        // brighten up, as the color palette itself often contains the 
        // light/dark dynamic range desired
        if( bri > 127 ) {
          bri = 255;
        } else {
          bri = dim8_raw( bri * 2);
        }

        CRGB color = ColorFromPalette( currentPalette, index, bri);
        leds[Utils::XY(i,j)] = color;
      }
    }
    
    ihue+=1;
  }


  double doubleMap(double x, double in_min, double in_max, double out_min, double out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  }

  void rgbToHsv(byte r, byte g, byte b, byte hsv[]) {
      double rd = (double) r/255;
      double gd = (double) g/255;
      double bd = (double) b/255;
      double dmax = max(rd, max(gd, bd)), dmin = min(rd, min(gd, bd));
      double h, s, v = dmax;

      double d = dmax - dmin;
      s = dmax == 0 ? 0 : d / dmax;

      if (dmax == dmin) { 
          h = 0; // achromatic
      } else {
          if (dmax == rd) {
              h = (gd - bd) / d + (gd < bd ? 6 : 0);
          } else if (dmax == gd) {
              h = (bd - rd) / d + 2;
          } else if (dmax == bd) {
              h = (rd - gd) / d + 4;
          }
          h /= 6;
      }

      hsv[0] = (byte)doubleMap(h, 0, 1, 0, 255);
      hsv[1] = (byte)doubleMap(s, 0, 1, 0, 255);
      hsv[2] = (byte)doubleMap(v, 0, 1, 0, 255);
  }

  // This function sets up a palette of purple and green stripes.
  void SetupRotatingLavaPalette()
  {
    uint8_t secondHand = (millis() / 1000) % 60;
    static uint8_t lastSecond = 99;
    
    // Rotate the hue of the palette in-place once a second.
    if (lastSecond != secondHand)
    {
      lastSecond = secondHand;

      static uint8_t hue = 1;
      currentPalette = CRGBPalette16( LavaColors_p );
      for(int i = 0; i < 16; i++)
      {
        byte hsv[3];
        rgbToHsv(currentPalette.entries[i].r, currentPalette.entries[i].g, currentPalette.entries[i].b, hsv);
        CHSV pixel = CHSV(hsv[0] + hue, hsv[1], hsv[2]);
        currentPalette.entries[i] = pixel;
      }
      hue += 1;
    }
  }

  void update() override
  {
    SetupRotatingLavaPalette();

    // generate noise data
    fillnoise8();
    
    // convert the noise data to colors in the LED array
    // using the current palette
    mapNoiseToLEDsUsingPalette();

    FastLED.show();
  }

private:
  CRGB* leds;
  int numLeds;
};
