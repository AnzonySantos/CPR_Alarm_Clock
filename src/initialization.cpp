//Breif: Serves as the header file for the initalization functions (hardware, libraries, and data)
#include "initialization.h"
#include <navigation.h>
#include "globals.h"

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
    attachInterrupt(ROT_ENC_BUTTON_PIN, button2_callback, FALLING);
    attachInterrupt(PUSH_BUTTON2_PIN, button1_callback, FALLING);
    attachInterrupt(PUSH_BUTTON3_PIN, button3_callback, FALLING);
}

void library_setup(){
    //Library Initalization
    Wire.begin();
    u8g2.begin();
    rtc.begin();
    prefs.begin("alarm_clock_state_variables", false);
    
    if (debug_flag){
        Serial.begin(9600);
    }
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
//menu button
void button1_callback(){
    in_menu = !in_menu;
    count = 0;
    x = 0;
    y = 0;
    x_level = true;
    y_level = false;
}
//select button
void back(){//back helps with navigation
    count = 0;
    y = 0;
    x_level = true;
    y_level = false;
}
String next_day(String day){ //helps with navigation
    if (day == "Every")return "Mon";
    if (day == "Mon")return "Tue";
    if (day == "Tue")return "Wed";
    if (day == "Wed")return "Thu";
    if (day == "Thu")return "Fri";
    if (day == "Fri")return "Sat";
    if (day == "Sat")return "Sun";
    if (day == "Sun")return "Every";
}
int next_snooze_length(int length){ //helps with navigation
    if (length == 0) return 15;
    if (length == 15) return 30;
    if (length == 30) return 60;
    if (length == 60) return 0;
}
void button2_callback(){
    count = 0;
    if (!in_menu){
        return;
    }
    if (x_level){
        x_level = false;
        y_level = true;
    }
    if (!y_level){
        return;
    }
    switch(x){
        case 0:
        //display settings
            switch(y){
                case 0:
                    if(bright == "auto"){
                        bright = "high";
                    }
                    else if(bright == "high"){
                        bright = "med";
                    }
                    else if (bright == "med"){
                        bright = "low";
                    }
                    else{
                        bright = "auto";
                    }
                return;
                case 1:
                    dls_flag = !dls_flag;
                return;
                case 2:
                    mil_time_flag = !mil_time_flag;
                return;
                case 3:
                    back();
                return;
                default:
                return;
            }
        return;

        case 1:
        case 2:
        case 3: {
            int alarm_index = x - 1;   // x=1 -> alarms[0], x=2 -> alarms[1], x=3 -> alarms[2]
            AlarmSettings &a = alarms[alarm_index];
            what_alarm = alarm_index;

            switch(y){
                case 0:
                    a.toggle = !a.toggle;
                return;
                case 1:
                    a.hours_flag = !a.hours_flag;
                return;
                case 2:
                    a.minutes_flag = !a.minutes_flag;
                return;
                case 3:
                    a.secs_flag = !a.secs_flag;
                return;
                case 4:
                    a.selected_sound = (a.selected_sound + 1) % 3;
                return;
                case 5:
                    a.snooze_delay_flag = !a.snooze_delay_flag;
                return;
                case 6:
                    a.day = (a.day + 1) % 8;
                return;
                case 7:
                    a.snooze_amount_flag = !a.snooze_amount_flag;
                return;
                case 8:
                return;   // snooze_length placeholder — see note below
                case 9:
                    back();
                return;
                default:
                return;
            }
        }
        return;

        case 4:
        //clock settings
            switch(y){
                case 0:
                clock_hour_flag = !clock_hour_flag;
                return;
                case 1:
                clock_minute_flag = !clock_minute_flag;
                return;
                case 2:
                clock_second_flag = !clock_second_flag;
                return;
                case 3:
                back();
                return;
                default:
                return;
            }
        return;

        default:
        return;
    }
}
void button3_callback(){
    return;
}
