/*
 * Copyright (c) 2020 Aaron Christophel
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include "nrf52.h"

#define ppi_set() NRF_PPI->CHENSET = 0xff; __disable_irq();// enable first 8 ppi channels
#define ppi_clr() NRF_PPI->CHENCLR = 0xff; __enable_irq(); // disable first 8 ppi channels

void cc_setup(int flip1, int flip2, int flip3, int flip4, int flip5, int flip6);
void write_fast_spi_(const uint8_t* ptr, uint32_t len);

void init_fast_spi();
void enable_spi(bool state);
void write_fast_spi(const uint8_t *ptr, uint32_t len, bool mode);
void read_fast_spi(uint8_t *ptr, uint32_t len);

