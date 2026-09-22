//Brief: Serves as the header file for the initalization functions

#include <Wire.h>
#include <U8g2lib.h>
#include <RTClib.h>
#include <ESP32Encoder.h>
#include <Preferences.h>

#ifndef INITIALIZATION_H
#define INITIALIZATION_H

///Pindefinitions
#define BUZZER_PIN              6
#define BRIGHTNESS_PIN          4
#define SDA_PIN                 8
#define SCL_PIN                 9
#define ROT_ENC_BUTTON_PIN      5
#define PUSH_BUTTON2_PIN        17
#define PUSH_BUTTON3_PIN        16
#define ROT_ENC_A_PIN           15
#define ROT_ENC_B_PIN           7

///Audio
#define PLACE_HOLDER_FREQUENCY  1000
#define PWM_RESOLUTION          8

#define  DEBOUNCE_DELAY_MS      250


//Hardware Objects. We may just want to move the initialization and their declaration here.
extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;
extern RTC_DS3231 rtc;
extern ESP32Encoder encoder;
extern Preferences prefs;
extern bool debug_flag;

///HelperFunctions
void initalization();
void hardware_setup();
void library_setup();
void initalize_data();
void backup_data();

//Button ISR's Note: Rename and if you'd like they're just here as they are needed to compile
void button1_callback();
void button2_callback();
void button3_callback();

#endif //INITIALIZATION_H
