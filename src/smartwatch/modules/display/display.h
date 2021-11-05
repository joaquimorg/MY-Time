#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdbool.h>
#include <stdint.h>
#include "nrf52.h"

#define RGB2COLOR(r, g, b) ((((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3)))

class Display
{
  protected:
    uint16_t verticalScrollingStartAddress = 0;

  public:
    Display(void);
    void init(void);

    void draw_bitmap (uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t* bitmap);
    void draw_square(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);

    void vertical_scroll_definition(uint16_t topFixedLines, uint16_t scrollLines, uint16_t bottomFixedLines, uint16_t line);
    void vertical_scroll_start_address(uint16_t line);

    void sleep(void);
    void wake_up(void);

};

#endif /* DISPLAY_H */