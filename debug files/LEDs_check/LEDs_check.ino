#include <FastLED.h>

//This firmware just sets all LEDs to the same colors (red, gree, blue, yellow, magenta, white, black) 

#define BRIGHTNESS 50 //the overall brightness of all LEDs, the value can go from 0 to 255 but your leds might turn red and die off if you set it to over 100 (its not permanent, just remove power frojm the module to reset the volatge regulator current fuse)
#define SPEED 1000 //the time each color stays on (in millisecconds)

CRGB leds[72];

void setup() { 
  FastLED.addLeds<WS2812B, 9, RGB>(leds, 72);
  FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
  for (uint8_t i = 0; i < 72; i++) leds[i] = 0xff0000;
  FastLED.show();
  delay(SPEED);
  for (uint8_t i = 0; i < 72; i++) leds[i] = 0x00ff00;
  FastLED.show();
  delay(SPEED);
  for (uint8_t i = 0; i < 72; i++) leds[i] = 0x0000ff;
  FastLED.show();
  delay(SPEED);
  for (uint8_t i = 0; i < 72; i++) leds[i] = 0xffff00;
  FastLED.show();
  delay(SPEED);
  for (uint8_t i = 0; i < 72; i++) leds[i] = 0x00ffff;
  FastLED.show();
  delay(SPEED);
  for (uint8_t i = 0; i < 72; i++) leds[i] = 0xffffff;
  FastLED.show();
  delay(SPEED);
  for (uint8_t i = 0; i < 72; i++) leds[i] = 0x000000;
  FastLED.show();
  delay(SPEED);
}
