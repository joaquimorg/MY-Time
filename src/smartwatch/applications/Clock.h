#include "stdint.h"
#include "lvgl.h"
#include "base_app.h"

class Clock : public Application
{
    public:
        Clock( lv_obj_t * parent ) : Application(parent) {

            label1 = lv_label_create(screen);
            lv_label_set_text_fmt(label1, "MY-Time - Ready to rock");
            lv_obj_set_style_text_color(label1, lv_color_make(0xff, 0xff, 0xff), 0);
            lv_obj_set_width(label1, 220);
            lv_label_set_long_mode(label1, LV_LABEL_LONG_SCROLL_CIRCULAR);
            lv_obj_set_style_text_align(label1, LV_TEXT_ALIGN_CENTER, 0);
            lv_obj_align(label1, LV_ALIGN_CENTER, 0, 0);

        };

    protected:

        lv_obj_t * label1;

};

