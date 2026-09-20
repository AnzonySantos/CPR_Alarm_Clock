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

}
