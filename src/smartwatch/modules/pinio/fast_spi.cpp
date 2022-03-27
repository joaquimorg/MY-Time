/*
 * Copyright (c) 2020 Aaron Christophel
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */


#include "fast_spi.h"
#include "pinetime_board.h"


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


void init_fast_spi() {

  NRF_SPIM2->ENABLE = SPIM_ENABLE_ENABLE_Disabled << SPIM_ENABLE_ENABLE_Pos;
  NRF_SPIM2->PSEL.SCK = SPI_SCK;
  NRF_SPIM2->PSEL.MOSI = SPI_MOSI;
  NRF_SPIM2->PSEL.MISO = SPI_MISO;

  NRF_SPIM2->CONFIG = (SPIM_CONFIG_ORDER_MsbFirst << SPIM_CONFIG_ORDER_Pos) |
    (SPIM_CONFIG_CPOL_ActiveLow << SPIM_CONFIG_CPOL_Pos) |
    (SPIM_CONFIG_CPHA_Trailing << SPIM_CONFIG_CPHA_Pos);

  NRF_SPIM2->FREQUENCY = SPIM_FREQUENCY_FREQUENCY_M8 << SPIM_FREQUENCY_FREQUENCY_Pos;

  // setup CMD pin
  NRF_GPIOTE->CONFIG[1] = GPIOTE_CONFIG_MODE_Task << GPIOTE_CONFIG_MODE_Pos |
    GPIOTE_CONFIG_POLARITY_Toggle << GPIOTE_CONFIG_POLARITY_Pos |
    LCD_DC << GPIOTE_CONFIG_PSEL_Pos |
    GPIOTE_CONFIG_OUTINIT_Low << GPIOTE_CONFIG_OUTINIT_Pos;

  NRF_TIMER4->MODE = 0 << TIMER_MODE_MODE_Pos; // timer mode
  NRF_TIMER4->BITMODE = 0 << TIMER_BITMODE_BITMODE_Pos; // 16 bit
  NRF_TIMER4->PRESCALER = 0 << TIMER_PRESCALER_PRESCALER_Pos; // 16 MHz

  cc_setup(0,1,5,6,10,11);

  // PPI channels for toggeling pin
  for (int channel = 0; channel < 6; channel++) {
    NRF_PPI->CH[channel].EEP = (uint32_t)&NRF_TIMER4->EVENTS_COMPARE[channel];
    if (channel % 2)
      NRF_PPI->CH[channel].TEP = (uint32_t)&NRF_GPIOTE->TASKS_SET[1];
    else
      NRF_PPI->CH[channel].TEP = (uint32_t)&NRF_GPIOTE->TASKS_CLR[1];
  }


  NRF_PPI->CH[6].EEP = (uint32_t)&NRF_SPIM2->EVENTS_STARTED;
  NRF_PPI->CH[6].TEP = (uint32_t)&NRF_TIMER4->TASKS_CLEAR;

  NRF_PPI->CH[7].EEP = (uint32_t)&NRF_SPIM2->EVENTS_STARTED;
  NRF_PPI->CH[7].TEP = (uint32_t)&NRF_TIMER4->TASKS_START;

}

void enable_spi(bool state) {
  if (state)
    //NRF_SPIM2->ENABLE = 7;
    NRF_SPIM2->ENABLE = SPIM_ENABLE_ENABLE_Enabled << SPIM_ENABLE_ENABLE_Pos;
  else {
    /*while (NRF_SPIM2->ENABLE == 0) {
      NRF_SPIM2->ENABLE = 0;
    }*/
    NRF_SPIM2->ENABLE = SPIM_ENABLE_ENABLE_Disabled << SPIM_ENABLE_ENABLE_Pos;
    pinMode(SPI_SCK, OUTPUT);
    digitalWrite(SPI_SCK, HIGH);
  }
}


void enable_workaround(NRF_SPIM_Type* spim, uint32_t ppi_channel, uint32_t gpiote_channel) {
  NRF_GPIOTE->CONFIG[gpiote_channel] = (GPIOTE_CONFIG_MODE_Event << GPIOTE_CONFIG_MODE_Pos) |
    (spim->PSEL.SCK << GPIOTE_CONFIG_PSEL_Pos) |
    (GPIOTE_CONFIG_POLARITY_Toggle << GPIOTE_CONFIG_POLARITY_Pos);

  NRF_PPI->CH[ppi_channel].EEP = (uint32_t)&NRF_GPIOTE->EVENTS_IN[gpiote_channel];
  NRF_PPI->CH[ppi_channel].TEP = (uint32_t)&spim->TASKS_STOP;
  NRF_PPI->CHENSET = 1U << ppi_channel;
}

void disable_workaround(NRF_SPIM_Type* spim, uint32_t ppi_channel, uint32_t gpiote_channel) {
  NRF_GPIOTE->CONFIG[gpiote_channel] = 0;
  NRF_PPI->CH[ppi_channel].EEP = 0;
  NRF_PPI->CH[ppi_channel].TEP = 0;
  NRF_PPI->CHENSET = ppi_channel;
}


void write_fast_spi(const uint8_t* ptr, uint32_t len, bool mode) {

  if (mode)
    NRF_GPIOTE->TASKS_SET[1] = 1;
  else
    NRF_GPIOTE->TASKS_CLR[1] = 1;

  /*if (len == 1) {
    enable_workaround(NRF_SPIM2, 1, 1);
  }
  else {
    disable_workaround(NRF_SPIM2, 1, 1);
  }*/

  int v2 = 0;
  do
  {
    NRF_SPIM2->EVENTS_END = 0;
    NRF_SPIM2->EVENTS_ENDRX = 0;
    NRF_SPIM2->EVENTS_ENDTX = 0;
    NRF_SPIM2->TXD.PTR = (uint32_t)ptr + v2;
    if (len <= 0xFF)
    {
      NRF_SPIM2->TXD.MAXCNT = len;
      v2 += len;
      len = 0;
    }
    else
    {
      NRF_SPIM2->TXD.MAXCNT = 255;
      v2 += 255;
      len -= 255;
    }
    NRF_SPIM2->RXD.PTR = 0;
    NRF_SPIM2->RXD.MAXCNT = 0;
    NRF_SPIM2->TASKS_START = 1;
    while (NRF_SPIM2->EVENTS_END == 0);
    NRF_SPIM2->EVENTS_END = 0;
  } while (len);

  NRF_SPIM2->TASKS_STOP = 1;
  while (NRF_SPIM2->EVENTS_STOPPED == 0) __NOP();

}

void write_fast_spi_(const uint8_t* ptr, uint32_t len) {

  int v2 = 0;
  do
  {
    NRF_SPIM2->EVENTS_END = 0;
    NRF_SPIM2->EVENTS_ENDRX = 0;
    NRF_SPIM2->EVENTS_ENDTX = 0;
    NRF_SPIM2->TXD.PTR = (uint32_t)ptr + v2;
    if (len <= 0xFF)
    {
      NRF_SPIM2->TXD.MAXCNT = len;
      v2 += len;
      len = 0;
    }
    else
    {
      NRF_SPIM2->TXD.MAXCNT = 255;
      v2 += 255;
      len -= 255;
    }
    NRF_SPIM2->RXD.PTR = 0;
    NRF_SPIM2->RXD.MAXCNT = 0;
    NRF_SPIM2->TASKS_START = 1;
    while (NRF_SPIM2->EVENTS_END == 0);
    NRF_SPIM2->EVENTS_END = 0;
  } while (len);

  NRF_SPIM2->TASKS_STOP = 1;
  while (NRF_SPIM2->EVENTS_STOPPED == 0) __NOP();

}

void read_fast_spi(uint8_t* ptr, uint32_t len)
{
  /*if (len == 1) {
    enable_workaround(NRF_SPIM2, 7, 7);
  } else {
    disable_workaround(NRF_SPIM2, 7, 7);
  }*/

  int v2 = 0;
  do
  {
    NRF_SPIM2->EVENTS_END = 0;
    NRF_SPIM2->EVENTS_ENDRX = 0;
    NRF_SPIM2->EVENTS_ENDTX = 0;
    NRF_SPIM2->TXD.PTR = 0;
    NRF_SPIM2->TXD.MAXCNT = 0;
    NRF_SPIM2->RXD.PTR = (int)ptr + v2;
    if (len <= 0xFF)
    {
      NRF_SPIM2->RXD.MAXCNT = len;
      v2 += len;
      len = 0;
    }
    else
    {
      NRF_SPIM2->RXD.MAXCNT = 255;
      v2 += 255;
      len -= 255;
    }
    NRF_SPIM2->TASKS_START = 1;
    while (NRF_SPIM2->EVENTS_END == 0);
    NRF_SPIM2->EVENTS_END = 0;
  } while (len);
}

