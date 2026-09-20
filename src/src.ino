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

//Hardware Iniitalization Objects
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(
  U8G2_R0,
  U8X8_PIN_NONE
  );
  
RTC_DS3231 rtc;
ESP32Encoder encoder;
Preferences prefs;

bool debug_flag = 0; // If you want the serial monitor to be on so you can print debug statemnts, here you go 

void setup() {
  initalization();
}

void loop() {



  //wills navigation logic 
  if(in_menu){
    if(x_level){
      x = count/2%5;
      if(x < 0){
        //this logic for x may need to be tweaked a bit
        x = x_max + x;
      }
    }
    else if(y_level){
      if(alarm1_hours_flag){
        alarm1_hours += count/2;
        alarm1_hours = alarm1_hours%24;
        count = 0;
      }
      else if(alarm1_minutes_flag){
        alarm1_mins += count/2;
        alarm1_mins = alarm1_mins%60;
        count = 0;
      }
      else if(alarm1_sec_flag){
        alarm1_sec += count/2;
        alarm1_sec = alarm1_sec%60;
        count = 0;
      }
      else if(snooze1_delay_flag){
        snooze1_delay += count/2;
        if(snooze1_delay > 15){
          snooze1_delay = 5;
        }
        count = 0
      }
      else if(snooze1_amount_flag){
        snooze1_amount += count/2;
        if(snooze1_amount > 10){
          snooze1_amount = 0;
        }
        count = 0;
      }
      else if(alarm2_hours_flag){
        alarm2_hours += count/2;
        alarm2_hours = alarm2_hours%24;
        count = 0;
      }
      else if(alarm2_minutes_flag){
        alarm2_mins += count/2;
        alarm2_mins = alarm2_mins%60;
        count = 0;
      }
      else if(alarm2_sec_flag){
        alarm2_sec += count/2;
        alarm2_sec = alarm2_sec%60;
        count = 0;
      }
      else if(snooze2_delay_flag){
        snooze2_delay += count/2;
        if(snooze2_delay > 15){
          snooze2_delay = 5;
        }
        count = 0
      }
      else if(snooze2_amount_flag){
        snooze2_amount += count/2;
        if(snooze2_amount > 10){
          snooze2_amount = 0;
        }
        count = 0;
      }
      else if(alarm3_hours_flag){
        alarm3_hours += count/2;
        alarm3_hours = alarm3_hours%24;
        count = 0;
      }
      else if(alarm3_minutes_flag){
        alarm3_mins += count/2;
        alarm3_mins = alarm3_mins%60;
        count = 0;
      }
      else if(alarm3_sec_flag){
        alarm3_sec += count/2;
        alarm3_sec = alarm3_sec%60;
        count = 0;
      }
      else if(snooze3_delay_flag){
        snooze3_delay += count/2;
        if(snooze3_delay > 15){
          snooze3_delay = 5;
        }
        count = 0
      }
      else if(snooze3_amount_flag){
        snooze3_amount += count/2;
        if(snooze3_amount > 10){
          snooze3_amount = 0;
        }
        count = 0;
      }
      else if(clock_hours_flag){
        clock_hours += count/2;
        clock_hours = clock_hours%24;
        count = 0;
      }
      else if(clock_minutes_flag){
        clock_mins += count/2;
        clock_mins = clock_mins%60;
        count = 0;
      }
      else if(clock_sec_flag){
        clock_sec += count/2;
        clock_sec = clock_sec%60;
        count = 0;
      }
      else if(x == 1 || x == 2 || x == 3){
        y = count/2%10;
        if(y < 0){
          y = y_max + y;
        }
      }
      else if(x == 4 || x==0){
        y = count/2%4;
        if(y < 0){
          y = y_max + y;
        }
      }

    }
  }
}
