//Breif: Serves as the header file for the initalization functions (hardware, libraries, and data)
#include "initialization.h"
#include <navigation.h>
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
string next_day(std::string day){ //helps with navigation
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
        //if we are not in menu we are not selecting
        return;
    }
    if (x_level){
        //if we are in x_level we must go to y level
        x_level = false;
        y_level = true;
    }
    if (!y_level){
        //this is bad should not branch here
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
                    back();//to be defined
                return;
                default:
                return;
            }
        return;
        case 1:
            switch(y){
                case 0:
                alarm1_toggle = !alarm1_toggle;
                return;
                case 1:
                alarm1_hours_flag = !alarm1_hours_flag;
                return;
                case 2:
                alarm1_minutes_flag = !alarm1_minutes_flag;
                return;
                case 3:
                alarm1_sec_flag = !alarm1_sec_flag;
                return;
                case 4:
                selected_sound1 = % 3;
                return;
                case 5:
                snooze1_delay_flag = !snooze1_delay_flag;
                return;
                case 6:
                alarm_day1 = next_day(alarm_day1);//to be defined
                return;
                case 7:
                snooze1_amount_flag = !snooze1_amount_flag;
                return;
                case 8:
                snooze1_length = next_snooze_length(snooze1_length);//to be defined
                return;
                case 9:
                back();//to be defined
                return;
                default:
                return;
            }
        //alarm 1
        return;
        case 2:
        //alarm 2
            switch(y)
                case 0:
                alarm2_toggle = !alarm2_toggle;
                case 1:
                alarm2_hours_flag = !alarm2_hours_flag;
                case 2:
                alarm2_minutes_flag = !alarm2_minutes_flag;
                case 3:
                alarm2_sec_flag = !alarm2_sec_flag;
                case 4:
                selected_sound2 = % 3;
                case 5:
                snooze2_delay_flag = !snooze2_delay_flag;
                case 6:
                alarm_day2 = next_day(alarm_day2);//to be defined
                case 7:
                snooze2_amount_flag = !snooze2_amount_flag;
                case 8:
                snooze2_length = next_snooze_length(snooze2_length);//to be defined
                case 9:
                back();//to be defined
                default:
                return;
        return;
        case 3:
        //alarm 3
            switch(y):
                case 0:
                alarm3_toggle = !alarm3_toggle;
                case 1:
                alarm3_hours_flag = !alarm3_hours_flag;
                case 2:
                alarm3_minutes_flag = !alarm3_minutes_flag;
                case 3:
                alarm3_sec_flag = !alarm3_sec_flag;
                case 4:
                selected_sound3 = % 3;
                case 5:
                snooze3_delay_flag = !snooze3_delay_flag;
                case 6:
                alarm_day3 = next_day(alarm_day3);//to be defined
                case 7:
                snooze3_amount_flag = !snooze3_amount_flag;
                case 8:
                snooze3_length = next_snooze_length(snooze3_length);//to be defined
                case 9:
                back();//to be defined
                default:
                return;
        return;
        case 4:
        //clock settings
            switch(y):
                case 0:
                clock_hour_flag = !clock_hour_flag;
                case 1:
                clock_minute_flag = !clock_minute_flag;
                case 2:
                clock_second_flag = !clock_second_flag;
                case 3:
                back();//to be defined
                default:
                return;
        return;
        default:
        //bad
        return;

    }
}
void button3_callback(){
    return;
}
