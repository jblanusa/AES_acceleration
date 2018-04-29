/*
 * game.c
 *
 *  Created on: Mar 24, 2012
 *      Author: xavier.jimenez@epfl.ch
 */
#include <system.h>

#include "leds96_rgb.h"
#include "timer.h"
#include "utils.h"

/**
 * Similar to utils_display_text, but stops when the last
 * character has been shifted out.
 */
void utils_display_text_and_stop(char* text, int text_speed, int font_color, int back_color) {
	int position = 0;
	while (text[position>>2] != '\0') {
		leds96_shift_text_left(text, position, font_color, back_color);
		position++;
		timer_wait(text_speed);
	}
	// shift out the 3 characters on the screen
	int i;
	for(i=0; i<12; i++) {
		leds96_shift_column_left(0, back_color, back_color);
		timer_wait(text_speed);
	}
}

/**
 * Display some text on the led matrix.
 * The text is shifted from the right to the left.
 * Stops once the last character has been shifted in.
 * Predefined speed are available with
 * GAME_TEXT_SPEED_FAST, GAME_TEXT_SPEED_NORM and
 * GAME_TEXT_SPEED_SLOW
 */
void utils_display_text(char* text, int text_speed, int font_color, int back_color) {
	utils_display_text_and_stop(text, text_speed, font_color, back_color);
	// shift out the 3 characters on the screen
	int i;
	for(i=0; i<12; i++) {
		leds96_shift_column_left(0, back_color, back_color);
		timer_wait(text_speed);
	}
}
