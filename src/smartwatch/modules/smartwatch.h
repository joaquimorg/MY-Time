#ifndef SMARTWATCH_H
#define SMARTWATCH_H

#include "stdint.h"
#include "lvgl.h"
#include "backlight.h"
#include "display.h"
#include "lvglmodule.h"


class Smartwatch
{

  public:
    Backlight   backlight;
    Display     display;
    LvglModule  lvglmodule;
    
    Smartwatch(void);
    void init(void);    

  protected:
    
    TaskHandle_t  _smartwatchHandle;
    lv_obj_t *main_scr;

    static void task(void *instance);
    void run(void);

};

#endif //SMARTWATCH_H