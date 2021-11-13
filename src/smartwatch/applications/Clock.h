#include <stdint.h>
#include "lvgl.h"
#include "touch.h"
#include "base_app.h"
#include "smartwatch.h"

class Clock : public Application
{
    public:
        Clock( Smartwatch * smartwatch ) : Application(smartwatch->get_main_screen()), smartwatch{smartwatch} {

            lv_obj_set_style_bg_color(this->screen, lv_color_hex(0xff0000), 0);
            lv_obj_set_style_bg_opa(this->screen, LV_OPA_COVER, 0);

            lv_timeh = lv_label_create(this->screen);    
            lv_obj_set_style_text_font(lv_timeh, &lv_font_clock_90, 0);
            lv_label_set_text_fmt(lv_timeh, "%02i", 21);
            lv_obj_set_style_text_align(lv_timeh, LV_TEXT_ALIGN_CENTER, 0);
            lv_obj_set_style_text_color(lv_timeh, lv_color_hex(0xffffff), 0);
            lv_obj_align(lv_timeh, LV_ALIGN_CENTER, -55, -35);

            lv_timem = lv_label_create(this->screen);    
            lv_obj_set_style_text_font(lv_timem, &lv_font_clock_90, 0);
            lv_label_set_text_fmt(lv_timem, "%02i", 10);
            lv_obj_set_style_text_align(lv_timem, LV_TEXT_ALIGN_CENTER, 0);
            lv_obj_set_style_text_color(lv_timem, lv_color_hex(0x00ff00), 0);
            lv_obj_align(lv_timem, LV_ALIGN_CENTER, 55, -35);

            lv_times = lv_label_create(this->screen);    
            lv_obj_set_style_text_font(lv_times, &lv_font_clock_90, 0);
            lv_label_set_text_static(lv_times, ":");
            lv_obj_set_style_text_align(lv_times, LV_TEXT_ALIGN_CENTER, 0);
            lv_obj_set_style_text_color(lv_times, lv_color_hex(0xffff00), 0);
            lv_obj_align(lv_times, LV_ALIGN_CENTER, 0, -35);
            
            lv_info = lv_label_create( this->screen );
            lv_label_set_text(lv_info, "MY-Time - Ready to rock");
            lv_obj_set_style_text_color(lv_info, lv_color_hex(0x00ffff), 0);
            lv_obj_set_width(lv_info, 220);
            lv_label_set_long_mode(lv_info, LV_LABEL_LONG_SCROLL_CIRCULAR);
            lv_obj_set_style_text_align(lv_info, LV_TEXT_ALIGN_CENTER, 0);
            lv_obj_align(lv_info, LV_ALIGN_CENTER, 0, 50);

        };

        bool gestures(Touch::Gestures gesture) { 
            switch (gesture) {
                case Touch::Gestures::SlideDown:
                    smartwatch->load_application(Applications::Debug, Smartwatch::RefreshDirections::Down);
                    return true;
                default:
                    return false;
            }
            return false; 
        };

    protected:
        Smartwatch * smartwatch;
        lv_obj_t * lv_timeh;
        lv_obj_t * lv_timem;
        lv_obj_t * lv_times;
        lv_obj_t * lv_info;

};

