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
   HwPWM0.setMaxValue(256);
   HwPWM0.setClockDiv(PWM_PRESCALER_PRESCALER_DIV_16); // freq = 1Mhz

   HwPWM0.writePin(LCD_LIGHT_3, backlightValue, true);

   lightTimer = xTimerCreate ("lightTimer", ms2tick(2), pdTRUE, this, Backlight::timer_callback);
   
}

void Backlight::backlight_timer(void) {
   
   if ( backlightValue > backlightNewValue ) {
      backlightValue--;
   } else if ( backlightValue < backlightNewValue ) {
      backlightValue++;
   }
   HwPWM0.writePin(LCD_LIGHT_3, backlightValue, true);

   if ( backlightNewValue == backlightValue ) {
      xTimerStop(lightTimer, 0);
      backlightLevel = backlightNewLevel;
      if ( backlightLevel == 0 ) {
         HwPWM0.writePin(LCD_LIGHT_3, 0, true);
         digitalWrite(LCD_LIGHT_3, HIGH);
      }
   }
}

void Backlight::set_level(uint8_t level) {
      
   if ( level == backlightLevel ) return;
   isDimmed = false;
   switch (level) {
      case 0:
         backlightNewLevel = 0;
         backlightNewValue = 0;
         
         xTimerStart(lightTimer, 0);
         break;
      case 1:
         backlightNewLevel = 1;
         backlightNewValue = 64;
         
         xTimerStart(lightTimer, 0);
         break;
      case 2:       
         backlightNewLevel = 2;
         backlightNewValue = 128;
         
         xTimerStart(lightTimer, 0);
         break;
      case 3:
         backlightNewLevel = 3;
         backlightNewValue = 256;
         
         xTimerStart(lightTimer, 0);
         break;

      case 9:
         isDimmed = true;
         backlightNewLevel = 9;
         backlightNewValue = 16;
         
         xTimerStart(lightTimer, 0);
         break;

      default:
         break;
   }
   
}

uint8_t Backlight::get_level(void){
   return backlightLevel;
}

const char * Backlight::get_icon(uint8_t level) {
   if ( level == 9 ) level = backlightSaveLevel;
   switch (level) {
      case 1:
         return "\xEE\xA4\x85";
      break;
      case 2:
         return "\xEE\xA4\x86";
      break;
      case 3:
         return "\xEE\xA4\x84";
      break;
      default:
         return "\xEE\xA4\x85";
      break;
   }
}