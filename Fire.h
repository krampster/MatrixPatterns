#pragma once

#include <FastLED.h>

#define MAT_W   WIDTH          /* Size (columns) of entire matrix */
#define MAT_H   HEIGHT          /* and rows */
#undef  MAT_COL_MAJOR       /* define if matrix is column-major (that is pixel 1 is in the same column as pixel 0) */
//#define MAT_TOP             /* define if matrix 0,0 is in top row of display; undef if bottom */
#define MAT_LEFT            /* define if matrix 0,0 is on left edge of display; undef if right */
#define MAT_ZIGZAG          /* define if matrix zig-zags ---> <--- ---> <---; undef if scanning ---> ---> ---> */
#define FPS 10              /* Refresh rate */

/* DEBUG CONFIGURATION */

#undef  SERIAL              /* Serial slows things down; don't leave it on. */

/* SECONDARY CONFIGURATION */


/* Display size; can be smaller than matrix size, and if so, you can move the origin.
 * This allows you to have a small fire display on a large matrix sharing the display
 * with other stuff. See README at Github. */
const uint16_t rows = MAT_H;
const uint16_t cols = MAT_W;
const uint16_t xorg = 0;
const uint16_t yorg = 0;

/* Flare constants */
const uint8_t flarerows = 1;    /* number of rows (from bottom) allowed to flare */
const uint8_t maxflare = 3;     /* max number of simultaneous flares */
const uint8_t flarechance = 30; /* chance (%) of a new flare (if there's room) */
const uint8_t flaredecay = 14;  /* decay rate of flare radiation; 14 is good */
const uint8_t sparkchance = 10; /* chance (%) of a new flare (if there's room) */

/* This is the map of colors from coolest (black) to hottest. Want blue flames? Go for it! */
const uint32_t colors[] = {
  0x000000,
  0x100000,
  0x300000,
  0x600000,
  0x800000,
  0xA00000,
  0xC02000,
  0xC04000,
  0xC05000,
  0xC07000,
  0xC0C014
};
const uint8_t NCOLORS = (sizeof(colors)/sizeof(colors[0]));

uint8_t pix[rows][cols];
uint8_t nflare = 0;
uint32_t flare[maxflare];

/** pos - convert col/row to pixel position index. This takes into account
 *  the serpentine display, and mirroring the display so that 0,0 is the
 *  bottom left corner and (MAT_W-1,MAT_H-1) is upper right. You may need
 *  to jockey this around if your display is different.
 */
#ifndef MAT_LEFT
#define __MAT_RIGHT
#endif
#ifndef MAT_TOP
#define __MAT_BOTTOM
#endif
#if defined(MAT_COL_MAJOR)
const uint8_t phy_h = MAT_W;
const uint8_t phy_w = MAT_H;
#else
const uint8_t phy_h = MAT_H;
const uint8_t phy_w = MAT_W;
#endif

uint16_t pos( uint16_t col, uint16_t row ) {
#if defined(MAT_COL_MAJOR)
    uint16_t phy_x = xorg + (uint16_t) row;
    uint16_t phy_y = yorg + (uint16_t) col;
#else
    uint16_t phy_x = xorg + (uint16_t) col;
    uint16_t phy_y = yorg + (uint16_t) row;
#endif
#if defined(MAT_LEFT) && defined(MAT_ZIGZAG)
  if ( ( phy_y & 1 ) == 1 ) {
    phy_x = phy_w - phy_x - 1;
  }
#elif defined(__MAT_RIGHT) && defined(MAT_ZIGZAG)
  if ( ( phy_y & 1 ) == 0 ) {
    phy_x = phy_w - phy_x - 1;
  }
#elif defined(__MAT_RIGHT)
  phy_x = phy_w - phy_x - 1;
#endif
#if defined(MAT_TOP) and defined(MAT_COL_MAJOR)
  phy_x = phy_w - phy_x - 1;
#elif defined(MAT_TOP)
  phy_y = phy_h - phy_y - 1;
#endif
  return phy_x + phy_y * phy_w;
}

uint32_t isqrt(uint32_t n) {
  if ( n < 2 ) return n;
  uint32_t smallCandidate = isqrt(n >> 2) << 1;
  uint32_t largeCandidate = smallCandidate + 1;
  return (largeCandidate*largeCandidate > n) ? smallCandidate : largeCandidate;
}

// Set pixels to intensity around flare
void glow( int x, int y, int z ) {
  int b = z * 10 / flaredecay + 1;
  for ( int i=(y-b); i<(y+b); ++i ) {
    for ( int j=(x-b); j<(x+b); ++j ) {
      if ( i >=0 && j >= 0 && i < rows && j < cols ) {
        int d = ( flaredecay * isqrt((x-j)*(x-j) + (y-i)*(y-i)) + 5 ) / 10;
        uint8_t n = 0;
        if ( z > d ) n = z - d;
        if ( n > pix[i][j] ) { // can only get brighter
          pix[i][j] = n;
        }
      }
    }
  }
}

int sparkRow = rows;
int sparkCol = cols;

void dospark() {
  if (sparkRow == rows && random(1,101) <= sparkchance) {
    sparkCol = random(2, cols - 1);
    sparkRow = 3;
  }
  if (sparkRow != rows) {
    if (random(0,4) < 2 && pix[sparkRow][sparkCol] > 0) {
      pix[sparkRow][sparkCol] += 1; // Half the time stay lighter.
    }
    if (random(0,5) == 1) {
      // Move towards the center randomly.
      int delta = (sparkCol >= (cols / 2)) ? -1 : 1;
      pix[sparkRow][sparkCol + delta] = pix[sparkRow][sparkCol];
      pix[sparkRow][sparkCol] = pix[sparkRow][sparkCol - delta];
      sparkCol += delta;
    }
    sparkRow++;
  }
}

void newflare() {
  if ( nflare < maxflare && random(1,101) <= flarechance ) {
    int min = 2;
    int max = cols - 2;

    // Generate 8 uniform random numbers and sum them
    float sum = 0.0;
    for (int i = 0; i < 4; ++i) {
      sum += random(1000) / 10.0;
    }

    // Map the sum to the desired range (0 to 10)
    float result = map(sum - 200.0, -200, 200, min, max);

    // Ensure the result is within the bounds
    int x = constrain(result, min, max);
    int y = random(0, flarerows);
    int z = NCOLORS - 1;
    flare[nflare++] = (z<<16) | (y<<8) | (x&0xff);
    glow( x, y, z );
  }
}



class Fire : public Pattern {
public:
  Fire(CRGB* ledsArray, int numLeds)
  {
    leds = ledsArray;
    numLeds = numLeds;
  }
  void initialize() override
  {
    for ( uint16_t i=0; i<rows; ++i ) {
      for ( uint16_t j=0; j<cols; ++j ) {
        pix[i][j] = 0;
      }
    }
  }


  /** make_fire() animates the fire display. It should be called from the
  *  loop periodically (at least as often as is required to maintain the
  *  configured refresh rate). Better to call it too often than not enough.
  *  It will not refresh faster than the configured rate. But if you don't
  *  call it frequently enough, the refresh rate may be lower than
  *  configured.
  */
  unsigned long t = 0; /* keep time */
  void make_fire() {
    uint16_t i, j;
    if ( t > millis() ) return;
    t = millis() + (1000 / FPS);

    // First, move all existing heat points up the display and fade
    for ( i=rows-1; i>0; --i ) {
      for ( j=0; j<cols; ++j ) {
        uint8_t n = 0;
        if ( pix[i-1][j] > 0 )
          n = pix[i-1][j] - 1;
        pix[i][j] = n;
      }
    }

    // Heat the bottom row
    for ( j=0; j<cols; ++j ) {
        pix[0][j] = random(NCOLORS-6, NCOLORS-2);
    }

    // dampen the edges, and strengthen the center.
    pix[0][0] -= 2;
    pix[0][1] -= 1;
    pix[0][cols-2] -= 1;
    pix[0][cols-1] -= 2;
    pix[0][cols/2] += 1;
    pix[0][cols/2-1] += 1;

    // flare
    for ( i=0; i<nflare; ++i ) {
      int x = flare[i] & 0xff;
      int y = (flare[i] >> 8) & 0xff;
      int z = (flare[i] >> 16) & 0xff;
      glow( x, y, z );
      if ( z > 1 ) {
        flare[i] = (flare[i] & 0xffff) | ((z-1)<<16);
      } else {
        // This flare is out
        for ( int j=i+1; j<nflare; ++j ) {
          flare[j-1] = flare[j];
        }
        --nflare;
      }
    }
    newflare();
    dospark();

    // Set and draw
    for ( i=0; i<rows; ++i ) {
      for ( j=0; j<cols; ++j ) {
        leds[pos(j,i)] = colors[pix[i][j]];
      }
    }
    FastLED.show();
  }

  void update() override
  {
    make_fire();
    delay(10);
  }

private:
  CRGB* leds;
  int numLeds;
};
