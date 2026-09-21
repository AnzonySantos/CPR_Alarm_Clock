#include "display.h"
#include "globals.h"
#include <cstdio>


void update_display(int x, int y, bool in_menu){
    check_brightness();
    u8g2.clearBuffer();
    render_screen(x, y, in_menu);
    u8g2.sendBuffer();
}

void check_brightness(){
    // raw_brightness_value is the shared sensor input; acquisition is deferred.
    if (bright == "auto"){
        if (raw_brightness_value >= 2854 && raw_brightness_value <= 4095){
           u8g2.setContrast(HIGH_BRIGHTNESS_LEVEL); 
        }
        else if (raw_brightness_value >= 1613 && raw_brightness_value < 2854){
           u8g2.setContrast(MEDIUM_BRIGHTNESS_LEVEL); 
        }
        else if (raw_brightness_value < 1613){
           u8g2.setContrast(LOW_BRIGHTNESS_LEVEL); 
        }
        else{
            ///Maybe add serial print debug statement, if you get to this point somethings gone wrong lol
        }
    }
    else{
        if (bright == "low"){ //LOW
           u8g2.setContrast(LOW_BRIGHTNESS_LEVEL); 
        }
        else if (bright == "med"){ //MEDIUM
           u8g2.setContrast(MEDIUM_BRIGHTNESS_LEVEL); 
        }
        else if (bright == "high"){ //HIGH
           u8g2.setContrast(HIGH_BRIGHTNESS_LEVEL); 
        }
        else {
            ///Again you could add a potential debug statement if things go wrong   
        }
    }
    return;
}


void render_screen(int x, int y, bool in_menu){
    if (!in_menu){
        draw_home_screen();
    }
    else{
        if (x_level){///Sential value could be changed if desired
            ///Could be made less verbose at this point im not sure if the "x's" are capped in between these numbers
            ///or not so this is a simple but kind ugly way to make sure only values 0-4 are displayed
            draw_options_screen(x);
        }
        else {
            switch(x){//For this to work as i envision the rotary coder must not be able to set the y value to something less than 0 
                case 0:
                    draw_display_settings_screen(y);
                break;
                case 1:
                    draw_clock_settings_screen(y);
                break;
                case 2:
                    draw_alarm1_screen(y);
                break;
                case 3:
                    draw_alarm2_screen(y);
                break;
                case 4:
                    draw_alarm3_screen(y);
                break;
            }
        }
    }
}



void draw_home_screen(){
    char date_str[24];
    char time_str[24];
    
    const char* am_pm_str = (global_clock_hours >= 12) ? "PM" : "AM";
    uint8_t hours_12 = global_clock_hours % 12;
    if (hours_12 == 0) {
        hours_12 = 12;
    }

    snprintf(date_str, sizeof(date_str), "%02d/%02d/%04d",
             global_clock_month, global_clock_day, global_clock_year);
    snprintf(time_str, sizeof(time_str), "%02d:%02d:%02d",
             hours_12, global_clock_minutes, global_clock_seconds);
    const char* day_str = alarm_day_text(global_clock_weekday);

    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(0, 10, date_str);
    uint16_t day_width = u8g2.getStrWidth(day_str);
    u8g2.drawStr(128 - day_width, 10, day_str);

    u8g2.setFont(u8g2_font_logisoso16_tf);
    
    uint16_t time_width = u8g2.getStrWidth(time_str);
    
    u8g2.setFont(u8g2_font_6x12_tr);
    uint16_t am_pm_width = u8g2.getStrWidth(am_pm_str);
    
    uint8_t gap = 3;
    uint16_t total_block_width = time_width + gap + am_pm_width;
    uint16_t start_x = (128 - total_block_width) / 2;

    u8g2.setFont(u8g2_font_logisoso16_tf);
    u8g2.drawStr(start_x, 42, time_str);

    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(start_x + time_width + gap, 42, am_pm_str);

    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(0, 63, "< Snooze");
    const char* menu_label = "Menu >";
    uint16_t menu_width = u8g2.getStrWidth(menu_label);
    u8g2.drawStr(128 - menu_width, 63, menu_label);    
    u8g2.sendBuffer();
}

void draw_options_screen(int x){
    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(2, 12, "Main Menu");
    u8g2.setFont(u8g2_font_6x10_tr);
    switch (x){
        case 0:
            u8g2.drawStr(2, 28, ">");
            u8g2.drawStr(16, 28, "Display Settings");
            u8g2.drawStr(16, 44, "Clock Settings");
            u8g2.drawStr(16, 60, "Alarm 1");
            break;

        case 1:
            u8g2.drawStr(16, 28, "Display Settings");
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, "Clock Settings");
            u8g2.drawStr(16, 60, "Alarm 1");
            break;

        case 2:
            u8g2.drawStr(16, 28, "Clock Settings");
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, "Alarm 1");
            u8g2.drawStr(16, 60, "Alarm 2");
            break;

        case 3:
            u8g2.drawStr(16, 28, "Alarm 1");
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, "Alarm 2");
            u8g2.drawStr(16, 60, "Alarm 3");
            break;

        case 4:
            u8g2.drawStr(16, 28, "Alarm 1");
            u8g2.drawStr(16, 44, "Alarm 2");
            u8g2.drawStr(2, 60, ">");
            u8g2.drawStr(16, 60, "Alarm 3");
            break;
    }
}

void draw_display_settings_screen(int y){
    const String brightness = bright;
    const String daylight_savings = dls_flag ? "On" : "Off";
    const String military_time = mil_time_flag ? "On" : "Off";

    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(2, 12, "Display Settings");
    u8g2.setFont(u8g2_font_6x10_tr);
    switch(y){
        case 0:
            u8g2.drawStr(2, 28, ">");
            u8g2.drawStr(16, 28, ("Brightness: " + brightness).c_str());
            u8g2.drawStr(16, 44, ("Daylight Savings: " + daylight_savings).c_str());
            u8g2.drawStr(16, 60, ("Military Time: " + military_time).c_str());
            break;
        case 1:
            u8g2.drawStr(16, 28, ("Brightness: " + brightness).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Daylight Savings: " + daylight_savings).c_str());
            u8g2.drawStr(16, 60, ("Military Time: " + military_time).c_str());
            break;
        case 2:
            u8g2.drawStr(16, 28, ("Daylight Savings: " + daylight_savings).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Military Time: " + military_time).c_str());
            u8g2.drawStr(16, 60, "Back");
            break;
        case 3:
            u8g2.drawStr(16, 28, ("Daylight Savings: " + daylight_savings).c_str());
            u8g2.drawStr(16, 44, ("Military Time: " + military_time).c_str());
            u8g2.drawStr(2, 60, ">");
            u8g2.drawStr(16, 60, "Back");
            break;
    }
}

void draw_clock_settings_screen(int y){
    const String hours(global_clock_hours), minutes(global_clock_minutes), seconds(global_clock_seconds);
    const String day(global_clock_day), month(global_clock_month), year(global_clock_year);

    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(2, 12, "Clock Settings");
    u8g2.setFont(u8g2_font_6x10_tr);
    switch (y){
        case 0:
            u8g2.drawStr(2, 28, ">");
            u8g2.drawStr(16, 28, ("Hours: " + hours).c_str());
            u8g2.drawStr(16, 44, ("Minutes: " + minutes).c_str());
            u8g2.drawStr(16, 60, ("Seconds: " + seconds).c_str());
            break;
        case 1:
            u8g2.drawStr(16, 28, ("Hours: " + hours).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Minutes: " + minutes).c_str());
            u8g2.drawStr(16, 60, ("Seconds: " + seconds).c_str());
            break;
        case 2:
            u8g2.drawStr(16, 28, ("Hours: " + hours).c_str());
            u8g2.drawStr(16, 44, ("Minutes: " + minutes).c_str());
            u8g2.drawStr(2, 60, ">");
            u8g2.drawStr(16, 60, ("Seconds: " + seconds).c_str());
            break;
        case 3:
            u8g2.drawStr(16, 28, ("Minutes: " + minutes).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Seconds: " + seconds).c_str());
            u8g2.drawStr(16, 60, ("Day: " + day).c_str());
            break;
        case 4:
            u8g2.drawStr(16, 28, ("Seconds: " + seconds).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Day: " + day).c_str());
            u8g2.drawStr(16, 60, ("Month: " + month).c_str());
            break;
        case 5:
            u8g2.drawStr(16, 28, ("Day: " + day).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Month: " + month).c_str());
            u8g2.drawStr(16, 60, ("Year: " + year).c_str());
            break;
        case 6:
            u8g2.drawStr(16, 28, ("Month: " + month).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Year: " + year).c_str());
            u8g2.drawStr(16, 60, "Back");
            break;
        case 7:
            u8g2.drawStr(16, 28, ("Month: " + month).c_str());
            u8g2.drawStr(16, 44, ("Year: " + year).c_str());
            u8g2.drawStr(2, 60, ">");
            u8g2.drawStr(16, 60, "Back");
            break;
    }
}

void draw_alarm1_screen(int y){
    const AlarmSettings& settings = alarms[0];
    const String alarm = settings.toggle ? "On" : "Off";
    const String hours(settings.hours), minutes(settings.minutes), seconds(settings.second);
    const String daily = settings.day == 7 ? "On" : "Off";
    const String day(settings.month_day), month(settings.month), sound(settings.selected_sound);
    const String snooze_length(settings.snooze_length);
    // The existing Snooze row has no defined setting; preserve it as a placeholder.
    const String snooze = "--";
    const String delay_var(settings.snooze_delay), snooze_amount(settings.snooze_amount);

    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(2, 12, "Alarm 1 Settings");
    u8g2.setFont(u8g2_font_6x10_tr);
    switch (y) {
        case 0:
            u8g2.drawStr(2, 28, ">");
            u8g2.drawStr(16, 28, ("Alarm: " + alarm).c_str());
            u8g2.drawStr(16, 44, ("Hours: " + hours).c_str());
            u8g2.drawStr(16, 60, ("Minutes: " + minutes).c_str());
            break;
        case 1:
            u8g2.drawStr(16, 28, ("Alarm: " + alarm).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Hours: " + hours).c_str());
            u8g2.drawStr(16, 60, ("Minutes: " + minutes).c_str());
            break;
        case 2:
            u8g2.drawStr(16, 28, ("Hours: " + hours).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Minutes: " + minutes).c_str());
            u8g2.drawStr(16, 60, ("Seconds: " + seconds).c_str());
            break;
        case 3:
            u8g2.drawStr(16, 28, ("Minutes: " + minutes).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Seconds: " + seconds).c_str());
            u8g2.drawStr(16, 60, ("Daily: " + daily).c_str());
            break;
        case 4:
            u8g2.drawStr(16, 28, ("Seconds: " + seconds).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Daily: " + daily).c_str());
            u8g2.drawStr(16, 60, ("Day: " + day).c_str());
            break;
        case 5:
            u8g2.drawStr(16, 28, ("Daily: " + daily).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Day: " + day).c_str());
            u8g2.drawStr(16, 60, ("Month: " + month).c_str());
            break;
        case 6:
            u8g2.drawStr(16, 28, ("Day: " + day).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Month: " + month).c_str());
            u8g2.drawStr(16, 60, ("Sound: " + sound).c_str());
            break;
        case 7:
            u8g2.drawStr(16, 28, ("Month: " + month).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Sound: " + sound).c_str());
            u8g2.drawStr(16, 60, ("Snooze Length: " + snooze_length).c_str());
            break;
        case 8:
            u8g2.drawStr(16, 28, ("Sound: " + sound).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Snooze Length: " + snooze_length).c_str());
            u8g2.drawStr(16, 60, ("Snooze: " + snooze).c_str());
            break;
        case 9:
            u8g2.drawStr(16, 28, ("Snooze Length: " + snooze_length).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Snooze: " + snooze).c_str());
            u8g2.drawStr(16, 60, ("Delay: " + delay_var).c_str());
            break;
        case 10:
            u8g2.drawStr(16, 28, ("Snooze: " + snooze).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Delay: " + delay_var).c_str());
            u8g2.drawStr(16, 60, ("Snooze Amount: " + snooze_amount).c_str());
            break;
        case 11:
            u8g2.drawStr(16, 28, ("Delay: " + delay_var).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Snooze Amount: " + snooze_amount).c_str());
            u8g2.drawStr(16, 60, "Back");
            break;
        case 12:
            u8g2.drawStr(16, 28, ("Delay: " + delay_var).c_str());
            u8g2.drawStr(16, 44, ("Snooze Amount: " + snooze_amount).c_str());
            u8g2.drawStr(2, 60, ">");
            u8g2.drawStr(16, 60, "Back");
            break;
    }
}
void draw_alarm2_screen(int y){
    const AlarmSettings& settings = alarms[1];
    const String alarm = settings.toggle ? "On" : "Off";
    const String hours(settings.hours), minutes(settings.minutes), seconds(settings.second);
    const String daily = settings.day == 7 ? "On" : "Off";
    const String day(settings.month_day), month(settings.month), sound(settings.selected_sound);
    const String snooze_length(settings.snooze_length);
    // The existing Snooze row has no defined setting; preserve it as a placeholder.
    const String snooze = "--";
    const String delay_var(settings.snooze_delay), snooze_amount(settings.snooze_amount);

    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(2, 12, "Alarm 2 Settings");
    u8g2.setFont(u8g2_font_6x10_tr);
    switch (y) {
        case 0:
            u8g2.drawStr(2, 28, ">");
            u8g2.drawStr(16, 28, ("Alarm: " + alarm).c_str());
            u8g2.drawStr(16, 44, ("Hours: " + hours).c_str());
            u8g2.drawStr(16, 60, ("Minutes: " + minutes).c_str());
            break;
        case 1:
            u8g2.drawStr(16, 28, ("Alarm: " + alarm).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Hours: " + hours).c_str());
            u8g2.drawStr(16, 60, ("Minutes: " + minutes).c_str());
            break;
        case 2:
            u8g2.drawStr(16, 28, ("Hours: " + hours).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Minutes: " + minutes).c_str());
            u8g2.drawStr(16, 60, ("Seconds: " + seconds).c_str());
            break;
        case 3:
            u8g2.drawStr(16, 28, ("Minutes: " + minutes).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Seconds: " + seconds).c_str());
            u8g2.drawStr(16, 60, ("Daily: " + daily).c_str());
            break;
        case 4:
            u8g2.drawStr(16, 28, ("Seconds: " + seconds).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Daily: " + daily).c_str());
            u8g2.drawStr(16, 60, ("Day: " + day).c_str());
            break;
        case 5:
            u8g2.drawStr(16, 28, ("Daily: " + daily).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Day: " + day).c_str());
            u8g2.drawStr(16, 60, ("Month: " + month).c_str());
            break;
        case 6:
            u8g2.drawStr(16, 28, ("Day: " + day).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Month: " + month).c_str());
            u8g2.drawStr(16, 60, ("Sound: " + sound).c_str());
            break;
        case 7:
            u8g2.drawStr(16, 28, ("Month: " + month).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Sound: " + sound).c_str());
            u8g2.drawStr(16, 60, ("Snooze Length: " + snooze_length).c_str());
            break;
        case 8:
            u8g2.drawStr(16, 28, ("Sound: " + sound).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Snooze Length: " + snooze_length).c_str());
            u8g2.drawStr(16, 60, ("Snooze: " + snooze).c_str());
            break;
        case 9:
            u8g2.drawStr(16, 28, ("Snooze Length: " + snooze_length).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Snooze: " + snooze).c_str());
            u8g2.drawStr(16, 60, ("Delay: " + delay_var).c_str());
            break;
        case 10:
            u8g2.drawStr(16, 28, ("Snooze: " + snooze).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Delay: " + delay_var).c_str());
            u8g2.drawStr(16, 60, ("Snooze Amount: " + snooze_amount).c_str());
            break;
        case 11:
            u8g2.drawStr(16, 28, ("Delay: " + delay_var).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Snooze Amount: " + snooze_amount).c_str());
            u8g2.drawStr(16, 60, "Back");
            break;
        case 12:
            u8g2.drawStr(16, 28, ("Delay: " + delay_var).c_str());
            u8g2.drawStr(16, 44, ("Snooze Amount: " + snooze_amount).c_str());
            u8g2.drawStr(2, 60, ">");
            u8g2.drawStr(16, 60, "Back");
            break;
    }
}

void draw_alarm3_screen(int y){
    const AlarmSettings& settings = alarms[2];
    const String alarm = settings.toggle ? "On" : "Off";
    const String hours(settings.hours), minutes(settings.minutes), seconds(settings.second);
    const String daily = settings.day == 7 ? "On" : "Off";
    const String day(settings.month_day), month(settings.month), sound(settings.selected_sound);
    const String snooze_length(settings.snooze_length);
    // The existing Snooze row has no defined setting; preserve it as a placeholder.
    const String snooze = "--";
    const String delay_var(settings.snooze_delay), snooze_amount(settings.snooze_amount);

    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(2, 12, "Alarm 3 Settings");
    u8g2.setFont(u8g2_font_6x10_tr);
    switch (y) {
        case 0:
            u8g2.drawStr(2, 28, ">");
            u8g2.drawStr(16, 28, ("Alarm: " + alarm).c_str());
            u8g2.drawStr(16, 44, ("Hours: " + hours).c_str());
            u8g2.drawStr(16, 60, ("Minutes: " + minutes).c_str());
            break;
        case 1:
            u8g2.drawStr(16, 28, ("Alarm: " + alarm).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Hours: " + hours).c_str());
            u8g2.drawStr(16, 60, ("Minutes: " + minutes).c_str());
            break;
        case 2:
            u8g2.drawStr(16, 28, ("Hours: " + hours).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Minutes: " + minutes).c_str());
            u8g2.drawStr(16, 60, ("Seconds: " + seconds).c_str());
            break;
        case 3:
            u8g2.drawStr(16, 28, ("Minutes: " + minutes).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Seconds: " + seconds).c_str());
            u8g2.drawStr(16, 60, ("Daily: " + daily).c_str());
            break;
        case 4:
            u8g2.drawStr(16, 28, ("Seconds: " + seconds).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Daily: " + daily).c_str());
            u8g2.drawStr(16, 60, ("Day: " + day).c_str());
            break;
        case 5:
            u8g2.drawStr(16, 28, ("Daily: " + daily).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Day: " + day).c_str());
            u8g2.drawStr(16, 60, ("Month: " + month).c_str());
            break;
        case 6:
            u8g2.drawStr(16, 28, ("Day: " + day).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Month: " + month).c_str());
            u8g2.drawStr(16, 60, ("Sound: " + sound).c_str());
            break;
        case 7:
            u8g2.drawStr(16, 28, ("Month: " + month).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Sound: " + sound).c_str());
            u8g2.drawStr(16, 60, ("Snooze Length: " + snooze_length).c_str());
            break;
        case 8:
            u8g2.drawStr(16, 28, ("Sound: " + sound).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Snooze Length: " + snooze_length).c_str());
            u8g2.drawStr(16, 60, ("Snooze: " + snooze).c_str());
            break;
        case 9:
            u8g2.drawStr(16, 28, ("Snooze Length: " + snooze_length).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Snooze: " + snooze).c_str());
            u8g2.drawStr(16, 60, ("Delay: " + delay_var).c_str());
            break;
        case 10:
            u8g2.drawStr(16, 28, ("Snooze: " + snooze).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Delay: " + delay_var).c_str());
            u8g2.drawStr(16, 60, ("Snooze Amount: " + snooze_amount).c_str());
            break;
        case 11:
            u8g2.drawStr(16, 28, ("Delay: " + delay_var).c_str());
            u8g2.drawStr(2, 44, ">");
            u8g2.drawStr(16, 44, ("Snooze Amount: " + snooze_amount).c_str());
            u8g2.drawStr(16, 60, "Back");
            break;
        case 12:
            u8g2.drawStr(16, 28, ("Delay: " + delay_var).c_str());
            u8g2.drawStr(16, 44, ("Snooze Amount: " + snooze_amount).c_str());
            u8g2.drawStr(2, 60, ">");
            u8g2.drawStr(16, 60, "Back");
            break;
    }
}
