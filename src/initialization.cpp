//Breif: Serves as the header file for the initalization functions (hardware, libraries, and data)
#include "initialization.h"

void initalization(){
    hardware_setup();
    library_setup();
    initalize_data();
}


void hardware_setup(){
    //Pin Assignments
    ledcAttach(BUZZER_PIN, PLACE_HOLDER_FREQUENCY, PWM_RESOLUTION);
    pinMode(BRIGHTNESS_PIN, INPUT);
    pinMode(ROT_ENC_BUTTON_PIN, INPUT_PULLUP);
    pinMode(PUSH_BUTTON2_PIN, INPUT_PULLUP);
    pinMode(PUSH_BUTTON3_PIN, INPUT_PULLUP);

    //Encoder Initalization
    ESP32Encoder::useInternalWeakPullResistors = puType::up;
    encoder.attachHalfQuad(ROT_ENC_A_PIN, ROT_ENC_B_PIN);
    encoder.clearCount();

    //Interrupt Declarations
    attachInterrupt(ROT_ENC_BUTTON_PIN, button1_callback, FALLING);
    attachInterrupt(PUSH_BUTTON2_PIN, button2_callback, FALLING);
    attachInterrupt(PUSH_BUTTON3_PIN, button3_callback, FALLING);
}

void library_setup(){
    //Library Initalization
    Wire.begin();
    u8g2.begin();
    rtc.begin();
    prefs.begin("alarm_clock_state_variables", false);

    return;
}
void initalize_data(){
    ///TODO: Ask group what should be stored in flash, and what the default values should be for all variables. Then intialize all
    //state variables here.
    return;
}

//Note to self: Button ISR's. Rename if you'd like they're just here as they are needed to compile. If issues arise when removing or renaming them
//make sure to update the "attachInterrupt(<PIN>, <ISR>, <EDGE>)" functions seen above in the interrupt declarations. If some more issues
//arise make sure they are linked by including the apropriate header file.
void button1_callback(){
    return;
}
void button2_callback(){
    return;
}
void button3_callback(){
    return;
}
