#pragma once

#define WIDTH          16
#define HEIGHT         16
#define NUM_LEDS       WIDTH * HEIGHT
#define LED_TYPE  WS2812B
#define CHIPSET   WS2812B
#define COLOR_ORDER   GRB
#define DATA_PIN        5
#define BRIGHTNESS     90
#define VOLTS           5
#define MAX_MA       1500

// Move to utils
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))


// Param for different pixel layouts
static const bool    kMatrixSerpentineLayout = true;
const uint8_t kMatrixWidth = WIDTH;
const uint8_t kMatrixHeight = HEIGHT;

class Utils {
  public: 
    static uint16_t XY( uint8_t x, uint8_t y);
};
