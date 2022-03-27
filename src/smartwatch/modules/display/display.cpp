#include "display.h"
#include "pinetime_board.h"
#include "nrf52.h"
#include "nrf.h"
#include "fast_spi.h"
#include "I2S.h"
#include <FreeRTOS.h>

#define ST77XX_SLPIN    0x10
#define ST77XX_VSCSAD   0x37
#define ST77XX_VSCRDEF  0x33
#define ST77XX_NOP      0x00
#define ST77XX_SWRESET  0x01
#define ST77XX_SLPOUT   0x11
#define ST77XX_NORON    0x13
#define ST77XX_INVOFF   0x20
#define ST77XX_INVON    0x21
#define ST77XX_DISPOFF  0x28
#define ST77XX_DISPON   0x29
#define ST77XX_CASET    0x2a
#define ST77XX_RASET    0x2b
#define ST77XX_RAMWR    0x2c
#define ST77XX_TEON     0x34
#define ST77XX_TEOFF    0x35
#define ST77XX_COLMOD   0x3a
#define ST77XX_MADCTL   0x36
#define ST77XX_PORCTRL  0xB2    // Porch Setting
#define ST77XX_GCTRL    0xB7    // Gate Contro
#define ST77XX_VCOMS    0xBB    // VCOMS Setting
#define ST77XX_LCMCTRL  0xC0    // LCM Control
#define ST77XX_VDVVRHEN 0xC2    // VDV and VRH Command Enabl
#define ST77XX_VRHS     0xC3
#define ST77XX_VDVS     0xC4
#define ST77XX_FRCTRL2  0xC6
#define ST77XX_PWCTRL1  0xD0
#define ST77XX_PVGAMCTRL 0xE0
#define ST77XX_NVGAMCTRL 0xE1
#define ST77XX_RAMCTRL  0xB0    // RAM control


void _write_fast_spi(const uint8_t *ptr, uint32_t len) {
    write_fast_spi(ptr, len, true);
}

/**
 * Constructor
 */
Display::Display(void) {

}

void Display::vertical_scroll_definition(uint16_t topFixedLines, uint16_t scrollLines, uint16_t bottomFixedLines, uint16_t line) {

    verticalScrollingStartAddress = line;
    //start_write_display();

    uint8_t temp[6];
    spi_command(ST77XX_VSCRDEF);
    temp[0] = topFixedLines >> 8u;
    temp[1] = topFixedLines & 0x00ffu;
    temp[2] = scrollLines >> 8u;
    temp[3] = scrollLines & 0x00ffu;
    temp[4] = bottomFixedLines >> 8u;
    temp[5] = bottomFixedLines & 0x00ffu;
    _write_fast_spi(temp, 6);

    spi_command(ST77XX_VSCSAD);
    temp[0] = line >> 8u;
    temp[1] = line & 0x00ffu;
    _write_fast_spi(temp, 2);

    //end_write_display();
}

void Display::spi_command(uint8_t d) {    
    write_fast_spi(&d, 1, false);
}

void Display::start_write_display(void) {
    enable_spi(true);
    //digitalWrite(LCD_CSN, LOW);    
}

void Display::end_write_display(void) {
    //digitalWrite(LCD_CSN, HIGH);
    enable_spi(false);
}

void Display::sleep(void) {
    start_write_display();
    spi_command(ST77XX_SLPIN);
    spi_command(ST77XX_DISPOFF);
    end_write_display();
    //digitalWrite(LCD_CSN, LOW);
    //digitalWrite(LCD_DC, LOW);
}

void Display::wake_up(void) {
    start_write_display();
    spi_command(ST77XX_DISPON);
    spi_command(ST77XX_SLPOUT);
    //vertical_scroll_definition(0, 320, 0, verticalScrollingStartAddress);
    end_write_display();
}


void Display::set_addr_display(uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
    uint8_t temp[4];

    spi_command(ST77XX_CASET);
    temp[0] = (x >> 8);
    temp[1] = x;
    temp[2] = ((x + w - 1) >> 8);
    temp[3] = (x + w - 1);
    _write_fast_spi(temp, 4);
    spi_command(ST77XX_RASET);
    temp[0] = (y >> 8);
    temp[1] = y;
    temp[2] = ((y + h - 1) >> 8);
    temp[3] = ((y + h - 1) & 0xFF);
    _write_fast_spi(temp, 4);
    spi_command(ST77XX_RAMWR);
}

void Display::init(void) {
    uint8_t temp[15];

    /*if (mutexspi == nullptr) {
        mutexspi = xSemaphoreCreateBinary();
    }*/
    
    init_fast_spi(); 
    I2S_init();   

    pinMode(LCD_CSN, OUTPUT);
    pinMode(LCD_DC, OUTPUT);
    pinMode(LCD_RST, OUTPUT);

    pinMode(SPI_SCK, OUTPUT);
    digitalWrite(SPI_SCK, HIGH);

    digitalWrite(LCD_CSN, HIGH);    
    digitalWrite(LCD_DC, HIGH);

    digitalWrite(LCD_RST, HIGH);
    delay(20);
    digitalWrite(LCD_RST, LOW);
    delay(100);
    digitalWrite(LCD_RST, HIGH);
    delay(100);
    digitalWrite(LCD_CSN, LOW);
    start_write_display();

    //spi_command(ST77XX_SWRESET);
    //delay(100);

    spi_command(ST77XX_SLPOUT);
    delay(120);

    spi_command(ST77XX_MADCTL);
    temp[0] = 0x00; //0xC0 when rotated screen
    _write_fast_spi(temp, 1);

    spi_command(ST77XX_COLMOD);
    temp[0] = 0x55;
    _write_fast_spi(temp, 1);

    spi_command(ST77XX_PORCTRL);
    temp[0] = 0x0C;
    temp[1] = 0x0C;
    temp[2] = 0;
    temp[3] = 0x33;
    temp[4] = 0x33;
    _write_fast_spi(temp, 5);

    spi_command(ST77XX_GCTRL);
    temp[0] = 0x45; //0x35
    _write_fast_spi(temp, 1);

    spi_command(ST77XX_VCOMS);
    temp[0] = 0x2B; //0x19
    _write_fast_spi(temp, 1);

    spi_command(ST77XX_LCMCTRL);
    temp[0] = 0x2C;
    _write_fast_spi(temp, 1);

    spi_command(ST77XX_VDVVRHEN);
    temp[0] = 0x01;
    temp[1] = 0xFF;
    _write_fast_spi(temp, 2);

    spi_command(ST77XX_VRHS);
    temp[0] = 0x11; // 0x12
    _write_fast_spi(temp, 1);

    spi_command(ST77XX_VDVS);
    temp[0] = 0x20;
    _write_fast_spi(temp, 1);

    spi_command(ST77XX_FRCTRL2);
    temp[0] = 0x0F;
    _write_fast_spi(temp, 1);

    spi_command(ST77XX_PWCTRL1);
    temp[0] = 0xA4;
    temp[1] = 0xA1;
    _write_fast_spi(temp, 2);

    spi_command(ST77XX_PVGAMCTRL);
    temp[0]  = 0xD0;
    temp[1]  = 0x00;
    temp[2]  = 0x05;
    temp[3]  = 0x0E;
    temp[4]  = 0x15;
    temp[5]  = 0x0D;
    temp[6]  = 0x37;
    temp[7]  = 0x43;
    temp[8]  = 0x47;
    temp[9]  = 0x09;
    temp[10] = 0x15;
    temp[11] = 0x12;
    temp[12] = 0x16;
    temp[13] = 0x19;
    _write_fast_spi(temp, 14);

    spi_command(ST77XX_NVGAMCTRL);
    temp[0]  = 0xD0;
    temp[1]  = 0x00;
    temp[2]  = 0x05;
    temp[3]  = 0x0D;
    temp[4]  = 0x0C;
    temp[5]  = 0x06;
    temp[6]  = 0x2D;
    temp[7]  = 0x44;
    temp[8]  = 0x40;
    temp[9]  = 0x0E;
    temp[10] = 0x1C;
    temp[11] = 0x18;
    temp[12] = 0x16;
    temp[13] = 0x19;
    _write_fast_spi(temp, 14);

    spi_command(ST77XX_INVON);    

    spi_command(ST77XX_DISPON);

    //set_addr_display(0, 0, 320, 240);
    //vertical_scroll_definition(0, 320, 0, 0);
    end_write_display();

    //xSemaphoreGive(mutexspi);
}

void Display::drawBitmap_I2S_wh(uint16_t x, uint16_t y, uint32_t w, uint32_t h, uint8_t* data) {
    uint8_t temp[4];
    enable_spi(true);
    spi_command(ST77XX_CASET);
    temp[0] = (x >> 8);
    temp[1] = x;
    temp[2] = ((x + w - 1) >> 8);
    temp[3] = (x + w - 1);
    _write_fast_spi(temp, 4);
    spi_command(ST77XX_RASET);
    temp[0] = (y >> 8);
    temp[1] = y;
    temp[2] = ((y + h - 1) >> 8);
    temp[3] = ((y + h - 1) & 0xFF);
    _write_fast_spi(temp, 4);

    spi_command(ST77XX_RAMWR);

    _write_fast_spi(data, (w * h) * 2);

    enable_spi(false);

    //I2S_RAMWR(data, w * h);
}

void Display::drawBitmap_I2S(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t* data) {
    uint8_t temp[4];
    
    enable_spi(true);
    
    spi_command(ST77XX_CASET);
    temp[0] = x1 >> 8;
    temp[1] = x1 & 0xff;
    temp[2] = x2 >> 8;
    temp[3] = x2 & 0xff;
    _write_fast_spi(temp, 4);

    spi_command(ST77XX_RASET);
    temp[0] = y1 >> 8;
    temp[1] = y1 & 0xff;
    temp[2] = y2 >> 8;
    temp[3] = y2 & 0xff;
    _write_fast_spi(temp, 4);
        
    spi_command(ST77XX_RAMWR);

/*
    ppi_set();

    uint8_t byteArray[11];

    // setup display for writing
    byteArray[0] = ST77XX_CASET;

    byteArray[1] = x1 >> 8;
    byteArray[2] = x1 & 0xff;

    byteArray[3] = (x2 - 1) >> 8;
    byteArray[4] = (x2 - 1) & 0xff;

    byteArray[5] = ST77XX_RASET;

    byteArray[6] = y1 >> 8;
    byteArray[7] = y1 & 0xff;

    byteArray[8] = (y2 - 1) >> 8;
    byteArray[9] = (y2 - 1) & 0xff;

    byteArray[10] = ST77XX_RAMWR;


    uint8_t* c = (uint8_t*)realloc(data, sizeof(data) + 11);
    memcpy(c, byteArray,  11);

    //write_fast_spi_(byteArray, 11);
    write_fast_spi_(c, ((((x2 - x1) + 1) * ((y2 - y1) + 1)) * 2) + 11);
    
    ppi_clr();
    //

    free(data);
    */

   _write_fast_spi(data, ((((x2 - x1) + 1) * ((y2 - y1) + 1)) * 2));
    
    enable_spi(false);
    
    //xSemaphoreTake(mutexspi, portMAX_DELAY);    
    //I2S_RAMWR(data, ((x2 - x1) + 1) * ((y2 - y1) + 1));
    //xSemaphoreGive(mutexspi);
    
}
