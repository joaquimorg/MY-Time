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

    void set_addr_display(uint32_t x, uint32_t y, uint32_t w, uint32_t h);

    void vertical_scroll_definition(uint16_t topFixedLines, uint16_t scrollLines, uint16_t bottomFixedLines, uint16_t line);

    void spi_command(uint8_t d);
    void start_write_display(void);
    void end_write_display(void);

    void sleep(void);
    void wake_up(void);

};

#endif /* DISPLAY_H */