#ifndef NAVIGATION_H
#define NAVIGATION_H_H

#include 

extern bool in_menu;
extern int x;
extern int y;
extern bool alarm_flag;

extern bool alarm1_toggle;
extern bool alarm2_toggle;
extern bool alarm3_toggle;

extern bool alarm1_hours_flag;
extern bool alarm2_hours_flag;
extern bool alarm3_hours_flag;

extern bool alarm1_minutes_flag;
extern bool alarm2_minutes_flag;
extern bool alarm3_minutes_flag;

extern bool alarm1_sec_flag;
extern bool alarm2_sec_flag;
extern bool alarm3_sec_flag;

extern int selected_sound1;
extern int selected_sound2;
extern int selected_sound3;

extern bool snooze1_delay_flag;
extern bool snooze2_delay_flag;
extern bool snooze3_delay_flag;

extern std::string alarm_day1;
extern std::string alarm_day2;
extern std::string alarm_day3;

extern bool snooze1_amount_flag;
extern bool snooze2_amount_flag;
extern bool snooze3_amount_flag;

extern int snooze1_length;
extern int snooze2_length;
extern int snooze3_length;

extern bool dls_flag;
extern bool mil_time_flag;

extern bool clock_hour_flag;
extern bool clock_minute_flag;
extern bool clock_second_flag;

extern bool x_level;
extern bool y_level;

extern std::string bright;

#endif