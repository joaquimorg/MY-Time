#ifndef VIBRATION_H
#define VIBRATION_H

#include "stdint.h"
#include "nrf52.h"
#include "pinetime_board.h"

class Vibration
{
  protected:
    SoftwareTimer vibrationTimer;
    bool canVibrate = true;

    static void timer_callback(TimerHandle_t xTimer);
    void vibration_timer(void);

  public:
    Vibration(void);

    void init(void);
    void vibrate(uint8_t value, uint16_t time);
    void can_vibrate(bool value) { canVibrate = value; }
    bool vibration(void) { return canVibrate; }
};


#endif //VIBRATION_H