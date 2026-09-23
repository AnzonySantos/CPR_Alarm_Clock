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

//nav vars
int y_max = 9;
int x_max = 4;
uint8_t clock_hours = 0;
uint8_t clock_mins = 0;
uint8_t clock_sec = 0;

//delay vars
unsigned long previousMillis = 0;
const unsigned long interval = 500;

void setup() {
  initalization();
}


void loop() {
  update_display(x, y, in_menu);
  count = encoder.getCount();
  if(clock_dst_needs_updated){
    set_rtc_time(global_clock_year, global_clock_month, global_clock_day, global_clock_hours, global_clock_minutes, global_clock_seconds);
    clock_dst_needs_updated = false;
  }
  if(in_menu){
    if(x_level){
      x += count/2;
      if(x < 0){
        x = 0;
      }
      if(x > 4){
        x = 4;
      }
      count = 0;
      encoder.clearCount();
    }
    else if(y_level){
  if(what_alarm >= 0 && what_alarm < alarms.size() && alarms[what_alarm].hours_flag){
    alarms[what_alarm].hours += count/2;
    if(alarms[what_alarm].hours > 23){
      alarms[what_alarm].hours = 0;
    }
    if(alarms[what_alarm].hours < 0){
      alarms[what_alarm].hours = 23;
    }
    count = 0;
    encoder.clearCount();
  }
  else if(what_alarm >= 0 && what_alarm < alarms.size() && alarms[what_alarm].minutes_flag){
    alarms[what_alarm].minutes += count/2;
    if(alarms[what_alarm].minutes > 59){
      alarms[what_alarm].minutes = 0;
    }
    if(alarms[what_alarm].minutes < 0){
      alarms[what_alarm].minutes = 59;
    }
    count = 0;
    encoder.clearCount();
  }
  else if(what_alarm >= 0 && what_alarm < alarms.size() && alarms[what_alarm].secs_flag){
    alarms[what_alarm].second += count/2;
    if(alarms[what_alarm].second > 59){
      alarms[what_alarm].second = 0;
    }
    if(alarms[what_alarm].second < 0){
      alarms[what_alarm].second = 59;
    }
    count = 0;
    encoder.clearCount();
  }
  else if(what_alarm >= 0 && what_alarm < alarms.size() && alarms[what_alarm].snooze_delay_flag){//0-15
    alarms[what_alarm].snooze_delay += count/2;
    if(alarms[what_alarm].snooze_delay > 15){
      alarms[what_alarm].snooze_delay = 5;
    }
    if(alarms[what_alarm].snooze_delay < 5){
      alarms[what_alarm].snooze_delay = 15;
    }
    count = 0;
    encoder.clearCount();
  }
  else if(what_alarm >= 0 && what_alarm < alarms.size() && alarms[what_alarm].number_of_snoozes_flag){//0-10
    alarms[what_alarm].snooze_amount += count/2;
    if(alarms[what_alarm].snooze_amount > 10){
      alarms[what_alarm].snooze_amount = 0;
    }
    if(alarms[what_alarm].snooze_amount < 0){
      alarms[what_alarm].snooze_amount = 10;
    }
    count = 0;
    encoder.clearCount();
  }
    else if(what_alarm >= 0 && what_alarm < alarms.size() && alarms[what_alarm].day_flag){//1-31
    alarms[what_alarm].month_day += count/2;
    if(alarms[what_alarm].month_day > 31){
      alarms[what_alarm].month_day = 1;
    }
    if(alarms[what_alarm].month_day < 1){
      alarms[what_alarm].month_day = 31;
    }
    count = 0;
    encoder.clearCount();
  }
  else if(what_alarm >= 0 && what_alarm < alarms.size() && alarms[what_alarm].month_flag){//1-12
    alarms[what_alarm].month += count/2;
    if(alarms[what_alarm].month > 12){
      alarms[what_alarm].month = 1;
    }
    if(alarms[what_alarm].month < 1){
      alarms[what_alarm].month = 12;
    }
    count = 0;
    encoder.clearCount();
  }
  else if(clock_hour_flag){
    global_clock_hours += count/2;
    if(global_clock_hours > 23){
      global_clock_hours = 0;
    }
    if(global_clock_hours < 0){
      global_clock_hours = 23;
    }
    count = 0;
    encoder.clearCount();
    set_rtc_time(global_clock_year, global_clock_month, global_clock_day, global_clock_hours, global_clock_minutes, global_clock_seconds);
  }
  else if(clock_minute_flag){
    global_clock_minutes += count/2;
    if(global_clock_minutes > 59){
      global_clock_minutes = 0;
    }
    if(global_clock_minutes < 0){
      global_clock_minutes = 59;
    }
    count = 0;
    encoder.clearCount();
    set_rtc_time(global_clock_year, global_clock_month, global_clock_day, global_clock_hours, global_clock_minutes, global_clock_seconds);
  }
  else if(clock_second_flag){
    global_clock_seconds += count/2;
    if(global_clock_seconds > 59){
      global_clock_seconds = 0;
    }
    if(global_clock_seconds < 0){
      global_clock_seconds = 59;
    }
    count = 0;
    encoder.clearCount();
    set_rtc_time(global_clock_year, global_clock_month, global_clock_day, global_clock_hours, global_clock_minutes, global_clock_seconds);
  }
    else if(clock_day_flag){
    global_clock_day += count/2;
    if(global_clock_day > 31){
      global_clock_day = 1;
    }
    if(global_clock_day < 1){
      global_clock_day = 31;
    }
    count = 0;
    encoder.clearCount();
    set_rtc_time(global_clock_year, global_clock_month, global_clock_day, global_clock_hours, global_clock_minutes, global_clock_seconds);
  }
  else if(clock_month_flag){
    global_clock_month += count/2;
    if(global_clock_month > 12){
      global_clock_month = 1;
    }
    if(global_clock_month < 1){
      global_clock_month = 12;
    }
    count = 0;
    encoder.clearCount();
    set_rtc_time(global_clock_year, global_clock_month, global_clock_day, global_clock_hours, global_clock_minutes, global_clock_seconds);
  }
  else if(clock_year_flag){
    global_clock_year += count/2;
    count = 0;
    encoder.clearCount();
    set_rtc_time(global_clock_year, global_clock_month, global_clock_day, global_clock_hours, global_clock_minutes, global_clock_seconds);
  }
  else if(x == 4 || x == 2 || x == 3){
    y += count/2 % 12;
    if(y < 0){
      y = 0;
    }
    if(y > 12){
      y = 12;
    }
    count = 0;
    encoder.clearCount();
  }
  else if(x == 0){
    y += count/2 % 4;
    if(y < 0){
      y = 0;
    }
    if(y > 3){
      y = 3;
    }
    count = 0;
    encoder.clearCount();
  }
  else if(x == 1){
    y += count/2 % 6;
    if(y < 0){
      y = 0;
    }
    if(y > 6){
      y = 6;
    }
    count = 0;
    encoder.clearCount();
  }
    }
  }

  //alarm logic
  for (int i = 0; i < alarms.size(); i++){
    if(current_time_seconds() == alarms[i].toSeconds() && (rtc.now().dayOfTheWeek() == alarms[i].day || alarms[i].day == 7 || (rtc.now().day() == alarms[i].month_day && rtc.now().month() == alarms[i].month)) && !buzzer_active){ // correct time and day [7=everydy], and buzzer isnt active
      alarm_start_time = millis(); // when the alarm started
      buzzer_active = true; // the alarm is sounding
      if(alarms[i].is_snooze == false){ // if this is not a snooze alarm, set the current_alarm_snooze_count to the number of snoozes for this alarm
        if(alarms[i].snooze_amount == 0){
          current_alarm_snooze_count = 99999;
        }
        else{
          current_alarm_snooze_count = (alarms[i].snooze_amount + 1);
        }
      }
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
      what_alarm = i; // track what alarm should be sounded
    }
  }
  if(buzzer_active) {
    // has the alarm reached its set duration yet
    unsigned long elapsed_ms = millis() - alarm_start_time;
    bool duration_expired = elapsed_ms >= (unsigned long)(alarms[what_alarm].alarm_durration_seconds * 1000);
    unsigned long currentMillis = millis();

    if(stop_alarm || duration_expired || snooze_alarm ) { // if button is pressed or duration has expired or snoozed
      stop_sound(); // stop sound
      buzzer_active = false;
      is_alarm = false;
      stop_alarm = false;
      if(snooze_alarm == true){
        snooze_alarm = false;
 // decrement the snooze count
        if(current_alarm_snooze_count != 0){
         current_alarm_snooze_count=  current_alarm_snooze_count-1;
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
      }

      // delete the alarm if it is a temp snooze alarm
      if(alarms[what_alarm].is_snooze == true){
        alarms.erase(alarms.begin() + what_alarm);
      }
      
    } else {
      // inverting the display
      if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      toggle_invert(is_inverted);
      is_inverted = !is_inverted;
      }
      play_sound(alarms[what_alarm].selected_sound);
    }
  }else{
    revert_display();
    is_inverted = false;
  }
  backup_data();
}
