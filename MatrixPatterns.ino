#include <FastLED.h>
#include "Utils.h"
#include "PatternConfig.h"
#include "PatternManager.h"

#include "Cylon.h"
#include "DemoReel100.h"
#include "Fire.h"
#include "LavaLamp.h"
#include "Pacifica.h"
#include "Zenyatta16.h"
#include "Heart16.h"
#include "Scene16.h"
#include "HeartDust.h"
#include "TwinkleFox.h"

CRGB leds[NUM_LEDS];

// Define configurations for each pattern
PatternConfig patternConfigs[] = {

  {"Cylon", 0, [](CRGB* leds, int numLeds) -> Pattern* { return new Cylon(leds, numLeds); }},
  {"HeartDust", 60, [](CRGB* leds, int numLeds) -> Pattern* { return new HeartDust(leds, numLeds); }},
  {"TwinkleFox", 0, [](CRGB* leds, int numLeds) -> Pattern* { return new TwinkleFox(leds, numLeds); }},
  {"Scene16", 70, [](CRGB* leds, int numLeds) -> Pattern* { return new Scene16(leds, numLeds); }},
  //{"Heart16", 0, [](CRGB* leds, int numLeds) -> Pattern* { return new Heart16(leds, numLeds); }},
  {"Zenyatta16", 70, [](CRGB* leds, int numLeds) -> Pattern* { return new Zenyatta16(leds, numLeds); }},
  {"Pacifica", 200, [](CRGB* leds, int numLeds) -> Pattern* { return new Pacifica(leds, numLeds); }},
  {"LavaLamp", 0, [](CRGB* leds, int numLeds) -> Pattern* { return new LavaLamp(leds, numLeds); }},
  {"Fire", 0, [](CRGB* leds, int numLeds) -> Pattern* { return new Fire(leds, numLeds); }},
  {"DemoReel100", 50, [](CRGB* leds, int numLeds) -> Pattern* { return new DemoReel100(leds, numLeds); }},
};

PatternManager patternManager(patternConfigs, ARRAY_SIZE(patternConfigs), leds, NUM_LEDS);

// constants won't change. They're used here to set pin numbers:
const int buttonPin = A2;  // the number of the pushbutton pin
const int ledPin = 13;    // the number of the LED pin

// Variables will change:
int buttonState;            // the current reading from the input pin
int lastButtonState = LOW;  // the previous reading from the input pin

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers
unsigned long longPressDelay = 2000;

void setup() {
  delay(2000); //safety startup delay
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.setMaxPowerInVoltsAndMilliamps(VOLTS, MAX_MA);
  set_max_power_indicator_LED(ledPin);

  patternManager.initialize();

  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
}

bool buttonStateChanged() {
  // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);
  bool result = false;

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > longPressDelay && buttonState == HIGH) {
    // If you long proess for 2 seconds, tell the pattern manager to change more quickly.
    patternManager.timeDelay = 10 * 1000; // 10 000 milliseconds = 10 seconds
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // Whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state.

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;
      result = true;
    }
  }

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;
  return result;
}

void loop() {

  if (buttonStateChanged() && lastButtonState == HIGH) {
    patternManager.nextPattern();
  }
  
  patternManager.update();
  patternManager.switchPatternHourly();
}
