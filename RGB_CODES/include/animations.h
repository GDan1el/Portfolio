#include <Arduino.h>
#include <FastLED.h>

// Länge der einzelnen Streifen, beide Seiten sollten gleich lang sein
#define LED_NUM_INFINITY 65
#define LED_NUM_TRIANGLE 37
#define LED_NUM_LOGO 4 
struct CRGB leds_infinity[LED_NUM_INFINITY]; 
struct CRGB leds_triangle[LED_NUM_TRIANGLE];

// Gewinneranimation: Freudige Animation, wenn eine Spielseite alle 10 Becher getroffen hat
void win_animation(int player){


}

// Verliereranimation: Eher bedrückte, traurige Animation, wenn die gegnersiche Seite alle Becher getroffen hat 
void loser_animation(int player){

}


// Point Up animation: Trifft eine Seite einen Becher, kurze Jubelanimation
void point_up_animation(int player){
    
}

// Jede LED leuchtet in unnterschiedlichen Farben abwechselnd auf
// Quelle: https://gist.github.com/atuline/02e71a57636498d382e276311b328e53
void loop() {

  basicfadein();
  FastLED.show();
  
} // loop()



void basicfadein() {

  random16_set_seed(535);                                                           
  for (int i = 0; i< sizeof(leds_infinity); i++) {
    uint8_t fader = sin8(millis()/random8(10,20));                                  
    leds_infinity[i] = CHSV(i*20,255, fader);                                                
  }

  for (int i = 0; i < sizeof(leds_triangle); i++)
  {
    uint8_t fader = sin8(millis()/random8(10,20));
    leds_triangle[i] = CHSV(i*20,255, fader);
 }
  

  random16_set_seed(millis());                                                      

} // basicfadein()
