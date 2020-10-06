#include <Arduino.h>
#include <Bounce2.h>

#define PIN_BUTTON_MAIN_1 26
#define PIN_BUTTON_MAIN_2 0
#define PIN_BUTTON_MAIN_3 32

#define PIN_BUTTON_SLAVE_1 2
#define PIN_BUTTON_SLAVE_2 4
#define PIN_BUTTON_SLAVE_3 5

// Um die Eingabe zu kontrollieren wird die Bounce2 Bibliothek verwendet, für jeden Knopf eine Instanz
Bounce button_main_down = Bounce();
Bounce button_main_mid = Bounce();
Bounce button_main_up = Bounce();

Bounce button_slave_down = Bounce();
Bounce button_slave_mid = Bounce();
Bounce button_slave_up = Bounce();

//Bounce button_slave_back = Bounce();
//Bounce button_slave_mid = Bounce();
//Bounce button_slave_for = Bounce();

void setupButtons() {
    pinMode(PIN_BUTTON_MAIN_1, INPUT_PULLUP);
    pinMode(PIN_BUTTON_MAIN_2, INPUT_PULLUP);
    pinMode(PIN_BUTTON_MAIN_3, INPUT_PULLUP);

    pinMode(PIN_BUTTON_SLAVE_1, INPUT_PULLUP);
    pinMode(PIN_BUTTON_SLAVE_2, INPUT_PULLUP);
    pinMode(PIN_BUTTON_SLAVE_3, INPUT_PULLUP);

    button_main_down.attach(PIN_BUTTON_MAIN_3);
    button_main_mid.attach(PIN_BUTTON_MAIN_2);
    button_main_up.attach(PIN_BUTTON_MAIN_1);

    button_main_down.interval(5);
    button_main_mid.interval(5);
    button_main_up.interval(5);

    button_slave_down.attach(PIN_BUTTON_SLAVE_3);
    button_slave_mid.attach(PIN_BUTTON_SLAVE_2);
    button_slave_up.attach(PIN_BUTTON_SLAVE_1);

    button_slave_down.interval(5);
    button_slave_mid.interval(5);
    button_slave_up.interval(5);
}

bool button_activity_main(){
    // wenn sich der Status von einem Button geändert hat gebe 1 zurück
    return button_main_down.update() || button_main_mid.update() || button_main_up.update();
}