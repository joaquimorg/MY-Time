#include "stdint.h"
#include "lvgl.h"
#include "base_app.h"
#include "smartwatch.h"

class AppDebug : public Application
{
    public:
        AppDebug( Smartwatch * smartwatch ) : Application(smartwatch->get_main_screen()), smartwatch{smartwatch} {

            
            lv_info = lv_label_create( this->screen );
            lv_label_set_text_fmt(lv_info, "MY-Time - Ready to rock - %s", smartwatch->get_reset_reason());
            lv_obj_set_style_text_color(lv_info, lv_color_hex(0x00ffff), 0);
            lv_obj_set_width(lv_info, 220);
            lv_label_set_long_mode(lv_info, LV_LABEL_LONG_SCROLL_CIRCULAR);
            lv_obj_set_style_text_align(lv_info, LV_TEXT_ALIGN_CENTER, 0);
            lv_obj_align(lv_info, LV_ALIGN_CENTER, 0, 0);

        };

        bool gestures(Touch::Gestures gesture) { 
            switch (gesture) {
                case Touch::Gestures::SlideUp:
                    smartwatch->load_application(Applications::Clock, Smartwatch::RefreshDirections::Up);
                    return true;
            }
            return false; 
        };

    protected:
        Smartwatch * smartwatch;
        lv_obj_t * lv_info;

};

