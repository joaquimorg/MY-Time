#include "stdint.h"
#include "lvgl.h"
#include "base_app.h"
#include "smartwatch.h"

class HeartRate : public Application
{
    public:
        HeartRate( Smartwatch * smartwatch ) : Application(smartwatch->get_main_screen()), smartwatch{smartwatch} {

            lv_obj_set_style_bg_opa(this->screen, LV_OPA_COVER, 0);
            lv_obj_set_style_bg_color(this->screen, lv_color_hex(0x000055), 0);
            lv_obj_set_style_radius(this->screen, 15, 0);

            lv_info = lv_label_create( this->screen );
            lv_label_set_text(lv_info, "\xEE\xA4\x81 Heart Rate");
            lv_obj_set_style_text_color(lv_info, lv_color_hex(0xffffff), 0);
            lv_obj_set_style_text_align(lv_info, LV_TEXT_ALIGN_CENTER, 0);
            lv_obj_align(lv_info, LV_ALIGN_CENTER, 0, 0);

            set_update_interval(5000);
            update();
            
        };

        void update(void) {
           
        };

        bool gestures(Touch::Gestures gesture) {
            switch (gesture) {
                case Touch::Gestures::SlideRight:
                    smartwatch->load_application(Applications::Clock, Smartwatch::RefreshDirections::Right);
                    return true;
                default:
                    return false;
            }
            return false;  
        };

    protected:
        Smartwatch * smartwatch;
        lv_obj_t * lv_info;

};

