#include "display.h"
#include "pinetime_board.h"
#include "nrf52.h"
#include "nrf.h"
#include "fast_spi.h"

#define ST77XX_SLPIN    0x10
#define ST77XX_SLPOUT   0x11
#define ST77XX_DISPOFF  0x28
#define ST77XX_DISPON   0x29
#define ST77XX_VSCSAD   0x37
#define ST77XX_VSCRDEF  0x33

/**
 * Constructor
 */
Display::Display(void) {

}


void Display::vertical_scroll_definition(uint16_t topFixedLines, uint16_t scrollLines, uint16_t bottomFixedLines, uint16_t line) {

    verticalScrollingStartAddress = line;
    start_write_display();

    uint8_t temp[6];
    spi_command(ST77XX_VSCRDEF);
    temp[0] = topFixedLines >> 8u;
    temp[1] = topFixedLines & 0x00ffu;
    temp[2] = scrollLines >> 8u;
    temp[3] = scrollLines & 0x00ffu;
    temp[4] = bottomFixedLines >> 8u;
    temp[5] = bottomFixedLines & 0x00ffu;
    write_fast_spi(temp, 6);

    spi_command(ST77XX_VSCSAD);
    temp[0] = line >> 8u;
    temp[1] = line & 0x00ffu;
    write_fast_spi(temp, 2);

    end_write_display();
}

void Display::spi_command(uint8_t d) {
    digitalWrite(LCD_DC , LOW);
    write_fast_spi(&d, 1);
    digitalWrite(LCD_DC , HIGH);
}

void Display::start_write_display(void) {
    enable_spi(true);
    digitalWrite(LCD_CSN , LOW);
}

void Display::end_write_display(void) {
    digitalWrite(LCD_CSN , HIGH);
    enable_spi(false);
}

void Display::sleep(void) {
    start_write_display();
    spi_command(ST77XX_SLPIN);
    spi_command(ST77XX_DISPOFF);
    end_write_display();
}

void Display::wake_up(void) {
    start_write_display();
    spi_command(ST77XX_DISPON);
    spi_command(ST77XX_SLPOUT);
    end_write_display();
}


void Display::set_addr_display(uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
    uint8_t temp[4];
 
    spi_command(0x2A);
    temp[0] = (x >> 8);
    temp[1] = x;
    temp[2] = ((x + w - 1) >> 8);
    temp[3] = (x + w - 1);
    write_fast_spi(temp, 4);
    spi_command(0x2B);
    temp[0] = (y >> 8 );
    temp[1] = y;
    temp[2] = ((y + h - 1) >> 8);
    temp[3] = ((y + h - 1) & 0xFF);
    write_fast_spi(temp, 4);
    spi_command(0x2C);
}

void Display::init(void) {
    uint8_t temp[25];

    init_fast_spi();

    pinMode(LCD_CSN, OUTPUT);
    pinMode(LCD_DC, OUTPUT);
    pinMode(LCD_RST, OUTPUT);    

    digitalWrite(LCD_CSN, HIGH);
    digitalWrite(LCD_DC, HIGH);

    digitalWrite(LCD_RST, HIGH);
    delay(20);
    digitalWrite(LCD_RST, LOW);
    delay(100);
    digitalWrite(LCD_RST, HIGH);
    delay(100);
    start_write_display();
    spi_command(54);
    temp[0] = 0x00;//0xC0 when rotated screen
    write_fast_spi(temp, 1);
    spi_command(58);
    temp[0] = 5;
    write_fast_spi(temp, 1);
    spi_command(178);
    temp[0] = 12;
    temp[1] = 12;
    temp[2] = 0;
    temp[3] = 51;
    temp[4] = 51;
    write_fast_spi(temp, 5);
    spi_command(183);
    temp[0] = 53;
    write_fast_spi(temp, 1);
    spi_command(187);
    temp[0] = 25;
    write_fast_spi(temp, 1);
    spi_command(192);
    temp[0] = 44;
    write_fast_spi(temp, 1);
    spi_command(194);
    temp[0] = 1;
    write_fast_spi(temp, 1);
    spi_command(195);
    temp[0] = 18;
    write_fast_spi(temp, 1);
    spi_command(196);
    temp[0] = 32;
    write_fast_spi(temp, 1);
    spi_command(198);
    temp[0] = 15;
    write_fast_spi(temp, 1);
    spi_command(208);
    temp[0] = 164;
    temp[1] = 161;
    write_fast_spi(temp, 2);
    spi_command(224);
    temp[0] = 208;
    temp[1] = 4;
    temp[2] = 13;
    temp[3] = 17;
    temp[4] = 19;
    temp[5] = 43;
    temp[6] = 63;
    temp[7] = 84;
    temp[8] = 76;
    temp[9] = 24;
    temp[10] = 13;
    temp[11] = 11;
    temp[12] = 31;
    temp[13] = 35;
    write_fast_spi(temp, 14);
    spi_command(225);
    temp[0] = 208;
    temp[1] = 4;
    temp[2] = 12;
    temp[3] = 17;
    temp[4] = 19;
    temp[5] = 44;
    temp[6] = 63;
    temp[7] = 68;
    temp[8] = 81;
    temp[9] = 47;
    temp[10] = 31;
    temp[11] = 31;
    temp[12] = 32;
    temp[13] = 35;
    write_fast_spi(temp, 14);
    spi_command(33);
    spi_command(17);
    delay(120);
    spi_command(41);
    spi_command(0x11);
    spi_command(0x29);
    end_write_display();    
}


