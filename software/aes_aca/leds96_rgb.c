/*
 * leds96.c
 *
 *  Created on: Sep 18, 2012
 *      Author: xavier.jimenez@epfl.ch
 */
#include <system.h>
#include <io.h>

#include "leds96_rgb.h"

// the frame canvas
int canvas[12][8];

typedef struct{
	int a;
	int r;
	int g;
	int b;
} struct_argb;

struct_argb int_argb_to_struct(int c) {
	struct_argb ret;
	ret.a = (c>>24) & 0xFF;
	ret.r = (c>>16) & 0xFF;
	ret.g = (c>>8) & 0xFF;
	ret.b = c & 0xFF;
	return ret;
}

int struct_argb_to_int(struct_argb argb) {
	return ((argb.a & 0xFF) << 24) |
			((argb.r & 0xFF) << 16) |
			((argb.g & 0xFF) << 8)  |
			(argb.b & 0xFF);
}

/**
 * Applies a front color on top of a back color according to
 * the front alpha channel.
 */
struct_argb apply_alpha(struct_argb front, struct_argb back) {
	struct_argb ret;
	ret.a = 0xFF; // becomes opaque anyway
	ret.r = (back.r * (255-front.a) + front.r * front.a)/255;
	ret.g = (back.g * (255-front.a) + front.g * front.a)/255;
	ret.b = (back.b * (255-front.a) + front.b * front.a)/255;
	return ret;
}

/**
 * Reset the LEDs
 */
void leds96_reset() {
	int i;
	for (i=0; i<96; i++) {
		IOWR(FULLRGB_96LED_0_BASE, i, 0);
	}
}

/**
 * Draws a single pixel directly to the LEDs at coordinate x,y with color c (alpha ignored).
 */
void leds96_draw_pixel(int x, int y, int c){
	if (x<12 && y<8) {
		IOWR(FULLRGB_96LED_0_BASE, x+y*12, c);
	}
}
/**
 * Draws a 12x8 array directly to the LEDs.
 * Each entry represents the rgb color.
 */
void leds96_draw_image(int data[LEDS96_SCREEN_WIDTH][LEDS96_SCREEN_HEIGHT]) {
	int i,j;
	for(i=0; i<LEDS96_SCREEN_WIDTH; i++){
		for(j=0; j<LEDS96_SCREEN_HEIGHT;j++){
			IOWR(FULLRGB_96LED_0_BASE, i+12*j, data[i][j]);
		}
	}
}

/**
 * Copy the canvas content to the LEDs
 */
void leds96_display_canvas() {
	int i,j;
	for (i=0; i<LEDS96_SCREEN_WIDTH; i++) {
		for (j=0; j<LEDS96_SCREEN_HEIGHT; j++) {
			IOWR(FULLRGB_96LED_0_BASE, i+j*12, canvas[i][j]);
		}
	}
}

/**
 * Reset the canvas to a desired color c (alpha ignored)
 */
void leds96_canvas_reset(int c) {
	int i,j;
	for (i=0; i<LEDS96_SCREEN_WIDTH; i++) {
		for (j=0; j<LEDS96_SCREEN_HEIGHT; j++) {
			canvas[i][j] = c;
		}
	}
}

/**
 * Draws a 12x8 array on the canvas.
 * Each entry represents the rgb color.
 */
void leds96_canvas_draw_image(int data[LEDS96_SCREEN_WIDTH][LEDS96_SCREEN_HEIGHT]) {
	int i,j;
	for(i=0; i<LEDS96_SCREEN_WIDTH; i++){
		for(j=0; j<LEDS96_SCREEN_HEIGHT;j++){
			canvas[i][j] = data[i][j];
		}
	}
}

/**
 * Similar to leds96_canvas_draw_image, but takes the alpha channel into account.
 */
void leds96_canvas_draw_image_alpha(int data[12][8]) {
	int i,j;
	for(i=0; i<12; i++){
		for(j=0; j<8;j++){
			// read current color
			struct_argb t = int_argb_to_struct(canvas[i][j]);
			// color to draw
			struct_argb c = int_argb_to_struct(data[i][j]);
			// new color
			t = apply_alpha(c, t);
			// draw new color
			canvas[i][j] = struct_argb_to_int(t);
		}
	}
}

/**
 * Draw a bitmap onto the canvas. The input must be 96 bits (an array
 * of 3 integers), c0 is the color for bits at 0 and c1 for bits at 1.
 * For a simpler representation, bit 0 is on the top left and we
 * continue counting vertically with the pixel below. Therefore each byte
 * represent a column.
 */
void leds96_canvas_draw_bitmap(int data[3], int c0, int c1) {
	int i, j;
	int c=0;
	int x=0;
	int y=0;
	// for each word
	for (i=0; i<3; i++) {
		int d = data[i];
		// for each bit
		for (j=0; j<32; j++) {
			// which color to apply?
			if (d & 1) {
				c = c1;
			} else {
				c = c0;
			}
			// write the new color
			// IOWR(FULLRGB_96LED_0_BASE, x+y, c);
			canvas[x][y] = c;
			// update indexes
			d >>= 1;
			if ((j&7) == 7) {
				y=0;
				x++;
			} else {
				y++;
			}
		}
	}
}

/**
 * Similar to leds96_canvas_draw_bitmap_alpha, but takes the alpha channel into account.
 */
void leds96_canvas_draw_bitmap_alpha(int data[3], int c0, int c1) {
	int i, j;
	// decompose each channel
	struct_argb c0_argb = int_argb_to_struct(c0);
	struct_argb c1_argb = int_argb_to_struct(c1);
	int x=0;
	int y=0;
	// for each word
	for (i=0; i<3; i++) {
		int d = data[i];
		// for each bit
		for (j=0; j<32; j++) {
			// read current color
			struct_argb l = int_argb_to_struct(IORD(FULLRGB_96LED_0_BASE, x+y));
			// which color to apply?
			if (d & 1) {
				// color 1
				// we average each channel according to the new color alpha
				l = apply_alpha(c1_argb, l);
			} else {
				// color 0
				l = apply_alpha(c0_argb, l);
			}
			// write the new color
			// IOWR(FULLRGB_96LED_0_BASE, x+y, struct_argb_to_int(l));
			canvas[x][y] = struct_argb_to_int(l);
			// update indexes
			d >>= 1;
			if ((j&7) == 7) {
				y=0;
				x++;
			} else {
				y++;
			}
		}
	}
}

/**
 * Draws a single pixel on the canvas at coordinate x,y with color c (alpha ignored).
 */
void leds96_canvas_draw_pixel(int x, int y, int c){
	if (x<12 && y<8) {
		canvas[x][y] = c;
		// IOWR(FULLRGB_96LED_0_BASE, x+y*12, c);
	}
}

/**
 * Draws a filled rectangle on the canvas.
 * Arguments x and y set the top left angle coordinate,
 * while w and h set the dimensions.
 * The color is set by c (alpha ignored).
 */
void leds96_canvas_fill_rect(int x, int y, int w, int h, int c) {
	int i,j=0;
	for (i=x; i<x+w && i<12; i++) {
		for (j=y; j<y+h && j<8; j++) {
			// IOWR(FULLRGB_96LED_0_BASE, i+j*12, c);
			canvas[i][j] = c;
		}
	}
}

/**
 * Similar to leds96_canvas_fill_rect, but takes the alpha channel into account.
 */
void leds96_canvas_fill_rect_alpha(int x, int y, int w, int h, int c) {
	int i,j=0;
	struct_argb c_argb = int_argb_to_struct(c);
	for (i=x; i<x+w && i<12; i++) {
		for (j=y; j<y+h && j<8; j++) {
			struct_argb t = apply_alpha(c_argb, int_argb_to_struct(canvas[i][j]));
			// IOWR(FULLRGB_96LED_0_BASE, index, struct_argb_to_int(t));
			canvas[i][j] = struct_argb_to_int(t);
		}
	}
}

// bitmaps for characters on a 3x5 grid.
int ASCII_3x5[] = {
	0x00000000, // space (32)
	0x0000B800, // !
	0x00180018, // "
	0x00F850F8, // #
	0x0050F8A0, // $
	0x009820C8, // %
	0x00D0A8D8, // &
	0x00001800, // '
	0x00008870, // (
	0x00708800, // )
	0x00A870A8, // *
	0x00207020, // +
	0x0000C080, // ,
	0x00202020, // -
	0x00008000, // .
	0x00186080, // /
	0x007888F0, // 0 (48)
	0x0080F890, // 1
	0x0090A8C8, // 2
	0x00F8A888, // 3
	0x00F82038, // 4
	0x00E8A8B8, // 5
	0x00E8A8F8, // 6
	0x00F80808, // 7
	0x00F8A8F8, // 8
	0x00F8A8B8, // 9
	0x00005000, // :
	0x00D08000, // ;
	0x00885020, // <
	0x00505050, // =
	0x00205088, // >
	0x0010A808, // ?
	0x00B08870, // @
	0x00F828F8, // A (65)
	0x0050A8F8, // B
	0x008888F8, // C
	0x007088F8, // D
	0x0088A8F8, // E
	0x000828F8, // F
	0x00C88870, // G
	0x00F820F8, // H
	0x0088F888, // I
	0x00788848, // J
	0x00D820F8, // K
	0x008080F8, // L
	0x00F830F8, // M
	0x00F810F8, // N
	0x00708870, // O
	0x001028F8, // P
	0x00B0C870, // Q
	0x00D028F8, // R
	0x0048A890, // S
	0x0008F808, // T
	0x00F880F8, // U
	0x0078C078, // V
	0x00F860F8, // W
	0x00D820D8, // X
	0x0038E038, // Y
	0x0098A8C8, // Z
	0x000088F8, // [
	0x00806018, /* \ */
	0x00F88800, // ]
	0x00100810, // ^
	0x00808080, // _
	0x00001008, // `
	// lowercase not implemented... (97-122)
	0x0088F820, // { (123)
	0x0000D800, // |
	0x0020F888, // }
	0x00602030, // ~
};

/**
 * Translates a character into a 32-bit bitmap.
 */
int get_3x5_bmp(char c) {
	int index = c & 0xFF;
	int ret = 0xF8F8F8;
	if (index<32) ret = 0xF8F8F8;
	else if (index<97) ret = ASCII_3x5[index-32];
	else if (index<123) ret = ASCII_3x5[index-97+65-32];
	else if (index<127) ret = ASCII_3x5[index-123+97-32];
	// we shift it by 2 bits to center the text
	return ret >> 2;
}

/**
 * Shifts the screen to the left and append bitmap column to the right.
 * Only the 8 bits of data are read. Bits at 1 are colored with c1 and
 * bits at 0 with c0. The alpha channel is ignored.
 */
void leds96_shift_column_left(char data, int c0, int c1){
	int i, j;
	// shift the screen to the left
	for (i=0; i<11; i++) {
		for (j=0; j<96; j+=12) {
			IOWR(FULLRGB_96LED_0_BASE, i+j,
					IORD(FULLRGB_96LED_0_BASE, i+j+1));
		}
	}
	// for the last column we look at the data
	for (i=11; i<96; i+=12) {
		if (data & 1) {
			IOWR(FULLRGB_96LED_0_BASE, i, c1);
		} else {
			IOWR(FULLRGB_96LED_0_BASE, i, c0);
		}
		data = data>>1;
	}
}

/**
 * Shifts some text from the right to the left.
 * It takes the complete string character as argument as well as
 * the column to shift in (there are four 8-bit columns per character).
 * The font color and background color must be specified with font_color and
 * back_color arguments (alpha ignored).
 */
void leds96_shift_text_left(char* text, int column, int font_color, int back_color) {
	// get the new 8 bit to enter
	char data = get_3x5_bmp(text[column>>2]) >> (8 * (column & 3));
	// shift the frame to 1 column to the left and append the data
	leds96_shift_column_left(data, back_color, font_color);
}

/**
 * Same as leds96_shift_text_left, but shifts text from the left to the right.
 */
void leds96_shift_text_right(char* text, int position, int font_color, int back_color) {
	// get the new 8 bit to enter
	char data = get_3x5_bmp(text[position>>2]) >> (8 * (position & 3));
	// shift the frame to 1 column to the right
	int i, j;
	for (i=11; i>0; i--) {
		for (j=0; j<96; j+=12) {
			IOWR(FULLRGB_96LED_0_BASE, i+j,
					IORD(FULLRGB_96LED_0_BASE, i+j-1));
		}
	}
	// for the last column we look at the char
	for (i=0; i<96; i+=12) {
		if (data & 1) {
			IOWR(FULLRGB_96LED_0_BASE, i, font_color);
		} else {
			IOWR(FULLRGB_96LED_0_BASE, i, back_color);
		}
		data = data>>1;
	}
}
