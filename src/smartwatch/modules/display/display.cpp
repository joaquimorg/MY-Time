#include "display.h"
#include "pinetime_board.h"
#include "nrf52.h"
#include "nrf.h"


/**
 * Constructor
 */
Display::Display(void) {

}

// Fast SPI original code from : https://github.com/TT-392/pinetime-bad-apple

#define NOP         0x00
#define SWRESET     0x01
#define SLPIN		0x10
#define SLPOUT      0x11
#define NORON       0x13
//#define INVOFF      0x20
#define INVON       0x21
#define DISPOFF     0x28
#define DISPON      0x29
#define CASET       0x2a
#define RASET       0x2b
#define RAMWR       0x2c
#define VSCRDEF     0x33
//#define TEON        0x34
#define TEOFF       0x35
#define COLMOD      0x3a
#define MADCTL      0x36
#define VSCSAD      0x37
#define PORCTRL     0xB2  // Porch Setting
#define GCTRL       0xB7    // Gate Contro
#define VCOMS       0xBB    // VCOMS Setting
#define LCMCTRL     0xC0  // LCM Control
#define VDVVRHEN    0xC2 // VDV and VRH Command Enabl
#define VRHS        0xC3
#define VDVS        0xC4
#define FRCTRL2     0xC6
#define PWCTRL1     0xD0
#define PVGAMCTRL   0xE0
#define NVGAMCTRL   0xE1
#define RAMCTRL     0xB0 // RAM control

struct display_cmd {
  uint8_t cmd;
  const uint8_t *data;
  uint8_t len;
};

#define ppi_set() NRF_PPI->CHENSET = 0xff; __disable_irq();// enable first 8 ppi channels
#define ppi_clr() NRF_PPI->CHENCLR = 0xff; __enable_irq(); // disable first 8 ppi channels

const static struct display_cmd display_init_data[] = {
    //{SWRESET, NULL},
    //{SLPOUT, NULL},
    {COLMOD, (uint8_t *)"\x55", 1}, // MCU will send 16-bit RGB565 = 55 / 12-bit = 53
    {MADCTL, (uint8_t *)"\x00", 1}, // Left to right, top to bottom
    {PORCTRL, (uint8_t *)"\x0C\x0C\x00\x33\x33", 5},
    {GCTRL, (uint8_t *)"\x45", 1},
    {VCOMS, (uint8_t *)"\x2b", 1},
    {LCMCTRL, (uint8_t *)"\x2c", 1},
    {VDVVRHEN, (uint8_t *)"\x01\xFF", 2},
    {VRHS, (uint8_t *)"\x11", 1},
    {VDVS, (uint8_t *)"\x20", 1},
    {FRCTRL2, (uint8_t *)"\x0F", 1},
    {PWCTRL1, (uint8_t *)"\xA4\xA1", 2},
    {PVGAMCTRL, (uint8_t *)"\xd0\x00\x05\x0e\x15\x0d\x37\x43\x47\x09\x15\x12\x16\x19", 14},
    {NVGAMCTRL, (uint8_t *)"\xd0\x00\x05\x0d\x0c\x06\x2d\x44\x40\x0e\x1c\x18\x16\x19", 14},
    //{RAMCTRL, (uint8_t *)"\x00\xf8", 2},
    //{TEOFF, NULL},
    {INVON, NULL},
    {NORON, NULL},
    {NOP, NULL},
};


void cc_setup(int flip1, int flip2, int flip3, int flip4, int flip5, int flip6) {
    // the following CC setup will cause byte 0, 5 and 10 
    // of any SPIM0 dma transfer to be treated as CMD bytes
    // offset from spim events started to first bit
    // cmd pin gets sampled at end of byte, so we flip it in the middle for better stability
    // 2 * 1 nibble = 8 = offset to get to middle of byte
    NRF_TIMER4->CC[0] = 5 + 8 + (8 * flip1 * 2);
    NRF_TIMER4->CC[1] = 5 + 8 + (8 * flip2 * 2);
    NRF_TIMER4->CC[2] = 5 + 8 + (8 * flip3 * 2);
    NRF_TIMER4->CC[3] = 5 + 8 + (8 * flip4 * 2);
    NRF_TIMER4->CC[4] = 5 + 8 + (8 * flip5 * 2);
    NRF_TIMER4->CC[5] = 5 + 8 + (8 * flip6 * 2);
} // TODO: I am pretty sure there is no reason to have the first flip at 0, this is a wasted CC and ppi


void display_send(bool mode, uint8_t byte) {
    nrf_gpio_pin_write(LCD_DC, mode);


    NRF_SPIM0->TXD.MAXCNT = 1;
    NRF_SPIM0->TXD.PTR = (uint32_t)&byte;

    NRF_SPIM0->EVENTS_ENDTX = 0;
    NRF_SPIM0->EVENTS_ENDRX = 0;
    NRF_SPIM0->EVENTS_END = 0;

    NRF_SPIM0->TASKS_START = 1;
    while(NRF_SPIM0->EVENTS_ENDTX == 0) __NOP();
    while(NRF_SPIM0->EVENTS_END == 0) __NOP();
    NRF_SPIM0->TASKS_STOP = 1;
    while (NRF_SPIM0->EVENTS_STOPPED == 0) __NOP();
}

void display_sendbuffer(const uint8_t* m_tx_buf, int m_length) {
    NRF_SPIM0->TXD.MAXCNT = m_length;
    NRF_SPIM0->TXD.PTR = (uint32_t)m_tx_buf;

    NRF_SPIM0->EVENTS_ENDTX = 0;
    NRF_SPIM0->EVENTS_ENDRX = 0;
    NRF_SPIM0->EVENTS_END = 0;

    NRF_SPIM0->TASKS_START = 1;
    while(NRF_SPIM0->EVENTS_ENDTX == 0) __NOP();
    while(NRF_SPIM0->EVENTS_END == 0) __NOP();
    NRF_SPIM0->TASKS_STOP = 1;
    while (NRF_SPIM0->EVENTS_STOPPED == 0) __NOP();
}

// send a bunch of bytes from buffer
void display_sendbuffer_noblock(uint8_t* m_tx_buf, int m_length) {
    NRF_SPIM0->TXD.MAXCNT = m_length;
    NRF_SPIM0->TXD.PTR = (uint32_t)&m_tx_buf[0];

    NRF_SPIM0->EVENTS_ENDTX = 0;
    NRF_SPIM0->EVENTS_ENDRX = 0;
    NRF_SPIM0->EVENTS_END = 0;

    NRF_SPIM0->TASKS_START = 1;
}

// this function must be called after display_sendbuffer_noblock has been called
// and before the next call of spim related functions. It will wait for spim to
// finish and will then stop spim0
void display_sendbuffer_finish() {
    while(NRF_SPIM0->EVENTS_ENDTX == 0) 
        __NOP();

    while(NRF_SPIM0->EVENTS_END == 0) 
        __NOP();

    NRF_SPIM0->TASKS_STOP = 1;
    while (NRF_SPIM0->EVENTS_STOPPED == 0)
        __NOP();
}

void cmd_enable(bool enabled) {
    if (enabled) {
        // create GPIOTE task to switch LCD_COMMAND pin
        NRF_GPIOTE->CONFIG[1] = GPIOTE_CONFIG_MODE_Task << GPIOTE_CONFIG_MODE_Pos |
            GPIOTE_CONFIG_POLARITY_Toggle << GPIOTE_CONFIG_POLARITY_Pos |
            LCD_DC << GPIOTE_CONFIG_PSEL_Pos | 
            GPIOTE_CONFIG_OUTINIT_Low << GPIOTE_CONFIG_OUTINIT_Pos;
        ppi_set();
    } else {
        NRF_GPIOTE->CONFIG[1] = 0;
        ppi_clr();
        pinMode(LCD_DC, OUTPUT);
    }
}



void display_set_window(uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
  
    //y += 80; // when rotated screen
    display_send (0, CASET);
    display_send (1, x >> 8);
    display_send (1, x);
    display_send (1, (x + w - 1) >> 8);
    display_send (1, x + w - 1);

    display_send (0, RASET);
    display_send (1, y >> 8);
    display_send (1, y);
    display_send (1, (y + h - 1) >> 8);
    display_send (1, (y + h - 1) & 0xFF);

    display_send (0, RAMWR);

}


void display_display_off(void) {
    display_send (0, DISPOFF);
}


void display_display_on(void) {
    display_send (0, DISPON);
}

void display_sleep_out(void) {
    display_send (0, SLPOUT);
    delay_ns(10);
}

void display_sleep_in(void) {
    display_send (0, SLPIN);
    delay_ns(10);
}

void Display::vertical_scroll_start_address(uint16_t line) {
    verticalScrollingStartAddress = line;
    //assert(line < 320);

    display_send (0, VSCSAD);

    display_send (1, line >> 8u);
    display_send (1, line & 0x00ffu);

}

void Display::vertical_scroll_definition(uint16_t topFixedLines, uint16_t scrollLines, uint16_t bottomFixedLines, uint16_t line) {

    verticalScrollingStartAddress = line;
    /*display_send (0, VSCRDEF);

    display_send (1, topFixedLines >> 8u);
    display_send (1, topFixedLines & 0x00ffu);
    display_send (1, scrollLines >> 8u);
    display_send (1, scrollLines & 0x00ffu);
    display_send (1, bottomFixedLines >> 8u);
    display_send (1, bottomFixedLines & 0x00ffu);*/
 
    cc_setup(0,1,7,8,9,10);
    ppi_set();

    uint8_t byteArray[12];
    byteArray[0] = VSCRDEF;

    byteArray[1] = topFixedLines >> 8;
    byteArray[2] = topFixedLines & 0xff;

    byteArray[3] = scrollLines >> 8;
    byteArray[4] = scrollLines & 0xff;

    byteArray[5] = bottomFixedLines >> 8;
    byteArray[6] = bottomFixedLines & 0xff;

    byteArray[7] = MADCTL;
    byteArray[8] = 0x0;

    byteArray[9] = VSCSAD;

    byteArray[10] = line >> 8;
    byteArray[11] = line & 0xff;

    display_sendbuffer(byteArray, 12);

    /**/

    // the following CC setup will cause byte 0, 5 and 10 
    // of any SPIM0 dma transfer to be treated as CMD bytes
    cc_setup(0,1,5,6,10,11);
    ppi_clr();

}

void display_row_address_set() {
  
    display_send (0, RASET);

    display_send (1, 0x00);
    display_send (1, 0x00);
    display_send (1, 320u >> 8u);
    display_send (1, 320u & 0xffu);

}

void Display::sleep(void) {
    display_sleep_in();
    delay_ns(50);
    display_display_off();
    //nrf_gpio_cfg_default(pinDataCommand);  
}

void Display::wake_up(void) {
    //pinMode(pinDataCommand, OUTPUT);
    //delay_ns(50);
    display_sleep_out();
    vertical_scroll_definition(0, 320, 0, verticalScrollingStartAddress);
    //display_vertical_scroll_start_address(verticalScrollingStartAddress);
    display_display_on();
}


void Display::draw_square(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
    ppi_set();

    int maxLength = 254; 
    uint8_t byteArray [maxLength + 1];

    // addresses are offset by 1 to give the ability to recycle the array
    /* setup display for writing */
    byteArray[1] = CASET;

    byteArray[2] = x1 >> 8;
    byteArray[3] = x1 & 0xff;

    byteArray[4] = x2 >> 8;
    byteArray[5] = x2 & 0xff;

    byteArray[6] = RASET;

    byteArray[7] = y1 >> 8;
    byteArray[8] = y1 & 0xff;

    byteArray[9] = y2 >> 8;
    byteArray[10] = y2 & 0xff;

    byteArray[11] = RAMWR;
    /**/

    int area = (x2-x1+1)*(y2-y1+1);

    int areaToWrite;
    if (area > (maxLength - 11) / 2)
        areaToWrite = (maxLength - 11) / 2;
    else 
        areaToWrite = area;


    for (int i = 0; i < areaToWrite; i++) {
        byteArray[12+i*2] = color >> 8;
        byteArray[12+i*2+1] = color & 0xff;
    }

    area -= areaToWrite;

    // non blocking SPI here is negligible and unreliable cause stuff 
    // would be written memory while sending that same memory
    display_sendbuffer(byteArray + 1, (areaToWrite * 2)+11);
    ppi_clr();

    if (area > 0) {
        for (int i = 0; i < 6; i++) {
            byteArray[i*2] = color >> 8;
            byteArray[i*2+1] = color;
        }

        while (area > 0) {
            if (area > maxLength / 2)
                areaToWrite = maxLength / 2;
            else 
                areaToWrite = area;

            area -= areaToWrite;

            display_sendbuffer(byteArray, areaToWrite * 2);
        }
    }
}


void Display::draw_bitmap (uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t* bitmap) {
    //digitalWrite(LCD_CSN, LOW);
    ppi_set();

    int maxLength = 32; 
    uint8_t byteArray[maxLength];

    /* setup display for writing */
    byteArray[0] = CASET;

    byteArray[1] = x1 >> 8;
    byteArray[2] = x1 & 0xff;

    byteArray[3] = (x2 - 1) >> 8;
    byteArray[4] = (x2 - 1) & 0xff;

    byteArray[5] = RASET;

    byteArray[6] = y1 >> 8;
    byteArray[7] = y1 & 0xff;

    byteArray[8] = (y2 - 1) >> 8;
    byteArray[9] = (y2 - 1) & 0xff;

    byteArray[10] = RAMWR;
    /**/

    int areaToWrite;
    int area = (x2-x1)*(y2-y1);

    if (area > maxLength / 2 - 11)
        areaToWrite = maxLength / 2 - 11;
    else 
        areaToWrite = area;

    for (int i = 0; i < areaToWrite; i++) {
        byteArray[i*2 + 11] = bitmap[i*2];
        byteArray[i*2+1 + 11] = bitmap[i*2+1];
    }

    area -= areaToWrite;

    display_sendbuffer_noblock(byteArray, (areaToWrite * 2)+11);
    display_sendbuffer_finish();
    ppi_clr();

    int offset = 0;

    while (area > 0) {
        offset += areaToWrite*2;

        if (area > maxLength / 2)
            areaToWrite = maxLength / 2;
        else 
            areaToWrite = area;

        display_sendbuffer_noblock(bitmap+offset, areaToWrite * 2);
        display_sendbuffer_finish();
        area -= areaToWrite;
    }    
    ppi_clr();
    
    //digitalWrite(LCD_CSN, HIGH);
}

void Display::init(void) {

    digitalWrite(LCD_DC, HIGH);
    digitalWrite(LCD_CSN, HIGH);
    pinMode(SPI_MOSI, OUTPUT);
    pinMode(SPI_SCK, OUTPUT);
    //nrf_gpio_cfg_input(SPI_MISO, NRF_GPIO_PIN_NOPULL);

    pinMode(LCD_DC, OUTPUT);
    pinMode(LCD_CSN, OUTPUT);

    NRF_SPIM0->ENABLE = SPIM_ENABLE_ENABLE_Disabled << SPIM_ENABLE_ENABLE_Pos;
    NRF_SPIM0->PSEL.SCK  = SPI_SCK;
    NRF_SPIM0->PSEL.MOSI = SPI_MOSI;
    NRF_SPIM0->PSEL.MISO = SPI_MISO;

    NRF_SPIM0->CONFIG = (SPIM_CONFIG_ORDER_MsbFirst  << SPIM_CONFIG_ORDER_Pos)|
                        (SPIM_CONFIG_CPOL_ActiveLow  << SPIM_CONFIG_CPOL_Pos) |
                        (SPIM_CONFIG_CPHA_Trailing   << SPIM_CONFIG_CPHA_Pos);

    NRF_SPIM0->FREQUENCY = SPIM_FREQUENCY_FREQUENCY_M8 << SPIM_FREQUENCY_FREQUENCY_Pos;
    NRF_SPIM0->ENABLE = SPIM_ENABLE_ENABLE_Enabled << SPIM_ENABLE_ENABLE_Pos;

    /* deliver a reset */
    digitalWrite(LCD_RST, LOW);
    pinMode(LCD_RST, OUTPUT);
    delay_ns(10);
    digitalWrite(LCD_RST, HIGH);
    delay_ns(50);

    digitalWrite(LCD_CSN, LOW);

    /* initialize the display */
    display_send (0, SWRESET);
    delay_ns(20);

    display_send (0, SLPOUT);
    delay_ns(10);

    for (const struct display_cmd *i = display_init_data; i->cmd != NOP; i++) {
        display_send (0, i->cmd);
        for (size_t di = 0; di < i->len; di++) {
          display_send (1, i->data[di]);
        }
    }
    display_row_address_set();
    
    /* enable the display */
    display_send (0, DISPON);

    NRF_TIMER4->MODE = 0 << TIMER_MODE_MODE_Pos; // timer mode
    NRF_TIMER4->BITMODE = 0 << TIMER_BITMODE_BITMODE_Pos; // 16 bit
    NRF_TIMER4->PRESCALER = 0 << TIMER_PRESCALER_PRESCALER_Pos; // 16 MHz

    // the following CC setup will cause byte 0, 5 and 10 
    // of any SPIM0 dma transfer to be treated as CMD bytes
    cc_setup(0,1,5,6,10,11);


    // PPI channels for toggeling pin
    for (int channel = 0; channel < 6; channel++) { 
        NRF_PPI->CH[channel].EEP = (uint32_t) &NRF_TIMER4->EVENTS_COMPARE[channel];
        if (channel % 2)
            NRF_PPI->CH[channel].TEP = (uint32_t) &NRF_GPIOTE->TASKS_SET[1];
        else 
            NRF_PPI->CH[channel].TEP = (uint32_t) &NRF_GPIOTE->TASKS_CLR[1];
    }


    NRF_PPI->CH[6].EEP = (uint32_t) &NRF_SPIM0->EVENTS_STARTED;
    NRF_PPI->CH[6].TEP = (uint32_t) &NRF_TIMER4->TASKS_CLEAR;

    NRF_PPI->CH[7].EEP = (uint32_t) &NRF_SPIM0->EVENTS_STARTED;
    NRF_PPI->CH[7].TEP = (uint32_t) &NRF_TIMER4->TASKS_START;

    cmd_enable(1);
    //verticalScrollingStartAddress = 0; 
    //display_vertical_scroll_definition(0, 320, 0, verticalScrollingStartAddress);
    //draw_square(0, 0, 239, 239, RGB2COLOR(0x00, 0x00, 0x00));
    draw_square(0, 0, 239, 239, RGB2COLOR(0xa0, 0x00, 0x00));
}

