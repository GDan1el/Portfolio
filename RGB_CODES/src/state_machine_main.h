#include <Arduino.h>


int current_state = 0;
int last_state = -1;

#define STATE_NUM 4 // Anzahl an Zuständen
#define MENU_TIMEOUT 5000 // nach XXX Millisekunden soll von jedem Menüpunkt wieder ins Hauptmenü gesprungen werden
#define FPS 120 // Aktualisierungsrate der Berechnungen und LEDs
unsigned long last_millis = 0;
unsigned long last_input = 0;   // speichert den letzten input um nach gewisser Zeit ins Hauptmenü zurück zu springen

void update_stm(){

    if ((millis() - last_millis) >= (1/FPS*1000)){
    // all dieser Code wird alle 1/FPS Sekunden ausgeführt
        last_millis = millis(); 

        if(button_main_mid.update() && button_main_mid.read()){
        // state bei Tastendruck wechseln
        current_state = (current_state + 1) % STATE_NUM;
        // speicher Zeitpunkt der letzten Eingabe
        last_input = millis();
        
        }

        if ((millis() - last_input) >=  MENU_TIMEOUT && current_state != 0){
            // Springe zurück ins Hauptmenü
            last_state = current_state; // noch nicht ganz klar ob dieser Schritt Sinn macht
            current_state = 0;
        }
    


        if (current_state == 0){    // Hauptmenü, Punkte verstellen
            // bei vor taste: Punkte erhöhen und Animation
            // bei zurück Taste: keine besondere Animation, Punktestand "ausbessern"
            // allgemein Punkte anzeigen
            

        }

        if (current_state == 1){    // Effekte auswählen 
            // alle möglichen Effekte auswählen lassen
            // vermutlich noch einen Zustandsautomaten


        }

        if (current_state == 2){    // Farbauswahl
            // HSV Farbraum
            //  Button_up/down verändert H Wert, also den Farbwert
            // if Button_Up: H += 5

            
        }

        if (current_state == 3){    // Helligkeit einstellen
            // Mit den beiden Tasten die Helligkeit ändern


            
        }

        if (current_state == 4){    // Logofarbe wählen
            // HSV Farbraum
            //  Button_up/down verändert H Wert, also den Farbwert
            // if Button_Up: H += 5

            if(button_main_up.update() && button_main_up.read()){
               main_logo_H += 32;
               if (main_logo_H >= 256) {
                   main_logo_H = 0;
               } 
             }

            if(button_main_down.update() && button_main_down.read()){
               main_logo_H -= 32;   
               // muss man mal gucken wie sich das mit den uint8_t verhält, eventuell kommt es bei der Subtraktion schon zu einem Fehler oder Überlauf
               if (main_logo_H <= 0) {
                   main_logo_H = 255;
               } 
             } 

            
        }


    }

   
    









}