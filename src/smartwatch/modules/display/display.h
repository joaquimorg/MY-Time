#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdbool.h>
#include <stdint.h>

#define RGB2COLOR(r, g, b) ((((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3)))
/*
void draw_bitmap (uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t* bitmap);
void draw_square(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);

void display_pause();
void display_resume();

void st7789_vertical_scroll_definition(uint16_t topFixedLines, uint16_t scrollLines, uint16_t bottomFixedLines, uint16_t line);
void st7789_vertical_scroll_start_address(uint16_t line);

void st7789_sleep_out(void);
void st7789_sleep_in(void);

void st7789_init(void);

void st7789_sleep(void);
void st7789_wake_up(void);
*/

#endif /* ST7789_H */