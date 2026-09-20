// globals.h
#pragma once
#include <Arduino.h>

struct AlarmSettings {
  bool toggle = false;

  int  hours   = 0;
  int  minutes = 0;
  int  second  = 0;

  bool hours_flag   = false;
  bool minutes_flag = false;
  bool secs_flag    = false;

  int  selected_sound = 1;

  String alarm_day = "Everyday";

  int  snooze_delay  = 5;
  int  snooze_amount = 5;
  bool snooze_delay_flag  = false;
  bool snooze_amount_flag = false;

  //convert alarm time to seconds
  int toSeconds() const {
        return (hours * 3600) + (minutes * 60) + seconds;
    }
};

extern AlarmSettings alarms[3];
extern int selected_alarm;