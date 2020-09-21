int current_state = -1;
#define FPS 120 // frames per seconds
#define STATE_NUM 4 // Anzahl an Zuständen
unsigned long last_millis = 0;
#include <button_handler.h>



void loop(){

    if ((millis() - last_millis) >= (1/FPS*1000)){
    // all dieser Code wird mit alle 1/FPS Sekunden ausgeführt
        last_millis = millis(); 

        if(button_main_mid.update() && button_main_mid.read()){
        // state bei Tastendruck wechseln
        current_state = (current_state + 1) % STATE_NUM;
        
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
            // Die Helligkeit des Logos soll sich nicht ändern lassen
            if (button_main_up.update() && button_main_up.read()){
                // Ist bereits der höchste Helligekeitswert erreicht soll nichts passieren
                if (led_inf_brightness != 224){
               led_inf_brightness += (32 % 256);
               led_tri_brightness += (32 % 256);  
            
                }
            }
                //power_usage = (calculate_unscaled_power_mW(led_main_inf, LED_NUM_INFINITY)*led_inf_brightness + calculate_unscaled_power_mW(led_main_tri, LED_NUM_TRIANGLE)*led_tri_brightness + calculate_unscaled_power_mW(led_main_logo, LED_NUM_LOGO)*led_logo_brightness)/255;
            if (button_main_down.update() && button_main_down.read()){
                // Ist bereits der niedrigste Helligekeitswert erreicht soll nichts passieren
                if(led_inf_brightness != 0 ){
               led_inf_brightness -= (32 % 256);
               led_tri_brightness -= (32 % 256);  
                }
            } 
                    
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