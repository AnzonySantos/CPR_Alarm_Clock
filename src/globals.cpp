#include "globals.h"

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
RTC_DS3231 rtc;
ESP32Encoder encoder;
Preferences prefs;
bool debug_flag = false;

std::vector<AlarmSettings> alarms(3);
unsigned long alarm_start_time = 0;
bool buzzer_active = false;
int what_alarm = -1;
volatile bool stop_alarm = false;
volatile bool snooze_alarm = false;
bool is_alarm = false;

bool in_menu = false;
int x = 0;
int y = 0;
int64_t count = 0;
bool x_level = true;
bool y_level = false;
String bright = "auto";
bool dls_flag = false;
bool mil_time_flag = false;

// uint16_t raw_brightness_value = 0;


uint16_t raw_brightness_value = 0;
int current_alarm_snooze_count = 0;


uint8_t global_clock_hours = 0;
uint8_t global_clock_minutes = 0;
uint8_t global_clock_seconds = 0;
int global_clock_day = 1;
int global_clock_month = 1;
int global_clock_year = 2000;
int global_clock_weekday = 6;
bool clock_hour_flag = false;
bool clock_minute_flag = false;
bool clock_second_flag = false;

const char* alarm_day_text(int day) {
    static const char* const names[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Every"};
    return day >= 0 && day <= 7 ? names[day] : "?";
}


unsigned long last_button1_pressed_time = 0;
unsigned long last_button2_pressed_time = 0;
unsigned long last_button3_pressed_time = 0;
