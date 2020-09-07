#include <Arduino.h>
#include <button_handler.h>
#include <led_handler.h>
#include <state_machine_main.h>

// Länge der einzelnen Streifen, beide Seiten sollten gleich lang sein
#define LED_NUM_INFINITY 65
#define LED_NUM_TRIANGLE 37
#define LED_NUM_LOGO 4 


// erstellen der ganzen CRGB Arrays für die Farben der Pixel, jeder Streifen hat sein eigenes Array
CRGB led_main_inf[LED_NUM_INFINITY] = CRGB::Black;
CRGB led_main_tri[LED_NUM_TRIANGLE] = CRGB::Black;
CRGB led_main_logo[LED_NUM_LOGO] = CRGB::Blue;

CRGB led_slave_inf[LED_NUM_INFINITY] = CRGB::Black;
CRGB led_slave_tri[LED_NUM_TRIANGLE] = CRGB::Black;
CRGB led_slave_logo[LED_NUM_LOGO] = CRGB::Red;


// Punktestand für beide Seiten
uint8_t score_main = 0;
uint8_t score_slave = 0;





void setup(){

    // alles für die Buttons initialisieren und Bounce library vorbereiten
    setupButtons();

    // alle LED Streifen initialisieren
    setupLEDs(led_main_inf, led_main_tri, led_main_logo, led_slave_inf, led_slave_tri, led_slave_logo, LED_NUM_INFINITY, LED_NUM_TRIANGLE, LED_NUM_LOGO);


}


void loop(){

    update_stm();



}