#include "display.h"
#include "pinetime_board.h"
#include "nrf52.h"
#include "nrf.h"
#include "fast_spi.h"


// ST7789 specific commands used in init
#define ST7789_SWRESET	    0x01
#define ST7789_SLPIN		0x10
#define ST7789_SLPOUT		0x11
#define ST7789_PTLON		0x12
#define ST7789_NORON		0x13
#define ST7789_INVOFF		0x20
#define ST7789_INVON		0x21
#define ST7789_DISPOFF	    0x28
#define ST7789_DISPON		0x29
#define ST7789_CASET		0x2A
#define ST7789_RASET		0x2B
#define ST7789_RAMWR		0x2C
#define ST7789_PTLAR		0x30
#define ST7789_VSCRDEF	    0x33      // Vertical scrolling definition (ST7789V)
#define ST7789_MADCTL		0x36      // Memory data access control
#define ST7789_VSCSAD		0x37
#define ST7789_IDMOFF		0x38      // Idle mode off
#define ST7789_IDMON		0x39      // Idle mode on
#define ST7789_COLMOD		0x3A
#define ST7789_RAMCTRL	    0xB0      // RAM control
#define ST7789_RGBCTRL	    0xB1      // RGB control
#define ST7789_PORCTRL	    0xB2      // Porch control
#define ST7789_FRCTRL1	    0xB3      // Frame rate control
#define ST7789_INVCTRL	    0xB4
#define ST7789_PARCTRL	    0xB5      // Partial mode control
#define ST7789_GCTRL		0xB7      // Gate control
#define ST7789_GTADJ		0xB8      // Gate on timing adjustment
#define ST7789_DGMEN		0xBA      // Digital gamma enable
#define ST7789_VCOMS		0xBB      // VCOMS setting
#define ST7789_LCMCTRL	    0xC0      // LCM control
#define ST7789_IDSET		0xC1      // ID setting
#define ST7789_VDVVRHEN	    0xC2      // VDV and VRH command enable
#define ST7789_VRHS			0xC3      // VRH set
#define ST7789_VDVSET		0xC4      // VDV setting
#define ST7789_VCMOFSET	    0xC5      // VCOMS offset set


/**
 * Constructor
 */
Display::Display(void) {

}

void Display::start_write_display(void) {
    enable_spi(true);
    digitalWrite(LCD_CSN, LOW);
}

void Display::end_write_display(void) {
    digitalWrite(LCD_CSN, HIGH);
    enable_spi(false);
}

void Display::write_command(uint8_t cmd) {
    digitalWrite(LCD_DC, LOW);
    write_fast_spi(&cmd, 1);
}

void Display::write_data(uint8_t data) {
    digitalWrite(LCD_DC, HIGH);
    write_fast_spi(&data, 1);
}

void Display::write_spi(const uint8_t* data, size_t size) {
    write_fast_spi(data, size);
}

void Display::write_command_data(uint8_t command, const uint8_t* params, uint16_t len) {
    write_command(command);
    if ((params != NULL && len > 0)) {
        write_spi(params, len);
    }
}

void Display::init(void) {


    pinMode(LCD_CSN, OUTPUT);
    pinMode(LCD_DC, OUTPUT);
    pinMode(LCD_RST, OUTPUT);

    digitalWrite(LCD_CSN, HIGH);
    digitalWrite(LCD_DC, HIGH);
    digitalWrite(LCD_RST, LOW);

    hardware_reset();

    start_write_display();
    
    //software_reset();
    
    sleep_out();
    col_mod();
    memory_data_access_control();
    column_address_set();
    row_address_set();
    display_inversion_on();
    normal_mode_on();
    vertical_scroll_definition(0, 320, 0);
    vertical_scroll_start_address(0);
    display_on();

    end_write_display();
}

void Display::software_reset() {
    write_command(ST7789_SWRESET);
    delay_ns(100);
}

void Display::sleep_out() {
    write_command(ST7789_SLPOUT);
    delay_ns(100);
}

void Display::sleep_in() {
    write_command(ST7789_SLPIN);
    delay_ns(100);
}

void Display::col_mod() {
    write_command(ST7789_COLMOD);
    write_data(0x55);
    delay_ns(100);
}

void Display::memory_data_access_control() {
    write_command(ST7789_MADCTL);
    write_data(0x00);
}

void Display::column_address_set() {
    write_command(ST7789_CASET);
    write_data(0x00);
    write_data(0x00);
    write_data(240 >> 8u);
    write_data(240 & 0xffu);
}

void Display::row_address_set() {
    write_command(ST7789_RASET);
    write_data(0x00);
    write_data(0x00);
    write_data(320u >> 8u);
    write_data(320u & 0xffu);
}

void Display::display_inversion_on() {
    write_command(ST7789_INVON);
    delay_ns(100);
}

void Display::normal_mode_on() {
    write_command(ST7789_NORON);
    delay_ns(100);
}

void Display::display_on() {
    write_command(ST7789_DISPON);
}

void Display::set_addr_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    write_command(ST7789_CASET);
    write_data(x0 >> 8);
    write_data(x0 & 0xff);
    write_data(x1 >> 8);
    write_data(x1 & 0xff);

    write_command(ST7789_RASET);
    write_data(y0 >> 8);
    write_data(y0 & 0xff);
    write_data(y1 >> 8);
    write_data(y1 & 0xff);

    write_to_ram();
}

void Display::write_to_ram() {
    write_command(ST7789_RAMWR);
}

void Display::display_off() {
    write_command(ST7789_DISPOFF);
    delay_ns(100);
}

void Display::vertical_scroll_definition(uint16_t topFixedLines, uint16_t scrollLines, uint16_t bottomFixedLines) {
    write_command(ST7789_VSCRDEF);
    write_data(topFixedLines >> 8u);
    write_data(topFixedLines & 0x00ffu);
    write_data(scrollLines >> 8u);
    write_data(scrollLines & 0x00ffu);
    write_data(bottomFixedLines >> 8u);
    write_data(bottomFixedLines & 0x00ffu);
}

void Display::vertical_scroll_start_address(uint16_t line) {
    verticalScrollingStartAddress = line;
    //assert(line < 320);
    write_command(ST7789_VSCSAD);
    write_data(line >> 8u);
    write_data(line & 0x00ffu);
}


void Display::partial_display(uint8_t mode) {
    write_command(mode == 1 ? ST7789_PTLON : ST7789_NORON);
}

void Display::set_part_area(uint16_t sr, uint16_t er) {
    write_command(ST7789_PTLAR);
    write_data(sr >> 8u);
    write_data(sr & 0x00ffu);
    write_data(er >> 8u);
    write_data(er & 0x00ffu);
}

void Display::draw_pixel(uint16_t x, uint16_t y, uint32_t color) {
    //if((x < 0) ||(x >= Width) || (y < 0) || (y >= Height)) return;

    set_addr_window(x, y, x + 1, y + 1);
    digitalWrite(LCD_DC, HIGH);
    write_spi(reinterpret_cast<const uint8_t*>(&color), 2);
}

void Display::draw_buffer(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t* data, size_t size) {

    set_addr_window(x, y, x + width - 1, y + height - 1);
    digitalWrite(LCD_DC, HIGH);
    write_spi(data, size);

}

void Display::hardware_reset() {
    digitalWrite(LCD_RST, LOW);
    delay_ns(100);
    digitalWrite(LCD_RST, HIGH);
    delay_ns(100);
}

void Display::sleep(void) {
    sleep_in();
}

void Display::wake_up(void) {
    sleep_out();
    vertical_scroll_definition(0, 320, 0);
    vertical_scroll_start_address(verticalScrollingStartAddress);
    display_on();
}