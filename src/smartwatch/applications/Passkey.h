#include "stdint.h"
#include "lvgl.h"
#include "base_app.h"
#include "smartwatch.h"

class Passkey : public Application
{
    public:
        Passkey( Smartwatch * smartwatch ) : Application(smartwatch->get_main_screen()), smartwatch{smartwatch} {

            lv_obj_t * lv_title = lv_label_create(this->screen);
            lv_label_set_text_static(lv_title, "Pairing Key\nEnter the code\nin you device.");
            lv_obj_set_style_text_align(lv_title, LV_TEXT_ALIGN_CENTER, 0);
            lv_obj_set_style_text_color(lv_title, lv_color_hex(0x00ffff), 0);
            lv_obj_align(lv_title, LV_ALIGN_TOP_MID, 0, 10);
            
            lv_obj_t * lv_key = lv_label_create(this->screen);    
            lv_label_set_text_fmt(lv_key, "%s", smartwatch->passkey );
            lv_obj_set_style_text_font(lv_key, &lv_font_clock_42, 0);
            lv_obj_set_style_text_align(lv_key, LV_TEXT_ALIGN_CENTER, 0);
            lv_obj_set_style_text_color(lv_key, lv_color_hex(0x00ff00), 0);
            lv_obj_align(lv_key, LV_ALIGN_CENTER, 0, 30);

        };

        void update(void) {
            
        };

        bool gestures(Touch::Gestures gesture) { 
            switch (gesture) {
                
                default:
                    return false;
            }
            return false; 
        };

    protected:
        Smartwatch * smartwatch;

};

