#include "stdint.h"
#include "lvgl.h"
#include "base_app.h"
#include "smartwatch.h"

class Steps : public Application
{
    public:
        Steps( Smartwatch * smartwatch ) : Application(smartwatch->get_main_screen()), smartwatch{smartwatch} {

            ax = 0;
            ay = 0;
            az = 0;
            lv_obj_set_style_bg_opa(this->screen, LV_OPA_COVER, 0);
            lv_obj_set_style_bg_color(this->screen, lv_color_hex(0x005500), 0);
            lv_obj_set_style_radius(this->screen, 15, 0);

            lv_info = lv_label_create( this->screen );
            //lv_label_set_text(lv_info, "\xEE\xA4\x84 steps today");
            lv_label_set_text(lv_info, "Welcome to\nMy-Time !");
            lv_obj_set_style_text_color(lv_info, lv_color_hex(0xffffff), 0);
            lv_obj_set_style_text_align(lv_info, LV_TEXT_ALIGN_CENTER, 0);
            //lv_obj_align(lv_info, LV_ALIGN_CENTER, 0, 20);
            lv_obj_align(lv_info, LV_ALIGN_CENTER, 0, 0);

            /*lv_steps = lv_label_create( this->screen );
            //lv_label_set_text_fmt(lv_steps, "%li", smartwatch->stepCount.getStepCounterOutput());
            lv_label_set_text(lv_steps, "0");
            lv_obj_set_style_text_color(lv_steps, lv_color_hex(0xffffff), 0);
            lv_obj_set_style_text_align(lv_steps, LV_TEXT_ALIGN_CENTER, 0);
            lv_obj_align(lv_steps, LV_ALIGN_CENTER, 0, -30);
            lv_obj_set_style_text_font(lv_steps, &lv_font_38, 0);

            //smartwatch->stepCount.getAcceleration(&ax, &ay, &az);
            lv_status = lv_label_create( this->screen );
            lv_label_set_text_fmt(lv_status, "X(%i) Y(%i) Z(%i)", ax, ay, az);
            lv_obj_set_style_text_color(lv_status, lv_color_hex(0xffff00), 0);
            lv_obj_set_style_text_align(lv_status, LV_TEXT_ALIGN_CENTER, 0);
            lv_obj_align(lv_status, LV_ALIGN_BOTTOM_MID, 0, 0);*/

            set_update_interval(500);
            update();
            
        };

        void update(void) {
           //lv_label_set_text_fmt(lv_steps, "%li", smartwatch->stepCount.getStepCounterOutput());
           //smartwatch->stepCount.getAcceleration(&ax, &ay, &az);
           //lv_label_set_text_fmt(lv_status, "X(%i) Y(%i) Z(%i)", ax, ay, az);
        };

        bool gestures(Touch::Gestures gesture) {
            switch (gesture) {
                case Touch::Gestures::SlideLeft:
                    smartwatch->load_application(Applications::Clock, Smartwatch::RefreshDirections::Left);
                    return true;
                default:
                    return false;
            }
            return false;
        };

    protected:
        Smartwatch * smartwatch;
        lv_obj_t * lv_info;
        lv_obj_t * lv_steps;
        lv_obj_t * lv_status;
        int16_t ax;
        int16_t ay;
        int16_t az;

};

