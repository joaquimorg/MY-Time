#include <stdint.h>
#include "lvgl.h"
#include "touch.h"
#include "base_app.h"
#include "smartwatch.h"

class Clock : public Application
{
    public:        

        Clock( Smartwatch * smartwatch ) : Application(smartwatch->get_main_screen()), smartwatch{smartwatch} {

            smartwatch->rtc_time.get_time();

            lv_obj_set_style_bg_opa(this->screen, LV_OPA_COVER, 0);
            lv_obj_set_style_bg_color(this->screen, lv_color_hex(0x550000), 0);
            lv_obj_set_style_radius(this->screen, 15, 0);

            lv_timeh = lv_label_create(this->screen);    
            lv_obj_set_style_text_font(lv_timeh, &lv_font_clock_90, 0);
            lv_label_set_text_fmt(lv_timeh, "%02i", smartwatch->rtc_time.hours());
            lv_obj_set_style_text_align(lv_timeh, LV_TEXT_ALIGN_CENTER, 0);
            lv_obj_set_style_text_color(lv_timeh, lv_color_hex(0xffffff), 0);
            lv_obj_align(lv_timeh, LV_ALIGN_CENTER, -55, -35);

            lv_timem = lv_label_create(this->screen);    
            lv_obj_set_style_text_font(lv_timem, &lv_font_clock_90, 0);
            lv_label_set_text_fmt(lv_timem, "%02i", smartwatch->rtc_time.minutes());
            lv_obj_set_style_text_align(lv_timem, LV_TEXT_ALIGN_CENTER, 0);
            lv_obj_set_style_text_color(lv_timem, lv_color_hex(0x00ff00), 0);
            lv_obj_align(lv_timem, LV_ALIGN_CENTER, 55, -35);

            lv_times = lv_label_create(this->screen);    
            lv_obj_set_style_text_font(lv_times, &lv_font_clock_90, 0);
            lv_label_set_text_static(lv_times, ":");
            lv_obj_set_style_text_align(lv_times, LV_TEXT_ALIGN_CENTER, 0);
            lv_obj_set_style_text_color(lv_times, lv_color_hex(0xffff00), 0);
            lv_obj_align(lv_times, LV_ALIGN_CENTER, 0, -35);

            lv_date = lv_label_create(this->screen);    
            //lv_obj_set_style_text_font(lv_date, &lv_font_clock_42, 0);
            lv_label_set_recolor(lv_date, true);
            lv_label_set_text_fmt(lv_date, "#00ff00 %s# %02i %s", smartwatch->rtc_time.get_days(), smartwatch->rtc_time.day(), smartwatch->rtc_time.get_months());
            lv_obj_set_style_text_align(lv_date, LV_TEXT_ALIGN_CENTER, 0);
            lv_obj_set_style_text_color(lv_date, lv_color_make(0xff, 0xff, 0xff), 0);
            lv_obj_align(lv_date, LV_ALIGN_CENTER, 0, 25);
            
            lv_info = lv_label_create( this->screen );
            lv_label_set_text(lv_info, "MY-Time - Ready to rock");
            lv_obj_set_style_text_color(lv_info, lv_color_hex(0x00ffff), 0);
            lv_obj_set_width(lv_info, 240);
            lv_label_set_long_mode(lv_info, LV_LABEL_LONG_SCROLL_CIRCULAR);
            //lv_obj_set_style_text_align(lv_info, LV_TEXT_ALIGN_CENTER, 0);
            lv_obj_align(lv_info, LV_ALIGN_BOTTOM_LEFT, 0, -25);

            save_old_time();

        };

        void update(void) {
            smartwatch->rtc_time.get_time();

            if (to_hour != smartwatch->rtc_time.hours())
                lv_label_set_text_fmt(lv_timeh, "%02i", smartwatch->rtc_time.hours());
            if (to_minutes != smartwatch->rtc_time.minutes())
                lv_label_set_text_fmt(lv_timem, "%02i", smartwatch->rtc_time.minutes());

            if ( smartwatch->rtc_time.seconds() % 2 == 0 ) {
                lv_label_set_text_static(lv_times,  ":");
            } else {
                lv_label_set_text_static(lv_times,  " ");
            }

            if (smartwatch->rtc_time.day() != to_day) {
                lv_label_set_text_fmt(lv_date, "#00ff00 %s# %02i %s", smartwatch->rtc_time.get_days(), smartwatch->rtc_time.day(), smartwatch->rtc_time.get_months());
            }

            save_old_time();
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
        lv_obj_t * lv_date;
        lv_obj_t * lv_info;

        uint8_t     to_day;      // 1-31
        uint8_t     to_hour;     // 0-23
        uint8_t     to_minutes;  // 0-59
        uint8_t     to_seconds;  // 0-59

        void save_old_time(void) {
            to_day = smartwatch->rtc_time.day();
            to_hour = smartwatch->rtc_time.hours();
            to_minutes = smartwatch->rtc_time.minutes();
            to_seconds = smartwatch->rtc_time.seconds();
        }

};

