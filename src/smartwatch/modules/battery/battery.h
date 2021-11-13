#ifndef BATTERY_H
#define BATTERY_H

#include "stdint.h"
#include "pinetime_board.h"

class Battery
{
  protected:
    uint16_t batteryVoltage;
    int batteryPercentRemaining;

    static constexpr uint8_t bufferSize = 5;
    int16_t batt_buffer[bufferSize];
    uint8_t batt_buffer_index = 0;

  public:
    Battery(void);

    void read();
    uint16_t get_voltage(void) { return batteryVoltage; };
    int get_percent_remaining(void) { return batteryPercentRemaining; };
    const char * get_icon(void);

};


#endif //BATTERY_H