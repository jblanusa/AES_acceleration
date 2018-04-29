#ifndef __UTILS_H_
#define __UTILS_H_

#define UTILS_TEXT_SPEED_FAST  50
#define UTILS_TEXT_SPEED_NORM  80
#define UTILS_TEXT_SPEED_SLOW 120
#define UTILS_DEFAULT_FONT_COLOR 0xFFFFFFFF
#define UTILS_DEFAULT_BACK_COLOR 0xFF000000

void utils_display_text(char* text, int text_speed, int font_color, int back_color);
void utils_display_text_and_stop(char* text, int text_speed, int font_color, int back_color);
int utils_display_menu(char** menu_items, int items_nb);

#endif // __UTILS_H_
