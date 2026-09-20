// globals.h
#pragma once
#include <Arduino.h>
#include <vector>

// Alarm struct
struct AlarmSettings {
  bool toggle = false;

  int hours   = 0;
  int minutes = 0;
  int second  = 0;

  int day = 0;
  int month_day = 0;
  int month = 0;

  bool hours_flag   = false;
  bool minutes_flag = false;
  bool secs_flag    = false;

  int  selected_sound = 1;

  String alarm_day = "Everyday";

  int  snooze_delay  = 5;
  int  snooze_amount = 5;
  bool snooze_delay_flag  = false;
  bool snooze_amount_flag = false;
  bool is_snooze = false; // is this alarm a temp snooze alarm

  int alarm_durration_seconds = 10;

  //convert alarm time to seconds
  int toSeconds() const {
        return (hours * 3600) + (minutes * 60) + second;
    }
};
/* 
Alarm Variables
*/
extern std::vector<AlarmSettings> alarms;
extern int selected_alarm;
extern bool buzzer_active;
extern int what_alarm;
extern volatile bool stop_alarm;
extern volatile bool snooze_alarm;

// other variables 
extern bool in_menu;