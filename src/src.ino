//Note: This is the entry point to the application, I would have called it something normal like main.ino,
// but the geniuses behind the Arduino IDE require the folder and the entry point name to be the same.

#include <Wire.h>
#include <U8g2lib.h>
#include <RTClib.h>
#include <Globals.h>
#include <ESP32Encoder.h>
#include <Preferences.h>
#include "initialization.h"
#include "hardware_helpers.h"

uint8_t global_clock_hours;
uint8_t global_clock_minutes;
uint8_t global_clock_seconds;

// Hardware Iniitalization Objects
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(
  U8G2_R0,
  U8X8_PIN_NONE
  );
  
RTC_DS3231 rtc;
ESP32Encoder encoder;
Preferences prefs;

bool debug_flag = 0; // If you want the serial monitor to be on so you can print debug statemnts, here you go 

bool in_menu = false;

/* Alarm Variables */
std::vector<AlarmSettings> alarms(3);
unsigned long alarm_start_time = 0;
bool buzzer_active = false;
bool is_alarm = false;
int what_alarm = -1;
volatile bool stop_alarm = false;
volatile bool snooze_alarm = false;

/* Ran once on setup */
void setup() {
  initalization();

}

/* Main Loop */
void loop() {
  /* 
  ALARM PORTION
  */
  for (int i = 0; i < alarms.size(); i++){
    if(current_time_seconds() == alarms[i].toSeconds() && (rtc.now().dayOfTheWeek() == alarms[i].day || alarms[i].day == 7 || (rtc.now().day() == alarms[i].month_day && rtc.now().month() == alarms[i].month)) && !buzzer_active){ // correct time and day [7=everydy], and buzzer isnt active
      alarm_start_time = millis(); // when the alarm started
      buzzer_active = true; // the alarm is sounding
      is_alarm = true;
      in_menu = false;
      what_alarm = i; // track what alarm should be sounded
      break;
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
}
