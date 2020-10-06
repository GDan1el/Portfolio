#include <Arduino.h>
#include <FastLED.h>
#include <Bounce2.h>
#include <..\include\button_handler.h>

// Variablen für Zustandsautomat und co
#define FPS 120 // frames per seconds
#define STATE_NUM 4 // Anzahl an Zuständen
unsigned long last_millis = 0;
int current_state_main = 0;
#define BUTTON_TIMEOUT 5000     // nach wie vielen ms oder Button-Eingabe wieder ins Hauptmenu zurückgesprungen wird
unsigned long last_button_input_main = 0;

// Pin Deklarationen von der Hauptseite
#define PIN_LED_MAIN_LOGO 18
#define PIN_LED_MAIN_INF 19
#define PIN_LED_MAIN_TRI 21

#define PIN_POWER_SOURCE 15

// Pin Deklarationen der Nebenseite
#define PIN_LED_SLAVE_LOGO 22
#define PIN_LED_SLAVE_TRI 23
#define PIN_LED_SLAVE_INF 25

// Länge der einzelnen Streifen, beide Seiten sollten gleich lang sein
#define LED_NUM_INFINITY 65
#define LED_NUM_TRIANGLE 37
#define LED_NUM_LOGO 4  

// andere Werte
#define RAINBOW_PERIOD 1  // Periode einer Umrundung des Regenbogens in Sekunden

int lastH = 0;
float DELTA_H = 256.0/(RAINBOW_PERIOD*FPS);
float power_usage = 0;
float power_limit = 0;    // Leistungslimit für LEDs, USB: 12000mW, DC: 30000mW
#define USB_POWER_LIMIT 12000
#define DC_POWER_LIMIT 25000

uint8_t score_main = 0;
uint8_t score_slave = 0;
bool happy_animation = false;




// alle CRGB Arrays für die Farben der Pixel, jeder Streifen hat sein eigenes Array
CRGB led_main_inf[LED_NUM_INFINITY] = CRGB::Black;
CRGB led_main_tri[LED_NUM_TRIANGLE] = CRGB::Black;
CRGB led_main_logo[LED_NUM_LOGO] = CRGB::Red;

CRGB led_slave_inf[LED_NUM_INFINITY] = CRGB::Black;
CRGB led_slave_tri[LED_NUM_TRIANGLE] = CRGB::Black;
CRGB led_slave_logo[LED_NUM_LOGO] = CRGB::Blue;

// für jede Gruppe an LEDs gibt es eine Helligkeit um zb das Logo unabhängig hell vom rest zu beleuchten
// diese beiden Helligkeiten sind allgemein für eine Spielseite über die Eingabe steuerbar
uint8_t brightness_level_main = 0;
uint8_t main_brightness = 0;
uint8_t slave_brightness = 0;

// Die einzelnen Helligkeiten werden dann vom Programm selber angesteuert
uint8_t led_inf_brightness = 0;
uint8_t led_tri_brightness = 0;
uint8_t led_logo_brightness = 200;

uint8_t main_logo_H = 0;  // Farbwert für die Logo LEDs der Main Seite
uint8_t slave_logo_H = 128;   // Frabwert für Slave Seite ~ blau


void setup(){
    // Pin als Eingang mit Pullup Widerstand definieren
    pinMode(PIN_POWER_SOURCE, INPUT_PULLUP);
    // die anderen Buttons werden gesondert deklariert
    setupButtons();

    Serial.begin(19200);
    delay(1000);

    // initialisiere alle LEDs
    FastLED.addLeds<WS2812B, PIN_LED_MAIN_INF, GRB>(led_main_inf, LED_NUM_INFINITY);
    FastLED.addLeds<WS2812B, PIN_LED_MAIN_TRI, GRB>(led_main_tri, LED_NUM_TRIANGLE);
    FastLED.addLeds<WS2812B, PIN_LED_MAIN_LOGO, GRB>(led_main_logo, LED_NUM_LOGO);

    FastLED.addLeds<WS2812B, PIN_LED_SLAVE_INF, GRB>(led_slave_inf, LED_NUM_INFINITY);
    FastLED.addLeds<WS2812B, PIN_LED_SLAVE_TRI, GRB>(led_slave_tri, LED_NUM_TRIANGLE);
    FastLED.addLeds<WS2812B, PIN_LED_SLAVE_LOGO, GRB>(led_slave_logo, LED_NUM_LOGO);

    // Hauptseite
    FastLED[0].showLeds(led_inf_brightness);  // FastLED[0] setzt den LED Streifen außenrum
    FastLED[1].showLeds(led_tri_brightness);  // FastLED[1] setzt das Dreieck
    FastLED[2].showLeds(led_logo_brightness); // FastLED[2] setzt die Logo LEDs
    // Nebenseite
    FastLED[3].showLeds(led_inf_brightness);  // FastLED[3] setzt den LED Streifen außenrum
    FastLED[4].showLeds(led_tri_brightness);  // FastLED[4] setzt das Dreieck
    FastLED[5].showLeds(led_logo_brightness); // FastLED[5] setzt die Logo LEDs

    Serial.println("Setup komplett.");
}



void loop(){

    if ((millis() - last_millis) >= (1/FPS*1000)){
    // all dieser Code wird mit alle 1/FPS Sekunden ausgeführt
        last_millis = millis(); 

        // Überprüfe die Art der Stromversorgung und setze dementsrprechend die maximale Leisung
        if (digitalRead(PIN_POWER_SOURCE))  // wenn Pin auf 1 steht, also ein DC Stecker eingesteckt ist
        {
            power_limit = DC_POWER_LIMIT;
        }  
        else    // Gerät wird über USB versorgt
        {
            power_limit = USB_POWER_LIMIT;
        }

        // Abfrage der Eingabeaktivität
        if (button_activity_main())
        {
            // wenn ein Button gedrückt wurde
            // Speichere den Zeitpunkt der letzten Button-Eingabe
            last_button_input_main = millis();
            Serial.println("Button betätigt: " + String(millis()));
        }
        else    // wenn keine eingabe registriert wurde
        {
            if ((millis() - last_button_input_main) >= BUTTON_TIMEOUT)  // guck ob die letzte Eingabe länger als Timeout her ist
            {
                current_state_main = 0;     // kehre ins "Hauptmenü" zurück
                Serial.println("Keine Eingabe Hauptseite seit " + String(BUTTON_TIMEOUT) + "ms. Zurück ins Hauptmenü.");
            }
        }
        
        
        
        // mittlerer Button wechselt den Hauptzustand
        if(button_main_mid.update() && button_main_mid.read()){
            // state bei Tastendruck wechseln
            current_state_main = (current_state_main + 1) % STATE_NUM;
        }
    

        // ###################### Zustandsautomat Hauptseite #############################
        if (current_state_main == 0){    // Hauptmenü, Punkte verstellen
            // bei vor taste: Punkte erhöhen und Animation
            // bei zurück Taste: keine besondere Animation, Punktestand "ausbessern"
            // allgemein Punkte anzeigen
            if (button_main_up.update() && button_main_up.read())
            {
                score_main += 1;    // Punktestand eins erhöhen
                happy_animation = true; // eine fröhliche Animation abspielen
            }
            if (button_main_down.update() && button_main_down.read())
            {
                score_main -= 1;    // Punktestand verrringer
            }
            score_main = constrain(score_main, 0, 10);   // den Punktestand auf 0 - 10 begrenzen

        }

        if (current_state_main == 1){    // Effekte auswählen 
            // alle möglichen Effekte auswählen lassen
            // vermutlich noch einen Zustandsautomaten


        }

        if (current_state_main == 2){    // Farbauswahl
            // HSV Farbraum
            //  Button_up/down verändert H Wert, also den Farbwert
            // if Button_Up: H += 5

            
        }

        if (current_state_main == 3){    // Helligkeit einstellen
            // Mit den beiden Tasten die Helligkeit ändern
            // Die Helligkeit des Logos soll sich nicht ändern lassen
            if (button_main_up.update() && button_main_up.read()){
                // Helligkeitsstufe erhöhen
                brightness_level_main++;
            }
                //power_usage = (calculate_unscaled_power_mW(led_main_inf, LED_NUM_INFINITY)*led_inf_brightness + calculate_unscaled_power_mW(led_main_tri, LED_NUM_TRIANGLE)*led_tri_brightness + calculate_unscaled_power_mW(led_main_logo, LED_NUM_LOGO)*led_logo_brightness)/255;
            if (button_main_down.update() && button_main_down.read()){
                // Helligkeitsstufe verringern
                brightness_level_main--;  
            } 
            // Es gibt 10 Helligkeitsstufen, je nach Animation und Stromverbrauch kann eine Stufe unterschiedlich hell sein
            brightness_level_main = constrain(brightness_level_main, 1, 10);

            // Helligkeiten für die individuellen Streifen neu berechnen
                    
        }

        if (current_state_main == 4){    // Logofarbe wählen
            // HSV Farbraum
            //  Button_up/down verändert H Wert, also den Farbwert
            // if Button_Up: H += 5

            if(button_main_up.update() && button_main_up.read()){
               main_logo_H += 32;   // bei einem Überlauf springt die unsigned Variable wieder auf 0 zurück
               //zB 250 + 36 = 30 
            }

            if(button_main_down.update() && button_main_down.read()){
               main_logo_H -= 32;   // Der Überlauf funktioniert auch andersherum
               // 5 - 30 = 230
             } 

            
        }


    }

   
    









}