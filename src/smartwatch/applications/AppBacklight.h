#include "stdint.h"
#include "lvgl.h"
#include "base_app.h"
#include "smartwatch.h"

class AppBacklight : public Application
{
    public:
        AppBacklight( Smartwatch * smartwatch ) : Application(smartwatch->get_main_screen()), smartwatch{smartwatch} {

            lv_obj_set_style_bg_opa(this->screen, LV_OPA_COVER, 0);
            lv_obj_set_style_bg_color(this->screen, lv_color_hex(0x040404), 0);
            lv_obj_set_style_radius(this->screen, 15, 0);

            lv_info = lv_label_create( this->screen );
            lv_label_set_text(lv_info, "Backlight");
            lv_obj_set_style_text_color(lv_info, lv_color_hex(0xffffff), 0);
            lv_obj_set_style_text_align(lv_info, LV_TEXT_ALIGN_CENTER, 0);
            lv_obj_align(lv_info, LV_ALIGN_TOP_MID, 0, 5);

            static lv_style_t style_main;
            lv_style_init(&style_main);
            lv_style_set_bg_opa(&style_main, LV_OPA_COVER);
            lv_style_set_bg_color(&style_main, lv_color_hex(0xa0a0a0));
            lv_style_set_radius(&style_main, LV_RADIUS_CIRCLE);
            lv_style_set_pad_ver(&style_main, -2); /*Makes the indicator larger*/

            static lv_style_t style_indicator;
            lv_style_init(&style_indicator);
            lv_style_set_bg_opa(&style_indicator, LV_OPA_COVER);
            lv_style_set_bg_color(&style_indicator, lv_color_hex(0x00ff00));
            lv_style_set_radius(&style_indicator, LV_RADIUS_CIRCLE);

            static lv_style_t style_knob;            
            lv_style_init(&style_knob);
            lv_style_set_bg_opa(&style_knob, LV_OPA_COVER);
            lv_style_set_bg_color(&style_knob, lv_color_hex(0xffff00));
            //lv_style_set_border_color(&style_knob, lv_palette_darken(LV_PALETTE_CYAN, 3));
            //lv_style_set_border_width(&style_knob, 4);
            lv_style_set_radius(&style_knob, LV_RADIUS_CIRCLE);
            lv_style_set_pad_all(&style_knob, 10); /*Makes the knob larger*/

            slider = lv_slider_create( this->screen );
            lv_obj_add_style(slider, &style_main, LV_PART_MAIN);
            lv_obj_add_style(slider, &style_indicator, LV_PART_INDICATOR);
            lv_obj_add_style(slider, &style_knob, LV_PART_KNOB);            
            lv_slider_set_range(slider, 1, 128);
            lv_slider_set_value(slider, smartwatch->backlight.get_value(), LV_ANIM_ON);
            lv_obj_set_width(slider, 200);
            lv_obj_center(slider);
            lv_obj_add_event_cb(slider, AppBacklight::slider_event_cb, LV_EVENT_VALUE_CHANGED, this);

            slider_label = lv_label_create( this->screen );
            lv_obj_set_style_text_font(slider_label, &lv_font_38, 0);
            lv_label_set_text_fmt(slider_label, "%li%%", lv_slider_get_value(slider) * 100 / 128);

            lv_obj_align_to(slider_label, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 30);

            set_update_interval(1000);
            update();
        };

        static void slider_event_cb(lv_event_t * e) {
            AppBacklight* user_data = static_cast<AppBacklight *>(lv_event_get_user_data(e));
            user_data->slider_event(lv_event_get_current_target(e));
        };

        void slider_event(lv_obj_t * sl) {
            uint32_t value = lv_slider_get_value(sl);
            smartwatch->backlight.set_value(value);
            lv_label_set_text_fmt(slider_label, "%li%%", value * 100 / 128);
            lv_obj_align_to(slider_label, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 30);
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
        lv_obj_t * slider_label;

};

