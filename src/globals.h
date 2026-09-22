// globals.h
#pragma once
#include <Arduino.h>
#include <vector>
#include <U8g2lib.h>
#include <RTClib.h>
#include <ESP32Encoder.h>
#include <Preferences.h>
#ifndef GLOBALS_H
#define GLOBALS_H

#include <Preferences.h>
extern Preferences prefs; 

// Alarm struct
struct AlarmSettings {
  bool toggle = false;

  int hours   = 0;
  int minutes = 0;
  int second  = 0;

  int day = 7;
  int month_day = 0;
  bool day_flag = false;
  int month = 0;
  bool month_flag = false;

  bool hours_flag   = false;
  bool minutes_flag = false;
  bool secs_flag    = false;

  int  selected_sound = 1;

  // day is authoritative: 0=Sunday through 6=Saturday, 7=Every.
  // Display text is derived; no separate editable day string.

  //5 -15 
  int  snooze_delay = 5;//snooze length 5 -15 increments of 1 
  bool snooze_delay_flag  = false;
  bool is_snooze = false; // is this alarm a temp snooze alarm

  int snooze_amount = 0;//number of snoozes 0-10 integer
  bool number_of_snoozes_flag = false;
  // Legacy navigation value; intentionally NOT mapped to sounding duration yet.
  int snooze_length = 0;
  int alarm_durration_seconds = 15;//15, 30, 60, 180

  //convert alarm time to seconds
  int toSeconds() const {
        return (hours * 3600) + (minutes * 60) + second;
    }
};
// Definitions live in globals.cpp. Main-loop code owns mutable state;
// button ISRs only publish events through initialization.cpp.
extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;
extern RTC_DS3231 rtc;
extern ESP32Encoder encoder;
extern Preferences prefs;
extern bool debug_flag;

extern std::vector<AlarmSettings> alarms;
extern unsigned long alarm_start_time;
extern bool buzzer_active;
extern int what_alarm; // -1 means no selected runtime alarm.
extern volatile bool stop_alarm;
extern volatile bool snooze_alarm;

extern bool in_menu; // true=menu, false=home (navigation AND display).
extern int x;
extern int y;
extern int64_t count;
extern bool x_level;
extern bool y_level;

// Preserve navigation's brightness spellings: auto/high/med/low.
extern String bright;
extern bool dls_flag;
extern bool mil_time_flag;
// Sensor acquisition remains for the integration pass; default is deterministic.
// extern uint16_t raw_brightness_value;

extern uint8_t global_clock_hours;
extern uint8_t global_clock_minutes;
extern uint8_t global_clock_seconds;
extern int global_clock_day;
extern int global_clock_month;
extern int global_clock_year;
extern int global_clock_weekday;
extern bool clock_hour_flag;
extern bool clock_minute_flag;
extern bool clock_second_flag;

extern bool is_inverted;

extern int y_max;
/*
extern uint8_t clock_hours;
extern uint8_t clock_mins;
extern uint8_t clock_sec;
*/


extern int x_max;
extern bool is_alarm;
extern int current_alarm_snooze_count;
const char* alarm_day_text(int day);

extern unsigned long last_button1_pressed_time;
extern unsigned long last_button2_pressed_time;
extern unsigned long last_button3_pressed_time;
#endif
