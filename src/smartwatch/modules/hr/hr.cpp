#include "hr.h"
#include "pinetime_board.h"
#include "nrf52.h"
#include "i2c.h"

#include "nrf52.h"
#include "hrs3300.h"

#define HRS3300_TEST      30

extern "C" { 
  
  int __hardfp_sqrt(int f){ return sqrt(f); } 

  void Hrs3300_write_reg(uint8_t addr, uint8_t data) {
    user_i2c_write(HR_TWI_ADDR, addr, &data, 1);
  }

  uint8_t Hrs3300_read_reg(uint8_t addr) {
    uint8_t reg_temp = 0;
    user_i2c_read(HR_TWI_ADDR, addr, &reg_temp, 1);
    return reg_temp;
  }

}


/**
 * Constructor
 */
HR::HR(void) {
}

void HR::init(void) {
    pinMode(HRS3300_TEST, INPUT);
    Hrs3300_chip_init();
    Hrs3300_chip_enable();
    Hrs3300_set_exinf(0, 0, 0, 0, 0, 0);
    Hrs3300_alg_open();
    heartrate_enable = true;
    endHR();
}

void HR::beginHR(void) {
    if (!heartrate_enable) {
        Hrs3300_chip_enable();
        heartrate_enable = true;
    }
}


void HR::endHR(void) {
    if (heartrate_enable) {
        heartrate_enable = false;
        Hrs3300_chip_disable();
    }
}

byte HR::getHR(void) {
  byte hr = last_heartrate_ms;
  switch (hr) {
    case 0:
      break;
    case 255:
      break;
    case 254://No Touch
      break;
    case 253://Please wait
      break;
    default:
      last_heartrate = hr;
      break;
  }
  return hr;
}

void HR::getHRms(void) {
  if (heartrate_enable) {
    last_heartrate_ms = get_heart_rate();
  }
}

