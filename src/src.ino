//Note: This is the entry point to the application, I would have called it something normal like main.ino,
// but the geniuses behind the Arduino IDE require the folder and the entry point name to be the same.

#include <Wire.h>
#include <U8g2lib.h>
#include <RTClib.h>
#include <ESP32Encoder.h>
#include <Preferences.h>
#include <navigation.h>
#include "initialization.h"
#include "hardware_helpers.h"
#include "globals.h"
#include "display.h"


/* Nav Variables */
int y_max = 9;
int x_max = 4;
uint8_t clock_hours = 0;
uint8_t clock_mins = 0;
uint8_t clock_sec = 0;

/* Ran once on setup */
void setup() {
  initalization();

}

/* Main Loop */
void loop() {
  update_display(x, y, in_menu);
  count = encoder.getCount();
  if(in_menu){
    if(x_level){
      x = count/2 % 5;
      if(x < 0){
        x = x_max + x;
      }
      count = 0;
    }
    else if(y_level){
  if(what_alarm >= 0 && what_alarm < alarms.size() && alarms[what_alarm].hours_flag){
    alarms[what_alarm].hours += count/2;
    alarms[what_alarm].hours = alarms[what_alarm].hours % 24;
    count = 0;
    encoder.clearCount();
  }
  else if(what_alarm >= 0 && what_alarm < alarms.size() && alarms[what_alarm].minutes_flag){
    alarms[what_alarm].minutes += count/2;
    alarms[what_alarm].minutes = alarms[what_alarm].minutes % 60;
    count = 0;
    encoder.clearCount();
  }
  else if(what_alarm >= 0 && what_alarm < alarms.size() && alarms[what_alarm].secs_flag){
    alarms[what_alarm].second += count/2;
    alarms[what_alarm].second = alarms[what_alarm].second % 60;
    count = 0;
    encoder.clearCount();
  }
  else if(what_alarm >= 0 && what_alarm < alarms.size() && alarms[what_alarm].snooze_delay_flag){//0-15
    alarms[what_alarm].snooze_delay += count/2;
    if(alarms[what_alarm].snooze_delay > 15){
      alarms[what_alarm].snooze_delay = 5;
    }
    count = 0;
    encoder.clearCount();
  }
  else if(what_alarm >= 0 && what_alarm < alarms.size() && alarms[what_alarm].number_of_snoozes_flag){//0-10
    alarms[what_alarm].snooze_amount += count/2;
    if(alarms[what_alarm].snooze_amount > 10){
      alarms[what_alarm].snooze_amount = 0;
    }
    count = 0;
    encoder.clearCount();
  }
    else if(what_alarm >= 0 && what_alarm < alarms.size() && alarms[what_alarm].day_flag){//1-31
    alarms[what_alarm].month_day += count/2;
    if(alarms[what_alarm].month_day > 31){
      alarms[what_alarm].month_day = 1;
    }
    count = 0;
    encoder.clearCount();
  }
  else if(what_alarm >= 0 && what_alarm < alarms.size() && alarms[what_alarm].month_flag){//1-12
    alarms[what_alarm].month += count/2;
    if(alarms[what_alarm].month > 12){
      alarms[what_alarm].month = 1;
    }
    count = 0;
    encoder.clearCount();
  }
  else if(clock_hour_flag){
    clock_hours += count/2;
    clock_hours = clock_hours % 24;
    count = 0;
    encoder.clearCount();
  }
  else if(clock_minute_flag){
    clock_mins += count/2;
    clock_mins = clock_mins % 60;
    count = 0;
    encoder.clearCount();
  }
  else if(clock_second_flag){
    clock_sec += count/2;
    clock_sec = clock_sec % 60;
    count = 0;
    encoder.clearCount();
  }
  else if(x == 4 || x == 2 || x == 3){
    y += count/2 % 12;
    if(y < 0){
      y = y_max + y;
    }
    count = 0;
    encoder.clearCount();
  }
  else if(x == 0){
    y += count/2 % 4;
    if(y < 0){
      y = y_max + y;
    }
    count = 0;
    encoder.clearCount();
  }
  else if(x == 1){
    y += count/2 % 7;
    if(y < 0){
      y = y_max + y;
    }
    count = 0;
    encoder.clearCount();
  }
    }
  }

  /* 
  ALARM PORTION
  */
  for (int i = 0; i < alarms.size(); i++){
    if(current_time_seconds() == alarms[i].toSeconds() && (rtc.now().dayOfTheWeek() == alarms[i].day || alarms[i].day == 7 || (rtc.now().day() == alarms[i].month_day && rtc.now().month() == alarms[i].month)) && !buzzer_active){ // correct time and day [7=everydy], and buzzer isnt active
      alarm_start_time = millis(); // when the alarm started
      buzzer_active = true; // the alarm is sounding
      is_alarm = true;
      in_menu = false;
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
      play_sound(1);
      what_alarm = i; // track what alarm should be sounded
    }
  }
  //DURATION CHECK
  if(buzzer_active) {
    // has the alarm reached its set duration yet
    unsigned long elapsed_ms = millis() - alarm_start_time;
    bool duration_expired = elapsed_ms >= (unsigned long)(alarms[what_alarm].alarm_durration_seconds * 1000);

    if(stop_alarm || duration_expired || snooze_alarm ) { // if button is pressed or duration has expired or snoozed
      stop_sound(); // stop sound
      buzzer_active = false;
      is_alarm = false;
      stop_alarm = false; // reset variable
      // IF WE ARE SNOOZING
      if(snooze_alarm == true){
        snooze_alarm = false; // reset variable 

        //find the time the new snooze alarm should be set to (in seconds)
        long snooze_time_seconds_delay = (current_time_seconds() + (alarms[what_alarm].snooze_delay * 60)) % 86400;

        // variables to set the new snooze alarm
        long snooze_time_hours = snooze_time_seconds_delay / 3600; 
        long snooze_time_minutes = (snooze_time_seconds_delay % 3600) / 60;
        long snooze_time_seconds = snooze_time_seconds_delay % 60;

        AlarmSettings snooze_new_alarm;
        snooze_new_alarm.hours = snooze_time_hours;
        snooze_new_alarm.minutes = snooze_time_minutes;
        snooze_new_alarm.second = snooze_time_seconds;
        snooze_new_alarm.toggle = true;
        snooze_new_alarm.is_snooze = true; // indicating this is a snooze alarm
        snooze_new_alarm.snooze_delay = alarms[what_alarm].snooze_delay;
        snooze_new_alarm.selected_sound = alarms[what_alarm].selected_sound;

        alarms.push_back(snooze_new_alarm);
      }

      // delete the alarm if it is a temp snooze alarm
      if(alarms[what_alarm].is_snooze == true){
        alarms.erase(alarms.begin() + what_alarm);
      }
      
    } else {
      play_sound(alarms[what_alarm].selected_sound);
    }
  }
  backup_data();
}
