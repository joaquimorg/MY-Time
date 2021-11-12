#ifndef LVGL_MODULE_H
#define LVGL_MODULE_H

#include <stdbool.h>
#include <stdint.h>

#include "display.h"
#include "touch.h"
#include "lvgl.h"

class LvglModule
{
  public:
    
    enum class refreshDirections { None, Up, Down, Left, Right };

    LvglModule(Display &display, Touch &touch);
    void init(void);
    void set_refresh_direction(refreshDirections direction);
    
    void flush_display(const lv_area_t *area, lv_color_t *color_p);
    void touchpad(lv_indev_data_t* data);

  protected:
    
    static constexpr uint16_t totalNbLines = 320;
    static constexpr uint16_t visibleNbLines = 240;

    static constexpr uint16_t displayHOR = 240;
    static constexpr uint16_t displayVER = 240;

    static constexpr uint16_t bufferSize = 15;

    uint16_t writeOffset = 0;
    uint16_t scrollOffset = 0;

    lv_disp_draw_buf_t draw_buf_dsc;
    lv_color_t buf_2_1[displayHOR * bufferSize];

    lv_disp_drv_t disp_drv;

    refreshDirections scrollDirection = refreshDirections::None;

    Display display;
    Touch   touch;

};

#endif /* LVGL_MODULE_H */