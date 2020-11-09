/*
  LEDStrip.cpp - Bibliothek um den Umgang mit der FastLED library und den LED Streifen einfacher zu machen
  Erstellt vom Dani am 14.10.202
  Ka was jetzt huier noch hin kommt
*/

#include "Arduino.h"
#include "FastLED.h"
#include "LEDStrip.h"


LEDStrip::LEDStrip(int data_pin, int len)
{
    _pin = data_pin;
    _len = len;
    CRGB _colors[_len] = CRGB::Black;

    FastLED.addLeds<WS2812B, data_pin, GRB>(_colors, _len);
}