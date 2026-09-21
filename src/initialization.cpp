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
    for(int i = 0; i < 3; i++){
        alarms[i].hours_flag = false;
        alarms[i].minutes_flag = false;
        alarms[i].secs_flag = false;
        alarms[i].day_flag = false;
        alarms[i].month_flag = false;
        alarms[i].snooze_delay_flag = false;
        alarms[i].number_of_snoozes_flag = false; 


    }
}
//select button
void back(){//back helps with navigation
    count = 0;
    y = 0;
    x_level = true;
    y_level = false;
}

void button2_callback(){
    count = 0;
    encoder.clearCount();
    if (!in_menu){
        return;
    }
    if (x_level){
        x_level = false;
        y_level = true;
    }
    else{

    switch(x){
        case 0:
        //display settings
            switch(y){
                case 0:
                    if(bright == "auto"){ bright = "high"; }
                    else if(bright == "high"){ bright = "med"; }
                    else if (bright == "med"){ bright = "low"; }
                    else{ bright = "auto"; }
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

        case 2:
        case 3:
        case 4: {
            int alarm_index = x - 2;   // x=2 -> alarms[0], x=3 -> alarms[1], x=4 -> alarms[2]
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
                case 4://this is now daily 
                    if(a.day = 7)a.day = 6;
                    else a.day = 7;
                return;
                case 5://day
                    a.day_flag = !a.day_flag;
                return;
                case 6://month
                    a.month_flag = !a.month_flag;
                return;
                case 7://sound //snooze length changes delay
                    if(a.selected_sound == 1)a.selected_sound = 2;
                    if(a.selected_sound == 2) a.selected_sound = 3;
                    else a.selected_sound = 1;
                return;
                case 8://snooze length 5 -15 increments of 1 
                    a.snooze_delay_flag = !a.snooze_delay_flag;
                return;
                case 9://snooze:--
                    return;
                case 12://Ddely
                    if(a.alarm_durration_seconds == 15) a.alarm_durration_seconds = 30;
                    else if(a.alarm_durration_seconds == 30) a.alarm_durration_seconds = 60;
                    else if(a.alarm_durration_seconds == 60) a.alarm_durration_seconds = 180;
                    else a.alarm_durration_seconds = 15;
                return;
                case 13:
                    a.number_of_snoozes_flag = !a.number_of_snoozes_flag;//number of snoozes 0-10 integer
                    return;
                case 14:
                    back();
                default:
                    return;
                //back
            }
        }
        return;

        default:
        return;
    }
    }
}
void button3_callback(){
    return;
}
