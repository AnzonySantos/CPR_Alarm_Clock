#include <Wire.h>
#include <U8g2lib.h>
#include <cstdint>
//TODO: Change many of the uint8_t type to a regular in or something else
//As we may want some negative values to make sure some things are unselected

extern bool autobrightness_flag;
extern bool debug_flag;
extern uint8_t global_brightness_level;
extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;

//8 bit value sent to OLED to tell it how bright its going to be 
#define LOW_BRIGHTNESS_LEVEL        255
#define MEDIUM_BRIGHTNESS_LEVEL     150
#define HIGH_BRIGHTNESS_LEVEL       50

void check_brightness();
void update_display(uint8_t x, int8_t y, bool in_menu);
void render_screen(uint8_t x, int8_t y, bool in_menu);

///May want to implement these a bit differently as im not entirely sure how this now fits with our navigation
void draw_home_screen();
void draw_options_screen(uint8_t x);
void draw_display_settings_screen(uint8_t y);
void draw_clock_settings_screen(uint8_t y);
void draw_alarm1_screen(uint8_t y);
void draw_alarm2_screen(uint8_t y);
void draw_alarm3_screen(uint8_t y);


void update_display(uint8_t x, int8_t y, bool in_menu){
    check_brightness();
    u8g2.clearBuffer();
    render_screen(x, y, in_menu);
    u8g2.sendBuffer();
}

void check_brightness(){
    uint16_t raw_brightness_value;
    if (autobrightness_flag){
        if (raw_brightness_value >= 2854 && raw_brightness_value <= 4095){
           u8g2.setContrast(HIGH_BRIGHTNESS_LEVEL); 
        }
        else if (raw_brightness_value >= 1613 && raw_brightness_value < 2854){
           u8g2.setContrast(MEDIUM_BRIGHTNESS_LEVEL); 
        }
        else if (raw_brightness_value >= 0 && raw_brightness_value < 1613){
           u8g2.setContrast(LOW_BRIGHTNESS_LEVEL); 
        }
        else{
            ///Maybe add serial print debug statement, if you get to this point somethings gone wrong lol
        }
    }
    else{
        if (global_brightness_level == 0){ //LOW
           u8g2.setContrast(LOW_BRIGHTNESS_LEVEL); 
        }
        else if (global_brightness_level == 1){ //MEDIUM
           u8g2.setContrast(MEDIUM_BRIGHTNESS_LEVEL); 
        }
        else if (global_brightness_level ==  2){ //HIGH
           u8g2.setContrast(HIGH_BRIGHTNESS_LEVEL); 
        }
        else {
            ///Again you could add a potential debug statement if things go wrong   
        }
    }
    return;
}


void render_screen(uint8_t x, int8_t y, bool in_menu){
    if (in_menu){
        draw_home_screen();
    }
    else{
        if (x_level){///Sential value could be changed if desired
            ///Could be made less verbose at this point im not sure if the "x's" are capped in between these numbers
            ///or not so this is a simple but kind ugly way to make sure only values 0-4 are displayed
            draw_options_screen(x);
        }
        else {
            switch(x){//For this to work as i envision the rotary coder must not be able to set the y value to something less than 0 
                case 0:
                    draw_display_settings_screen(y);
                break;
                case 1:
                    draw_clock_settings_screen(y);
                break;
                case 2:
                    draw_alarm1_screen(y);
                break;
                case 3:
                    draw_alarm2_screen(y);
                break;
                case 4:
                    draw_alarm3_screen(y);
                break;
            }
        }
    }
}


void draw_home_screen(){
    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(0, 10, data.date_str);
    uint16_t day_width = u8g2.getStrWidth(data.day_str);
    u8g2.drawStr(128 - day_width, 10, data.day_str);

    u8g2.setFont(u8g2_font_logisoso16_tf);
    uint16_t time_width = u8g2.getStrWidth(data.time_str);
    uint16_t time_x = (128 - time_width) / 2;
    u8g2.drawStr(time_x, 42, data.time_str);

    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(0, 63, "< Snooze");
    const char* menu_label = "Menu >";
    uint16_t menu_width = u8g2.getStrWidth(menu_label);
    u8g2.drawStr(128 - menu_width, 63, menu_label);
    u8g2.sendBuffer();
}

void draw_options_screen(uint8_t x){
    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(2, 12, "Main Menu");
    u8g2.setFont(u8g2_font_6x10_tr);
    switch (x){
        case 0:
            u8g2.drawStr(2, 28, ">");
            u8g2.drawStr(16, 28, "Display Settings");
            u8g2.drawStr(16, 44, "Clock Settings");
            u8g2.drawStr(16, 60, "Alarm 1");
            break;

        case 1:
            u8g2.drawStr(16, 28, "Display Settings");
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, "Clock Settings");
            u8g2.drawStr(16, 60, "Alarm 1");
            break;

        case 2:
            u8g2.drawStr(16, 28, "Clock Settings");
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, "Alarm 1");
            u8g2.drawStr(16, 60, "Alarm 2");
            break;

        case 3:
            u8g2.drawStr(16, 28, "Alarm 1");
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, "Alarm 2");
            u8g2.drawStr(16, 60, "Alarm 3");
            break;

        case 4:
            u8g2.drawStr(16, 28, "Alarm 1");
            u8g2.drawStr(16, 44, "Alarm 2");
            u8g2.drawStr(2, 60, ">");
            u8g2.drawStr(16, 60, "Alarm 3");
            break;
    }
}

void draw_display_settings_screen(uint8_t y){
    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(2, 12, "Display Settings");
    u8g2.setFont(u8g2_font_6x10_tr);
    switch(y){
        case 0:
            u8g2.drawStr(2, 28, ">");
            u8g2.drawStr(16, 28, ("Brightness: " + brightness.text).c_str());
            u8g2.drawStr(16, 44, ("Daylight Savings: " + daylight_savings.text).c_str());
            u8g2.drawStr(16, 60, ("Military Time: " + military_time.text).c_str());
            break;
        case 1:
            u8g2.drawStr(16, 28, ("Brightness: " + brightness.text).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Daylight Savings: " + daylight_savings.text).c_str());
            u8g2.drawStr(16, 60, ("Military Time: " + military_time.text).c_str());
            break;
        case 2:
            u8g2.drawStr(16, 28, ("Daylight Savings: " + daylight_savings.text).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Military Time: " + military_time.text).c_str());
            u8g2.drawStr(16, 60, "Back");
            break;
        case 3:
            u8g2.drawStr(16, 28, ("Daylight Savings: " + daylight_savings.text).c_str());
            u8g2.drawStr(16, 44, ("Military Time: " + military_time.text).c_str());
            u8g2.drawStr(2, 60, ">");
            u8g2.drawStr(16, 60, "Back");
            break;
    }
}

void draw_clock_settings_screen(uint8_t y){
    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(2, 12, "Clock Settings");
    u8g2.setFont(u8g2_font_6x10_tr);
    switch (y){
        case 0:
            u8g2.drawStr(2, 28, ">");
            u8g2.drawStr(16, 28, ("Hours: " + hours.text).c_str());
            u8g2.drawStr(16, 44, ("Minutes: " + minutes.text).c_str());
            u8g2.drawStr(16, 60, ("Seconds: " + seconds.text).c_str());
            break;
        case 1:
            u8g2.drawStr(16, 28, ("Hours: " + hours.text).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Minutes: " + minutes.text).c_str());
            u8g2.drawStr(16, 60, ("Seconds: " + seconds.text).c_str());
            break;
        case 2:
            u8g2.drawStr(16, 28, ("Hours: " + hours.text).c_str());
            u8g2.drawStr(16, 44, ("Minutes: " + minutes.text).c_str());
            u8g2.drawStr(2, 60, ">");
            u8g2.drawStr(16, 60, ("Seconds: " + seconds.text).c_str());
            break;
        case 3:
            u8g2.drawStr(16, 28, ("Minutes: " + minutes.text).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Seconds: " + seconds.text).c_str());
            u8g2.drawStr(16, 60, ("Day: " + day.text).c_str());
            break;
        case 4:
            u8g2.drawStr(16, 28, ("Seconds: " + seconds.text).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Day: " + day.text).c_str());
            u8g2.drawStr(16, 60, ("Month: " + month.text).c_str());
            break;
        case 5:
            u8g2.drawStr(16, 28, ("Day: " + day.text).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Month: " + month.text).c_str());
            u8g2.drawStr(16, 60, ("Year: " + year.text).c_str());
            break;
        case 6:
            u8g2.drawStr(16, 28, ("Month: " + month.text).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Year: " + year.text).c_str());
            u8g2.drawStr(16, 60, "Back");
            break;
        case 7:
            u8g2.drawStr(16, 28, ("Month: " + month.text).c_str());
            u8g2.drawStr(16, 44, ("Year: " + year.text).c_str());
            u8g2.drawStr(2, 60, ">");
            u8g2.drawStr(16, 60, "Back");
            break;
    }
}

void draw_alarm1_screen(uint8_t y){
    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(2, 12, "Alarm 1 Settings");
    u8g2.setFont(u8g2_font_6x10_tr);
    switch (y) {
        case 0:
            u8g2.drawStr(2, 28, ">");
            u8g2.drawStr(16, 28, ("Alarm: " + alarm.text).c_str());
            u8g2.drawStr(16, 44, ("Hours: " + hours.text).c_str());
            u8g2.drawStr(16, 60, ("Minutes: " + minutes.text).c_str());
            break;
        case 1:
            u8g2.drawStr(16, 28, ("Alarm: " + alarm.text).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Hours: " + hours.text).c_str());
            u8g2.drawStr(16, 60, ("Minutes: " + minutes.text).c_str());
            break;
        case 2:
            u8g2.drawStr(16, 28, ("Hours: " + hours.text).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Minutes: " + minutes.text).c_str());
            u8g2.drawStr(16, 60, ("Seconds: " + seconds.text).c_str());
            break;
        case 3:
            u8g2.drawStr(16, 28, ("Minutes: " + minutes.text).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Seconds: " + seconds.text).c_str());
            u8g2.drawStr(16, 60, ("Daily: " + daily.text).c_str());
            break;
        case 4:
            u8g2.drawStr(16, 28, ("Seconds: " + seconds.text).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Daily: " + daily.text).c_str());
            u8g2.drawStr(16, 60, ("Day: " + day.text).c_str());
            break;
        case 5:
            u8g2.drawStr(16, 28, ("Daily: " + daily.text).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Day: " + day.text).c_str());
            u8g2.drawStr(16, 60, ("Month: " + month.text).c_str());
            break;
        case 6:
            u8g2.drawStr(16, 28, ("Day: " + day.text).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Month: " + month.text).c_str());
            u8g2.drawStr(16, 60, ("Sound: " + sound.text).c_str());
            break;
        case 7:
            u8g2.drawStr(16, 28, ("Month: " + month.text).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Sound: " + sound.text).c_str());
            u8g2.drawStr(16, 60, ("Snooze Length: " + snooze_length.text).c_str());
            break;
        case 8:
            u8g2.drawStr(16, 28, ("Sound: " + sound.text).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Snooze Length: " + snooze_length.text).c_str());
            u8g2.drawStr(16, 60, ("Snooze: " + snooze.text).c_str());
            break;
        case 9:
            u8g2.drawStr(16, 28, ("Snooze Length: " + snooze_length.text).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Snooze: " + snooze.text).c_str());
            u8g2.drawStr(16, 60, ("Delay: " + delay_var.text).c_str());
            break;
        case 10:
            u8g2.drawStr(16, 28, ("Snooze: " + snooze.text).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Delay: " + delay_var.text).c_str());
            u8g2.drawStr(16, 60, ("Snooze Amount: " + snooze_amount.text).c_str());
            break;
        case 11:
            u8g2.drawStr(16, 28, ("Delay: " + delay_var.text).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Snooze Amount: " + snooze_amount.text).c_str());
            u8g2.drawStr(16, 60, "Back");
            break;
        case 12:
            u8g2.drawStr(16, 28, ("Delay: " + delay_var.text).c_str());
            u8g2.drawStr(16, 44, ("Snooze Amount: " + snooze_amount.text).c_str());
            u8g2.drawStr(2, 60, ">");
            u8g2.drawStr(16, 60, "Back");
            break;
    }
}
void draw_alarm2_screen(uint8_t y){
    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(2, 12, "Alarm 2 Settings");
    u8g2.setFont(u8g2_font_6x10_tr);
    switch (y) {
        case 0:
            u8g2.drawStr(2, 28, ">");
            u8g2.drawStr(16, 28, ("Alarm: " + alarm.text).c_str());
            u8g2.drawStr(16, 44, ("Hours: " + hours.text).c_str());
            u8g2.drawStr(16, 60, ("Minutes: " + minutes.text).c_str());
            break;
        case 1:
            u8g2.drawStr(16, 28, ("Alarm: " + alarm.text).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Hours: " + hours.text).c_str());
            u8g2.drawStr(16, 60, ("Minutes: " + minutes.text).c_str());
            break;
        case 2:
            u8g2.drawStr(16, 28, ("Hours: " + hours.text).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Minutes: " + minutes.text).c_str());
            u8g2.drawStr(16, 60, ("Seconds: " + seconds.text).c_str());
            break;
        case 3:
            u8g2.drawStr(16, 28, ("Minutes: " + minutes.text).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Seconds: " + seconds.text).c_str());
            u8g2.drawStr(16, 60, ("Daily: " + daily.text).c_str());
            break;
        case 4:
            u8g2.drawStr(16, 28, ("Seconds: " + seconds.text).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Daily: " + daily.text).c_str());
            u8g2.drawStr(16, 60, ("Day: " + day.text).c_str());
            break;
        case 5:
            u8g2.drawStr(16, 28, ("Daily: " + daily.text).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Day: " + day.text).c_str());
            u8g2.drawStr(16, 60, ("Month: " + month.text).c_str());
            break;
        case 6:
            u8g2.drawStr(16, 28, ("Day: " + day.text).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Month: " + month.text).c_str());
            u8g2.drawStr(16, 60, ("Sound: " + sound.text).c_str());
            break;
        case 7:
            u8g2.drawStr(16, 28, ("Month: " + month.text).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Sound: " + sound.text).c_str());
            u8g2.drawStr(16, 60, ("Snooze Length: " + snooze_length.text).c_str());
            break;
        case 8:
            u8g2.drawStr(16, 28, ("Sound: " + sound.text).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Snooze Length: " + snooze_length.text).c_str());
            u8g2.drawStr(16, 60, ("Snooze: " + snooze.text).c_str());
            break;
        case 9:
            u8g2.drawStr(16, 28, ("Snooze Length: " + snooze_length.text).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Snooze: " + snooze.text).c_str());
            u8g2.drawStr(16, 60, ("Delay: " + delay_var.text).c_str());
            break;
        case 10:
            u8g2.drawStr(16, 28, ("Snooze: " + snooze.text).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Delay: " + delay_var.text).c_str());
            u8g2.drawStr(16, 60, ("Snooze Amount: " + snooze_amount.text).c_str());
            break;
        case 11:
            u8g2.drawStr(16, 28, ("Delay: " + delay_var.text).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Snooze Amount: " + snooze_amount.text).c_str());
            u8g2.drawStr(16, 60, "Back");
            break;
        case 12:
            u8g2.drawStr(16, 28, ("Delay: " + delay_var.text).c_str());
            u8g2.drawStr(16, 44, ("Snooze Amount: " + snooze_amount.text).c_str());
            u8g2.drawStr(2, 60, ">");
            u8g2.drawStr(16, 60, "Back");
            break;
    }
}

void draw_alarm3_screen(uint8_t y){
    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(2, 12, "Alarm 3 Settings");
    u8g2.setFont(u8g2_font_6x10_tr);
    switch (y) {
        case 0:
            u8g2.drawStr(2, 28, ">");
            u8g2.drawStr(16, 28, ("Alarm: " + alarm.text).c_str());
            u8g2.drawStr(16, 44, ("Hours: " + hours.text).c_str());
            u8g2.drawStr(16, 60, ("Minutes: " + minutes.text).c_str());
            break;
        case 1:
            u8g2.drawStr(16, 28, ("Alarm: " + alarm.text).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Hours: " + hours.text).c_str());
            u8g2.drawStr(16, 60, ("Minutes: " + minutes.text).c_str());
            break;
        case 2:
            u8g2.drawStr(16, 28, ("Hours: " + hours.text).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Minutes: " + minutes.text).c_str());
            u8g2.drawStr(16, 60, ("Seconds: " + seconds.text).c_str());
            break;
        case 3:
            u8g2.drawStr(16, 28, ("Minutes: " + minutes.text).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Seconds: " + seconds.text).c_str());
            u8g2.drawStr(16, 60, ("Daily: " + daily.text).c_str());
            break;
        case 4:
            u8g2.drawStr(16, 28, ("Seconds: " + seconds.text).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Daily: " + daily.text).c_str());
            u8g2.drawStr(16, 60, ("Day: " + day.text).c_str());
            break;
        case 5:
            u8g2.drawStr(16, 28, ("Daily: " + daily.text).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Day: " + day.text).c_str());
            u8g2.drawStr(16, 60, ("Month: " + month.text).c_str());
            break;
        case 6:
            u8g2.drawStr(16, 28, ("Day: " + day.text).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Month: " + month.text).c_str());
            u8g2.drawStr(16, 60, ("Sound: " + sound.text).c_str());
            break;
        case 7:
            u8g2.drawStr(16, 28, ("Month: " + month.text).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Sound: " + sound.text).c_str());
            u8g2.drawStr(16, 60, ("Snooze Length: " + snooze_length.text).c_str());
            break;
        case 8:
            u8g2.drawStr(16, 28, ("Sound: " + sound.text).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Snooze Length: " + snooze_length.text).c_str());
            u8g2.drawStr(16, 60, ("Snooze: " + snooze.text).c_str());
            break;
        case 9:
            u8g2.drawStr(16, 28, ("Snooze Length: " + snooze_length.text).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Snooze: " + snooze.text).c_str());
            u8g2.drawStr(16, 60, ("Delay: " + delay_var.text).c_str());
            break;
        case 10:
            u8g2.drawStr(16, 28, ("Snooze: " + snooze.text).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Delay: " + delay_var.text).c_str());
            u8g2.drawStr(16, 60, ("Snooze Amount: " + snooze_amount.text).c_str());
            break;
        case 11:
            u8g2.drawStr(16, 28, ("Delay: " + delay_var.text).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Snooze Amount: " + snooze_amount.text).c_str());
            u8g2.drawStr(16, 60, "Back");
            break;
        case 12:
            u8g2.drawStr(16, 28, ("Delay: " + delay_var.text).c_str());
            u8g2.drawStr(16, 44, ("Snooze Amount: " + snooze_amount.text).c_str());
            u8g2.drawStr(2, 60, ">");
            u8g2.drawStr(16, 60, "Back");
            break;
    }
}
