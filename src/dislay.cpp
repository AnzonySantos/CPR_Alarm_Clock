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

///May want to implement these a bit differently as im not entily sure how this now fits with our navigation
void draw_home_screen();
void draw_options_screen(uint8_t x);
void draw_display_settings_screen(uint8_t y);
void draw_alarm1_screen(uint8_t y);
void draw_alarm2_screen(uint8_t y);
void draw_alarm3_screen(uint8_t y);
void draw_clock_screen(uint8_t y);

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
        if (y == -1){///Sential value could be changed if desired
            ///Could be made less verbose at this point im not sure if the "x's" are capped in between these numbers
            ///or not so this is a simple but kind ugly way to make sure only values 0-4 are displayed
            switch(x){
                case 0:
                    draw_options_screen(0);
                break;
                case 1:
                    draw_options_screen(1);
                break;
                case 2:
                    draw_options_screen(2);
                break;
                case 3:
                    draw_options_screen(3);
                break;
                case 4:
                    draw_options_screen(4);
                default:
                    //debugging and error handling
                break;
            }
        }
        else {
            switch(x){//For this to work as i envision the rotary coder must not be able to set the y value to something less than 0 
                case 0:
                    draw_display_settings_screen(y);
                break;
                case 1:
                    draw_alarm1_screen(y);
                break;
                case 2:
                    draw_alarm2_screen(y);
                break;
                case 3:
                    draw_alarm3_screen(y);
                break;
                case 4:
                    draw_clock_screen(y);
                break;
            }
        }
    }
}
