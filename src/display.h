#ifndef DISPLAY_H
#define DISPLAY_H

//8 bit value sent to OLED to tell it how bright its going to be 
#define LOW_BRIGHTNESS_LEVEL        10
#define MEDIUM_BRIGHTNESS_LEVEL     100
#define HIGH_BRIGHTNESS_LEVEL       255

// Navigation indices retain their signed int type across module boundaries.
// in_menu=true displays menus; false displays home.
void update_display(int x, int y, bool in_menu);
void render_screen(int x, int y, bool in_menu);
void check_brightness();
void draw_home_screen();
void draw_options_screen(int x);
void draw_display_settings_screen(int y);
void draw_clock_settings_screen(int y);
void draw_alarm1_screen(int y);
void draw_alarm2_screen(int y);
void draw_alarm3_screen(int y);

#endif
