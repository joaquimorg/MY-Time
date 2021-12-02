#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "vibration.h"
#include "pinetime_board.h"

#include "nrf52.h"



void Vibration::timer_callback(TimerHandle_t xTimer) {
    auto vr = static_cast<Vibration *>(pvTimerGetTimerID(xTimer));
    vr->vibration_timer();
}

/**
 * Constructor
 */
Vibration::Vibration(void) {
}

void Vibration::init(void) {
    HwPWM1.addPin( VIBRATOR_CTRL );
    HwPWM1.begin();
    HwPWM1.setMaxValue(256);
    HwPWM1.setClockDiv(PWM_PRESCALER_PRESCALER_DIV_16); // freq = 1Mhz

    HwPWM1.writePin(VIBRATOR_CTRL, 0, true);

    vibrationTimer.begin(ms2tick(10), Vibration::timer_callback, this, pdFALSE);
}


void Vibration::vibration_timer(void) {
    HwPWM1.writePin(VIBRATOR_CTRL, 0, true);
    //xTimerStop(vibrationTimer, 0);
}


void Vibration::vibrate(uint8_t value, uint16_t time) {
    if (!canVibrate) return;
    
    HwPWM1.writePin(VIBRATOR_CTRL, value, true);

    vibrationTimer.setPeriod(ms2tick(time));
    vibrationTimer.start();
}
