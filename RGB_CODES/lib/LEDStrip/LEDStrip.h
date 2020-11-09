/*
  Morse.h - Bibliothek um den Umgang mit der FastLED library und den LED Streifen einfacher zu machen
  Erstellt vom Dani am 14.10.202
  Ka was jetzt huier noch hin kommt
*/

#ifndef LEDStrip_h
#define LEDStrip_h

#include "Arduino.h"
#include "FastLED.h"

class LEDStrip
{
    public:
        LEDStrip(int data_pin, int len);
        void init();
        void set_color(CHSV color);
        void set_LED(CHSV color);
        void set_LED(CRGB color);
        void set_brightness(uint8_t brightness);
    
    private:
        int _pin;
        int _len;
        CRGB _colors[];
        
};


















#endif