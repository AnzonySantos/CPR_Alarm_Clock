//Breif: Serves as the header file for the initalization functions (hardware, libraries, and data)
#include "initialization.h"
#include <navigation.h>
#include "globals.h"


void initalization(){
    if (debug_flag) {
        Serial.begin(9600);
    }
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
}

void initalize_data() {
    prefs.begin("alarm_clock", true);

    bright = prefs.getString("bright", "auto");
    dls_flag = prefs.getBool("dls", false);
    mil_time_flag = prefs.getBool("mil_time", false);

    global_clock_hours = prefs.getUChar("clk_hr", 0);
    global_clock_minutes = prefs.getUChar("clk_min", 0);
    global_clock_seconds = prefs.getUChar("clk_sec", 0);
    global_clock_month = prefs.getUChar("clk_mth", 9);
    global_clock_day = prefs.getUChar("clk_day", 20);
    global_clock_year = prefs.getUShort("clk_yr", 2026);

    alarms[0].toggle = prefs.getBool("alarm1toggle", false);
    alarms[1].toggle = prefs.getBool("alarm2toggle", false);
    alarms[2].toggle = prefs.getBool("alarm3toggle", false);

    alarms[0].hours = prefs.getUChar("alm1_hr", 0);
    alarms[0].minutes = prefs.getUChar("alm1_min", 0);
    alarms[0].second = prefs.getUChar("alm1_sec", 0);

    alarms[1].hours = prefs.getUChar("alm2_hr", 0);
    alarms[1].minutes = prefs.getUChar("alm2_min", 0);
    alarms[1].second = prefs.getUChar("alm2_sec", 0);

    alarms[2].hours = prefs.getUChar("alm3_hr", 0);
    alarms[2].minutes = prefs.getUChar("alm3_min", 0);
    alarms[2].second = prefs.getUChar("alm3_sec", 0);

    alarms[0].selected_sound = prefs.getUChar("alm1_snd", 1);
    alarms[1].selected_sound = prefs.getUChar("alm2_snd", 1);
    alarms[2].selected_sound = prefs.getUChar("alm3_snd", 1);

    alarms[0].snooze_amount = prefs.getUChar("alm1_snam", 0);
    alarms[1].snooze_amount = prefs.getUChar("alm2_snam", 0);
    alarms[2].snooze_amount = prefs.getUChar("alm3_snam", 0);

    alarms[0].snooze_delay = prefs.getUChar("alm1_sndl", 8);
    alarms[1].snooze_delay = prefs.getUChar("alm2_sndl", 8);
    alarms[2].snooze_delay = prefs.getUChar("alm3_sndl", 8);

    alarms[0].snooze_length = prefs.getUChar("alm1_snln", 15);
    alarms[1].snooze_length = prefs.getUChar("alm2_snln", 15);
    alarms[2].snooze_length = prefs.getUChar("alm3_snln", 15);

    prefs.end();
}

void backup_data() {
    prefs.begin("alarm_clock", false);

    prefs.putString("bright", bright);
    prefs.putBool("dls", dls_flag);
    prefs.putBool("mil_time", mil_time_flag);

    prefs.putUChar("clk_hr", global_clock_hours);
    prefs.putUChar("clk_min", global_clock_minutes);
    prefs.putUChar("clk_sec", global_clock_seconds);
    prefs.putUChar("clk_mth", global_clock_month);
    prefs.putUChar("clk_day", global_clock_day);
    prefs.putUShort("clk_yr", global_clock_year);

    prefs.putBool("alarm1toggle", alarms[0].toggle);
    prefs.putBool("alarm2toggle", alarms[1].toggle);
    prefs.putBool("alarm3toggle", alarms[2].toggle);

    prefs.putUChar("alm1_hr", alarms[0].hours);
    prefs.putUChar("alm1_min", alarms[0].minutes);
    prefs.putUChar("alm1_sec", alarms[0].second);

    prefs.putUChar("alm2_hr", alarms[1].hours);
    prefs.putUChar("alm2_min", alarms[1].minutes);
    prefs.putUChar("alm2_sec", alarms[1].second);

    prefs.putUChar("alm3_hr", alarms[2].hours);
    prefs.putUChar("alm3_min", alarms[2].minutes);
    prefs.putUChar("alm3_sec", alarms[2].second);

    prefs.putUChar("alm1_snd", alarms[0].selected_sound);
    prefs.putUChar("alm2_snd", alarms[1].selected_sound);
    prefs.putUChar("alm3_snd", alarms[2].selected_sound);

    prefs.putUChar("alm1_snam", alarms[0].snooze_amount);
    prefs.putUChar("alm2_snam", alarms[1].snooze_amount);
    prefs.putUChar("alm3_snam", alarms[2].snooze_amount);

    prefs.putUChar("alm1_sndl", alarms[0].snooze_delay);
    prefs.putUChar("alm2_sndl", alarms[1].snooze_delay);
    prefs.putUChar("alm3_sndl", alarms[2].snooze_delay);

    prefs.putUChar("alm1_snln", alarms[0].snooze_length);
    prefs.putUChar("alm2_snln", alarms[1].snooze_length);
    prefs.putUChar("alm3_snln", alarms[2].snooze_length);

    prefs.end();
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
                    if(a.day == 7)a.day = 6;
                    else a.day = 7;
                return;
                case 5://day
                    a.day_flag = !a.day_flag;
                return;
                case 6://month
                    a.month_flag = !a.month_flag;
                return;
                case 7://sound //snooze length changes delay
                    a.selected_sound = (a.selected_sound % 3) + 1;
                return;
                case 8://snooze length 5 -15 increments of 1 
                    a.snooze_delay_flag = !a.snooze_delay_flag;
                return;
                case 9://snooze:--
                    return;
                case 10://Ddely
                    if(a.alarm_durration_seconds == 15) a.alarm_durration_seconds = 30;
                    else if(a.alarm_durration_seconds == 30) a.alarm_durration_seconds = 60;
                    else if(a.alarm_durration_seconds == 60) a.alarm_durration_seconds = 180;
                    else a.alarm_durration_seconds = 15;
                return;
                case 11:
                    a.number_of_snoozes_flag = !a.number_of_snoozes_flag;//number of snoozes 0-10 integer
                    return;
                case 12:
                    back();
                    return;
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
    snooze_alarm = true;
}
