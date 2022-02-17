#ifndef __DRAW_H
#define __DRAW_H

#include "main.h"

void draw_circle(unsigned int x, unsigned int y, unsigned int r);
void draw_circle_af(int x, int y, int r);
void draw_line(int x0, int y0, int x1, int y1);
void draw_char(int16_t x, int16_t y, unsigned char c, uint8_t color);
void draw_char8x8(int16_t x, int16_t y, unsigned char c, unsigned char color);
void draw_string(uint8_t x, uint8_t y, const char *msg, int size);
void draw_number(uint8_t x, uint8_t y, uint8_t num);
void draw_clear_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h);

#endif // __DRAW_H
