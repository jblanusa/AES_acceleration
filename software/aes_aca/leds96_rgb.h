#ifndef __LEDS96_H_
#define __LEDS96_H_

// the screen dimensions
#define LEDS96_SCREEN_HEIGHT 8
#define LEDS96_SCREEN_WIDTH  12

// a few basic colors
#define LEDS96_WHITE   0xFFFFFFFF
#define LEDS96_BLACK   0xFF000000
#define LEDS96_RED     0xFFFF0000
#define LEDS96_GREEN   0xFF00FF00
#define LEDS96_BLUE    0xFF0000FF
#define LEDS96_YELLOW  0xFFFFFF00
#define LEDS96_CYAN    0xFF00FFFF
#define LEDS96_MAGENTA 0xFFFF00FF

// functions affecting directly the leds
void leds96_reset();
void leds96_draw_pixel(int x, int y, int c);
void leds96_draw_image(int data[LEDS96_SCREEN_WIDTH][LEDS96_SCREEN_HEIGHT]);
void leds96_display_canvas();

//basic text functions (bypasses the canvas)
void leds96_shift_text_left(char* text, int position, int font_color, int back_color);
void leds96_shift_text_right(char* text, int position, int font_color, int back_color);
void leds96_shift_column_left(char data, int c0, int c1);

// function interacting with the canvas
void leds96_canvas_reset(int c);
void leds96_canvas_draw_bitmap(int data[3], int c0, int c1);
void leds96_canvas_draw_bitmap_alpha(int data[3], int c0, int c1);
void leds96_canvas_draw_image(int data[LEDS96_SCREEN_WIDTH][LEDS96_SCREEN_HEIGHT]);
void leds96_canvas_draw_image_alpha(int data[LEDS96_SCREEN_WIDTH][LEDS96_SCREEN_HEIGHT]);
void leds96_canvas_draw_pixel(int x, int y, int c);
void leds96_canvas_fill_rect(int x, int y, int w, int h, int c);
void leds96_canvas_fill_rect_alpha(int x, int y, int w, int h, int c);

#endif // __LEDS96_H_
