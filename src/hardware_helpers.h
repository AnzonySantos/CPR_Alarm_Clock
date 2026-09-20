///Brief: Header file for the hardware helper functions
#ifndef HARDWARE_HELPERS_H
#define HARDWARE_HELPERS_H

#include "globals.h"
#include "initialization.h"

///Frequency Of Tones and Duty Cycle
#define OFF                         0
#define TONE1                       1000
#define TONE2                       2000
#define TONE3                       4000
#define DUTY_CYCLE                  50

///Display I2C Commands
#define INVERT_DISPLAY_COMMAND      0xA7
#define REVERT_DISPLAY_COMMAND      0xA6

///Display invert functions
void invert_display();
void revert_display();
void toggle_invert(bool is_inverted);

///RTC clock functions
void set_rtc_time(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second);
void get_rtc_time();///Need to include some parameter, however im not sure what that struct looks like yet

///Audio Functions
void stop_sound();
void play_sound(uint8_t tone);
//next_snooze_length()

//Alarm functions
int current_time_seconds();

#endif //HARDWARE_HELPERS_H
