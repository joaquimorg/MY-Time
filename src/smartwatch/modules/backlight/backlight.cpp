#include "backlight.h"
#include "pinetime_board.h"
#include "nrf52.h"

/**
 * Constructor
 */
Backlight::Backlight(void) {

}


void Backlight::timer_callback(TimerHandle_t xTimer) {
    auto bl = static_cast<Backlight *>(pvTimerGetTimerID(xTimer));
    bl->backlight_timer();
}

void Backlight::init(void){
  
   //pinMode(LCD_LIGHT_1, OUTPUT);
   //pinMode(LCD_LIGHT_2, OUTPUT);
   //pinMode(LCD_LIGHT_3, OUTPUT);

   //digitalWrite(LCD_LIGHT_1, HIGH);
   //digitalWrite(LCD_LIGHT_2, HIGH);
   //digitalWrite(LCD_LIGHT_3, HIGH);

   HwPWM0.addPin( LCD_LIGHT_3 );
   HwPWM0.begin();
   HwPWM0.setMaxValue(128);
   HwPWM0.setClockDiv(PWM_PRESCALER_PRESCALER_DIV_16); // freq = 1Mhz

   HwPWM0.writePin(LCD_LIGHT_3, backlightValue, true);

   lightTimer.begin(ms2tick(6), Backlight::timer_callback, this, pdTRUE);
   
}

void Backlight::backlight_timer(void) {
   
   if ( backlightValue > backlightNewValue ) {
      backlightValue--;
   } else if ( backlightValue < backlightNewValue ) {
      backlightValue++;
   }
   HwPWM0.writePin(LCD_LIGHT_3, backlightValue, true);

   if ( backlightNewValue == backlightValue ) {
      lightTimer.stop();
      if ( backlightValue == 0 ) {
         HwPWM0.writePin(LCD_LIGHT_3, 0, true);
         digitalWrite(LCD_LIGHT_3, HIGH);
      }
   }
}

void Backlight::set_value(uint16_t value) {
   backlightNewValue = value;
   //HwPWM0.writePin(LCD_LIGHT_3, backlightValue, true);
   lightTimer.start();
   isDimmed = false;
}
