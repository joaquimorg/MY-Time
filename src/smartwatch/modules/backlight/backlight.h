#ifndef BACKLIGHT_CONFIG_H
#define BACKLIGHT_CONFIG_H

#include "stdint.h"
#include "nrf52.h"

class Backlight
{
  protected:
    uint8_t backlightLevel = 0;
    uint8_t backlightSaveLevel = 0;
    uint8_t backlightNewLevel = 0;
    TimerHandle_t lightTimer;
    uint16_t backlightNewValue = 0;
    uint16_t backlightValue = 0;
    bool isDimmed = false;

    static void timer_callback(TimerHandle_t xTimer);
    void backlight_timer(void);

  public:
    Backlight(void);
    void init(void);

    void set_level(uint8_t level);
    void save_level(void) { backlightSaveLevel = backlightLevel; }
    uint8_t get_saved_level(void) { return backlightSaveLevel; }
    uint8_t get_level(void);
    const char * get_icon(uint8_t level);

    void dim(void) {
      save_level();
      set_level(9);
    };

    bool is_dimmed(void) { return isDimmed; };

    void restore_dim(void) {
      backlightLevel = backlightSaveLevel;
    };

};

#endif //BACKLIGHT_CONFIG_H