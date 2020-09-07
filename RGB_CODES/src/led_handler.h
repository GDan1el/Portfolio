#include <Arduino.h>
#include <FastLED.h>

// Pin Deklarationen von der Masterseite
#define PIN_LED_MAIN_LOGO 18
#define PIN_LED_MAIN_INF 19
#define PIN_LED_MAIN_TRI 21

// Pin Deklarationen der Slaveseite
#define PIN_LED_SLAVE_LOGO 22
#define PIN_LED_SLAVE_TRI 23
#define PIN_LED_SLAVE_INF 25



void setupLEDs(CRGB main_inf[], CRGB main_tri[], CRGB main_logo[], CRGB slave_inf[], CRGB slave_tri[], CRGB slave_logo[], int NUM_INF, int NUM_TRI, int NUM_LOGO){

  // LEDs setupen
  FastLED.addLeds<WS2812B, PIN_LED_MAIN_INF, GRB>(main_inf, NUM_INF);
  FastLED.addLeds<WS2812B, PIN_LED_MAIN_TRI, GRB>(main_tri, NUM_TRI);
  FastLED.addLeds<WS2812B, PIN_LED_MAIN_LOGO, GRB>(main_logo, NUM_LOGO);

  FastLED.addLeds<WS2812B, PIN_LED_SLAVE_INF, GRB>(slave_inf, NUM_INF);
  FastLED.addLeds<WS2812B, PIN_LED_SLAVE_TRI, GRB>(slave_tri, NUM_TRI);
  FastLED.addLeds<WS2812B, PIN_LED_SLAVE_LOGO, GRB>(slave_logo, NUM_LOGO);


}