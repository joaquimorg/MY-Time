#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdbool.h>
#include <stdint.h>
#include "nrf52.h"


class Display
{
  protected:
    uint16_t verticalScrollingStartAddress = 0;
    
    void write_command(uint8_t cmd);
    void write_data(uint8_t data);
    void write_command_data(uint8_t command, const uint8_t *params, uint16_t len);

  public:
    Display(void);
    void init(void);
    
    void start_write_display(void);
    void end_write_display(void);
    void write_spi(const uint8_t* data, size_t size);

    void sleep(void);
    void wake_up(void);

    void software_reset();
    void sleep_out();
    void sleep_in();
    void col_mod();
    void memory_data_access_control();
    void column_address_set();
    void row_address_set();
    void display_inversion_on();
    void normal_mode_on();
    void display_on();
    void set_addr_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    void write_to_ram();
    void display_off();
    void vertical_scroll_definition(uint16_t topFixedLines, uint16_t scrollLines, uint16_t bottomFixedLines);
    void vertical_scroll_start_address(uint16_t line);
    void partial_display(uint8_t mode);
    void set_part_area(uint16_t sr, uint16_t er);
    void draw_pixel(uint16_t x, uint16_t y, uint32_t color);
    void draw_buffer(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t *data, size_t size);
    void hardware_reset();

};

#endif /* DISPLAY_H */