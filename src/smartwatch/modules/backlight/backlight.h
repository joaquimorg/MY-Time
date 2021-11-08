#ifndef BACKLIGHT_CONFIG_H
#define BACKLIGHT_CONFIG_H

#include "stdint.h"

class Backlight
{
  protected:
    uint8_t backlightLevel = 0;

  public:
    Backlight(void);
    void init(void);

    void set_level(uint8_t level);
    uint8_t get_level(void);
    const char * get_icon(uint8_t level);

};

#endif //BACKLIGHT_CONFIG_H