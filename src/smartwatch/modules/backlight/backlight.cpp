#include "backlight.h"
#include "pinetime_board.h"
#include "nrf52.h"

/**
 * Constructor
 */
Backlight::Backlight(void) {

}

void Backlight::init(void){
  
   pinMode(LCD_LIGHT_1, OUTPUT);
   pinMode(LCD_LIGHT_2, OUTPUT);
   pinMode(LCD_LIGHT_3, OUTPUT);

   digitalWrite(LCD_LIGHT_1, HIGH);
   digitalWrite(LCD_LIGHT_2, HIGH);
   digitalWrite(LCD_LIGHT_3, HIGH);
   
}

void Backlight::set_level(uint8_t level) {
      
   switch (level) {
      case 0:
         digitalWrite(LCD_LIGHT_1, HIGH);
         digitalWrite(LCD_LIGHT_2, HIGH);
         digitalWrite(LCD_LIGHT_3, HIGH);
      break;
      case 1:
         digitalWrite(LCD_LIGHT_1, LOW);
         digitalWrite(LCD_LIGHT_2, HIGH);
         digitalWrite(LCD_LIGHT_3, HIGH);
         backlightLevel = 1;
      break;
      case 2:
         digitalWrite(LCD_LIGHT_1, LOW);
         digitalWrite(LCD_LIGHT_2, LOW);
         digitalWrite(LCD_LIGHT_3, HIGH);
         backlightLevel = 2;
      break;
      case 3:
         digitalWrite(LCD_LIGHT_1, LOW);
         digitalWrite(LCD_LIGHT_2, LOW);
         digitalWrite(LCD_LIGHT_3, LOW);
         backlightLevel = 3;
      break;
      default:
      break;
   }
   
}


const char * Backlight::get_icon(uint8_t level) {
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