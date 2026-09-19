//Brief: Logic for many of the functions called to
//interface with the hardware. May want to split up into different files
//but I thought this would be a nice place for now.
#include "hardware_helpers.h"

void invert_display(){
    u8g2.sendF("c", INVERT_DISPLAY_COMMAND);
}
void revert_display(){
    u8g2.sendF("c", REVERT_DISPLAY_COMMAND);
}

void toggle_invert(bool is_inverted){
    if (is_inverted){
        revert_display();
    }
    else{
        invert_display();
    }
}

void stop_sound(){
    ledcWriteTone(BUZZER_PIN, OFF);
}

void play_sound(uint8_t tone){
    switch(tone){
        case 1:
            ledcWriteTone(BUZZER_PIN, TONE1); 
        break;

        case 2:
            ledcWriteTone(BUZZER_PIN, TONE2); 
        break;
        
        case 3:
            ledcWriteTone(BUZZER_PIN, TONE3); 
        break;

        default:
            if (debug_flag){
                Serial.println("Error: Undefined Tone Value");
            }
        break;
    }
}

///Gets the time from the rtc in a 24hr format
void get_rtc_time(){
    ///Note: If you really wanted to you could also get year, month, and day information from here
    DateTime current_rtc_time = rtc.now();
    global_clock_hours = current_rtc_time.hour();
    global_clock_minutes = current_rtc_time.minute();
    global_clock_seconds = current_rtc_time.second();
}
void set_rtc_time(uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second){
    ///May want to include guards and debug prints, but for now it should be fine.
    rtc.adjust(DateTime(year, month, day, hour, minute, second));
}
