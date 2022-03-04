#ifndef BACKLIGHT_CONFIG_H
#define BACKLIGHT_CONFIG_H

#include "stdint.h"
#include "nrf52.h"

class Backlight
{
  protected:
    SoftwareTimer lightTimer;
    uint16_t backlightSaveValue = 0;
    uint16_t backlightNewValue = 0;
    uint16_t backlightValue = 0;
    bool isDimmed = false;

    static void timer_callback(TimerHandle_t xTimer);
    void backlight_timer(void);

  public:
    Backlight(void);
    void init(void);    

    void set_value(uint16_t value);
    uint16_t get_value(void) { return backlightValue; }  
    uint16_t get_save_value(void) { return backlightSaveValue; }
    void save_value(void) { if(!isDimmed) { backlightSaveValue = backlightValue;} }

    void dim(void) {
      backlightSaveValue = backlightValue;
      if (backlightValue > 5) {
        set_value(5);
      }
      isDimmed = true;
    };

    bool is_dimmed(void) { return isDimmed; };

};

#endif //BACKLIGHT_CONFIG_H