#include <Arduino.h>
#include <FastLED.h>
#include <Bounce2.h>
#include <button_handler.h>

//wenn das funktioniert geht der Ständer von hier bis Düsseldorf Junge
// Pin Deklarationen von der Masterseite
#define PIN_LED_MAIN_LOGO 18
#define PIN_LED_MAIN_INF 19
#define PIN_LED_MAIN_TRI 21




#define PIN_POWER_SOURCE 15


// Pin Deklarationen der Slaveseite
#define PIN_LED_SLAVE_LOGO 22
#define PIN_LED_SLAVE_TRI 23
#define PIN_LED_SLAVE_INF 25



// Länge der einzelnen Streifen, beide Seiten sollten gleich lang sein
#define LED_NUM_INFINITY 65
#define LED_NUM_TRIANGLE 37
#define LED_NUM_LOGO 4  

// andere KOnstanten
#define RAINBOW_PERIOD 1  // Periode einer Umrundung des Regenbogens in Sekunden
#define FPS 60           // wie oft die Animation neu berechnet und die LEDs aktualisiert werden


// erstellen der ganzen CRGB Arrays für die Farben der Pixel, jeder Streifen hat sein eigenes Array
CRGB led_main_inf[LED_NUM_INFINITY] = CRGB::Black;
CRGB led_main_tri[LED_NUM_TRIANGLE] = CRGB::Black;
CRGB led_main_logo[LED_NUM_LOGO] = CRGB::Black;

CRGB led_slave_inf[LED_NUM_INFINITY] = CRGB::Black;
CRGB led_slave_tri[LED_NUM_TRIANGLE] = CRGB::Black;
CRGB led_slave_logo[LED_NUM_LOGO] = CRGB::Blue;


// für jede Gruppe an LEDs gibt es eine Helligkeit um zb das Logo unabhängig hell vom rest zu beleuchten
uint8_t led_inf_brightness = 32;
uint8_t led_tri_brightness = 32;
uint8_t led_logo_brightness = 200;

int lastH = 0;
float DELTA_H = 256.0/(RAINBOW_PERIOD*FPS);
float power_usage = 0;












CHSV rainbow_at_led(int currentLED, int numLED, uint8_t initH) {
  // erstelle einen Regenbogeneffekt und gebe die Farbe an der Stelle currentLED zurück
  // Berechne die Schrittweite für den H Anteil der Farbe
 
  float H = 256.0/numLED*currentLED + initH;  // Achtung auf Division nur mit Integern -> nur Ganzzahldivision, 256.0 erzeugt einen Float in der Gleihcung und deshalb wird eine Float Division gemacht
  if (H >= 256.0) {
    H -= 256.0;
  }
  //Serial.println("LED " + String(currentLED) + ": " + String(H));
  return CHSV(uint8_t(H), 255, 255); // für die aktuelle LED den H Anteil bestimmen, S und V auf max
}

uint8_t update_LED_rainbow(uint8_t deltaH, uint8_t lastH) {
  for (int i=0; i<LED_NUM_INFINITY; i++) {
    if ((lastH+deltaH)<=255) {
      // wenn die H Komponente im nächsten Schritt noch im Bereich 0-255 liegt
      led_main_inf[i] = rainbow_at_led(i, LED_NUM_INFINITY, lastH+deltaH);
      led_slave_inf[i] = rainbow_at_led(i, LED_NUM_INFINITY, lastH+deltaH);
    } else {
      // wenn das nächste H einen Wert größer 255 (>=256) hätte
      // übergib den Wert der Summe, der den "Überlauf" darstellt
      led_main_inf[i] = rainbow_at_led(i, LED_NUM_INFINITY, (lastH+deltaH)%255); 
      led_slave_inf[i] = rainbow_at_led(i, LED_NUM_INFINITY, (lastH+deltaH)%255);
    }
    
  }
  uint8_t nextH = lastH+deltaH;

  if (nextH > 256) {
    // wenn nextH 255 übersteigen sollte, dann gib den Übertrag zurück
    nextH = nextH%255;
  }
  //Serial.println("nextH = " + String(nextH));
  //Serial.println("=================================");
  return nextH;
}


void setup() {
  // alle Eingänge als Pullup Eingänge einstellen


  pinMode(PIN_POWER_SOURCE, INPUT_PULLUP);

 
 
 // Alle nötigen Schritte um die Knöpfe zu initialisieren
  setupButtons();

  Serial.begin(9600);
  delay(1000);



  // einmal die LEDs mit Regenbogen initialisieren
  for (int i=0; i<LED_NUM_INFINITY; i++) {
    CHSV hsv = rainbow_at_led(i, LED_NUM_INFINITY, 0);
    led_main_inf[i] = hsv;
    
  }

  for (int i=0; i<LED_NUM_TRIANGLE; i++) {
    CHSV hsv = rainbow_at_led(i, LED_NUM_TRIANGLE, 0);
    led_main_tri[i] = hsv;
    led_slave_tri[i] = hsv;
    
  }

  for (int i=0; i<LED_NUM_LOGO; i++) {
    led_main_logo[i] = CRGB::SkyBlue;
  }

  // die slave seite mit 1, 2 und 3 leds initialisieren
  led_slave_inf[0] = CRGB::Red;

  led_slave_tri[1] = CRGB::Red;



   // set up the LEDs
  FastLED.addLeds<WS2812B, PIN_LED_MAIN_INF, GRB>(led_main_inf, LED_NUM_INFINITY);
  FastLED.addLeds<WS2812B, PIN_LED_MAIN_TRI, GRB>(led_main_tri, LED_NUM_TRIANGLE);
  FastLED.addLeds<WS2812B, PIN_LED_MAIN_LOGO, GRB>(led_main_logo, LED_NUM_LOGO);

  FastLED.addLeds<WS2812B, PIN_LED_SLAVE_INF, GRB>(led_slave_inf, LED_NUM_INFINITY);
  FastLED.addLeds<WS2812B, PIN_LED_SLAVE_TRI, GRB>(led_slave_tri, LED_NUM_TRIANGLE);
  FastLED.addLeds<WS2812B, PIN_LED_SLAVE_LOGO, GRB>(led_slave_logo, LED_NUM_LOGO);




  FastLED[0].showLeds(led_inf_brightness);  // FastLED[0] setzt den LED Streifen außenrum
  FastLED[1].showLeds(led_tri_brightness);  // FastLED[1] setzt das Dreieck
  FastLED[2].showLeds(led_logo_brightness); // FastLED[2] setzt die Logo LEDs

  FastLED[3].showLeds(led_inf_brightness);  // FastLED[3] setzt den LED Streifen außenrum
  FastLED[4].showLeds(led_tri_brightness);  // FastLED[4] setzt das Dreieck
  FastLED[5].showLeds(led_logo_brightness); // FastLED[5] setzt die Logo LEDs

  Serial.println(calculate_unscaled_power_mW(led_main_inf, LED_NUM_INFINITY));

}

void loop() {
  // put your main code here, to run repeatedly:

  if(button_main_back.update() && button_main_back.read()){
    Serial.println("Knopf zuruck");
    // reduziere die Helligkeit um eins
    led_logo_brightness--;
  }
  if(button_main_mid.update() && button_main_mid.read()){
    Serial.println("Knopf mitte");
  }
  if(button_main_for.update() && button_main_for.read()){
    Serial.println("Knopf vor");
    // erhöhe die Helligkeit um eins
    led_logo_brightness++;
  }

  // begrenze die Helligkeit auf 0 - 255
  led_inf_brightness = constrain(led_inf_brightness, 0, 255);
  // berechne die LED Arrays mit Regenbogen neu
  lastH = update_LED_rainbow(DELTA_H, lastH);
  
  
  
  // die Farben der LEDs werden hier an die drei Streifen jeweils mit der Helligkeit in den Klammern gesendet, die Reihenfolge wurde im Setup festgelegt
  FastLED[0].showLeds(led_inf_brightness);
  FastLED[1].showLeds(led_tri_brightness);
  FastLED[2].showLeds(led_logo_brightness);

  FastLED[3].showLeds(led_inf_brightness);  // FastLED[3] setzt den LED Streifen außenrum
  FastLED[4].showLeds(led_tri_brightness);  // FastLED[4] setzt das Dreieck
  FastLED[5].showLeds(led_logo_brightness); // FastLED[5] setzt die Logo LEDs

  // ein Ansatz die Leistung der LEDs zu berechnen, abhängig davon ob Stromversorgung über USB oder Netzteil kann weniger oder mehr Leistung gezogen werden
  // daraus lassen sich dann die maximalen Helligkeiten berechnen
  power_usage = (calculate_unscaled_power_mW(led_main_inf, LED_NUM_INFINITY)*led_inf_brightness + calculate_unscaled_power_mW(led_main_tri, LED_NUM_TRIANGLE)*led_tri_brightness + calculate_unscaled_power_mW(led_main_logo, LED_NUM_LOGO)*led_logo_brightness)/255;
  //power_usage = (calculate_unscaled_power_mW(led_main_inf, LED_NUM_INFINITY) + calculate_unscaled_power_mW(led_main_tri, LED_NUM_TRIANGLE) + calculate_unscaled_power_mW(led_main_logo, LED_NUM_LOGO));

  Serial.println("Pges(mW) = " + String(power_usage) + " Iges(mA) = " + String(power_usage/5));
  Serial.println("======================");
  
  // irgendein Versuch eine Bildwiederholrate festzulegen, das klappt so aber nicht wirklich
  delay(1000/FPS);



}



