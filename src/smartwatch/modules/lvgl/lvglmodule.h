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

        LvglModule(Display& display, Touch& touch);
        void init(void);
        void set_refresh_direction(refreshDirections direction);
        void set_touch_data(Touch::Gestures gesture, uint8_t event, uint16_t x, uint16_t y) {
            this->touch_gesture = gesture;
            this->touch_event = event;
            this->touch_xpos = x;
            this->touch_ypos = y;
        };

        Touch::Gestures get_touch_gesture(void) { return this->touch_gesture; };
        void set_touch_gesture(Touch::Gestures gesture) { this->touch_gesture = gesture; };

        refreshDirections get_direction(void) { return this->scrollDirection; };

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
        lv_indev_drv_t indev_drv;

        refreshDirections scrollDirection = refreshDirections::None;

        Display display;
        Touch   touch;

        Touch::Gestures touch_gesture = Touch::Gestures::None;
        uint8_t touch_event;
        uint16_t touch_xpos;
        uint16_t touch_ypos;

        static void touchpad_read(lv_indev_drv_t* indev_drv, lv_indev_data_t* data);
        static void disp_flush(lv_disp_drv_t* disp_drv, const lv_area_t* area, lv_color_t* color_p);

        void flush_display(const lv_area_t* area, lv_color_t* color_p);
        void touchpad(lv_indev_data_t* data);

};

#endif /* LVGL_MODULE_H */