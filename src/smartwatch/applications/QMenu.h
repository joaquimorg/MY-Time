#include "stdint.h"
#include "lvgl.h"
#include "base_app.h"
#include "smartwatch.h"

class QMenu : public Application
{
    public:
        QMenu( Smartwatch * smartwatch ) : Application(smartwatch->get_main_screen()), smartwatch{smartwatch} {

            smartwatch->rtc_time.get_time();

            lv_ble = lv_label_create(this->screen);
            lv_obj_align(lv_ble, LV_ALIGN_TOP_RIGHT, -5, 5);
            lv_label_set_text(lv_ble, "\xEE\xA4\x83");
            if ( smartwatch->is_blutooth_connected() ) {
                lv_obj_set_style_text_color(lv_ble, lv_color_hex(0x0000a0), 0);
            } else {
                lv_obj_set_style_text_color(lv_ble, lv_color_hex(0x606060), 0);
            }

            lv_power = lv_label_create(this->screen);
            lv_obj_set_style_text_color(lv_power, lv_color_hex(0x606060), 0);
            lv_obj_align(lv_power, LV_ALIGN_TOP_RIGHT, -34, 5);
            if ( smartwatch->is_charging() ) {
                lv_label_set_text_fmt(lv_power, "%s %i%%", smartwatch->battery.get_icon(), smartwatch->battery.get_percent_remaining() == -1 ? 0 : smartwatch->battery.get_percent_remaining());
            } else {
                lv_label_set_text_fmt(lv_power, "\xEE\xA4\x85 %s %i%%", smartwatch->battery.get_icon(), smartwatch->battery.get_percent_remaining() == -1 ? 0 : smartwatch->battery.get_percent_remaining());
            }
            
            lv_time = lv_label_create(this->screen);
            lv_obj_set_style_text_color(lv_time, lv_color_hex(0x606060), 0);
            lv_obj_align(lv_time, LV_ALIGN_TOP_LEFT, 5, 5);
            lv_label_set_text_fmt(lv_time, "%02d:%02d", smartwatch->rtc_time.hours(), smartwatch->rtc_time.minutes());

            btn_dont_disturb = lv_btn_create(this->screen);
            lv_obj_add_event_cb(btn_dont_disturb, QMenu::button_event_handler, LV_EVENT_CLICKED, this);
            lv_obj_add_flag(btn_dont_disturb, LV_OBJ_FLAG_CHECKABLE);
            lv_obj_add_state(btn_dont_disturb, smartwatch->get_disturb_mode() ? LV_STATE_DEFAULT : LV_STATE_CHECKED);
            lv_obj_align(btn_dont_disturb, LV_ALIGN_CENTER, -55, -20);
            lv_obj_set_style_bg_color(btn_dont_disturb, lv_color_hex(0x006600), LV_STATE_CHECKED);

            lbl_dont_disturb = lv_label_create(btn_dont_disturb);
            lv_obj_set_style_text_font(lbl_dont_disturb, &lv_font_sys_48, 0);
            lv_label_set_text_static(lbl_dont_disturb, smartwatch->get_disturb_mode() ? "\xEE\xA4\x8B" : "\xEE\xA4\x8C");
            lv_obj_center(lbl_dont_disturb);    

            btn_backlight = lv_btn_create(this->screen);
            lv_obj_add_event_cb(btn_backlight, QMenu::button_event_handler, LV_EVENT_CLICKED, this);    
            lv_obj_align(btn_backlight, LV_ALIGN_CENTER, 55, -20);

            lbl_backlight = lv_label_create(btn_backlight);
            lv_obj_set_style_text_font(lbl_backlight, &lv_font_sys_48, 0);
            lv_label_set_text_static(lbl_backlight, smartwatch->backlight.get_icon(smartwatch->backlight.get_level()));
            lv_obj_center(lbl_backlight);

            btn_settings = lv_btn_create(this->screen);
            lv_obj_add_event_cb(btn_settings, QMenu::button_event_handler, LV_EVENT_CLICKED, this);
            lv_obj_align(btn_settings, LV_ALIGN_CENTER, 0, 65);

            lv_obj_t * btn_label = lv_label_create(btn_settings);
            lv_obj_set_style_text_font(btn_label, &lv_font_sys_48, 0);
            lv_label_set_text_static(btn_label, "\xEE\xA4\x82");
            lv_obj_center(btn_label);


            set_update_interval(5000);
            update();
        };

        void update(void) {
            smartwatch->rtc_time.get_time();
            lv_label_set_text_fmt(lv_time, "%02d:%02d", smartwatch->rtc_time.hours(), smartwatch->rtc_time.minutes());

            if ( smartwatch->is_charging() ) {
                lv_label_set_text_fmt(lv_power, "%s %i%%", smartwatch->battery.get_icon(), smartwatch->battery.get_percent_remaining() == -1 ? 0 : smartwatch->battery.get_percent_remaining());
            } else {
                lv_label_set_text_fmt(lv_power, "\xEE\xA4\x85 %s %i%%", smartwatch->battery.get_icon(), smartwatch->battery.get_percent_remaining() == -1 ? 0 : smartwatch->battery.get_percent_remaining());
            }

            if ( smartwatch->is_blutooth_connected() ) {
                lv_obj_set_style_text_color(lv_ble, lv_color_hex(0x0000a0), 0);
            } else {
                lv_obj_set_style_text_color(lv_ble, lv_color_hex(0x606060), 0);
            }

            lv_label_set_text_static(lbl_backlight, smartwatch->backlight.get_icon(smartwatch->backlight.get_level()));

            lv_label_set_text_static(lbl_dont_disturb, smartwatch->get_disturb_mode() ? "\xEE\xA4\x8B" : "\xEE\xA4\x8C");
        };

        static void button_event_handler(lv_event_t * e) {            
            QMenu* user_data = static_cast<QMenu *>(lv_event_get_user_data(e));
            user_data->button_event(lv_event_get_current_target(e));
        }

        void button_event(lv_obj_t * bt) {

            if ( bt == btn_dont_disturb ) {
                smartwatch->set_disturb_mode(!lv_obj_has_state(bt, LV_STATE_CHECKED));
            } else if ( bt == btn_backlight ) {
                uint8_t level = smartwatch->backlight.get_level() + 1;
                if ( level > 3 ) {
                    level = 1;
                }
                 smartwatch->backlight.set_level(level);
            } else if ( bt == btn_settings ) {
                //smartwatch->settings.show();
                smartwatch->load_application(Applications::Debug, Smartwatch::RefreshDirections::Down);
            }

            update();
        }

        bool gestures(Touch::Gestures gesture) { 
            switch (gesture) {
                case Touch::Gestures::SlideUp:
                    smartwatch->load_application(Applications::Clock, Smartwatch::RefreshDirections::Up);
                    return true;
                default:
                    return false;
            }
            return false; 
        };

    protected:
        Smartwatch * smartwatch;
        lv_obj_t * lv_ble;
        lv_obj_t * lbl_backlight;
        lv_obj_t * btn_backlight;
        lv_obj_t * lbl_dont_disturb;
        lv_obj_t * btn_dont_disturb;
        lv_obj_t * lv_time;
        lv_obj_t * lv_power;
        lv_obj_t * btn_settings;

};

