#include "stdint.h"
#include "lvgl.h"
#include "base_app.h"
#include "smartwatch.h"


class AppBacklight : public Application
{
    public:
        AppBacklight( Smartwatch * smartwatch ) : Application(smartwatch->get_main_screen()), smartwatch{smartwatch} {

            lv_info = lv_label_create( this->screen );
            lv_label_set_text(lv_info, "Backlight");
            lv_obj_set_style_text_color(lv_info, lv_color_hex(0x00ffff), 0);
            lv_obj_set_style_text_align(lv_info, LV_TEXT_ALIGN_CENTER, 0);
            lv_obj_align(lv_info, LV_ALIGN_TOP_MID, 0, 0);

            slider = lv_slider_create(this->screen);
            lv_obj_center(slider);


            set_update_interval(1000);
            update();
        };

        void update(void) {
            
        };

        bool gestures(Touch::Gestures gesture) { 
            switch (gesture) {
                case Touch::Gestures::SlideUp:
                    smartwatch->load_application(Applications::QMenu, Smartwatch::RefreshDirections::Up);
                    return true;
                default:
                    return false;
            }
            return false; 
        };

    protected:
        Smartwatch * smartwatch;
        lv_obj_t * lv_info;
        lv_obj_t * slider;

};

