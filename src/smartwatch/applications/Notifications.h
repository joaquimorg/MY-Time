#include "stdint.h"
#include "lvgl.h"
#include "base_app.h"
#include "smartwatch.h"

class Notifications : public Application
{
    public:
        Notifications( Smartwatch * smartwatch ) : Application(smartwatch->get_main_screen()), smartwatch{smartwatch} {

            lv_obj_set_style_bg_opa(this->screen, LV_OPA_COVER, 0);
            lv_obj_set_style_bg_color(this->screen, lv_color_hex(0x101010), 0);
            lv_obj_set_style_radius(this->screen, 15, 0);

            if ( smartwatch->notification.get_notification_count() > 0 ) {
                show_notification();
            } else {
                lv_obj_t * lv_info = lv_label_create(this->screen);
                lv_label_set_recolor(lv_info, true);
                lv_label_set_text_static(lv_info, "\xEE\xA4\x80\n#00FFFF Notification#\n\nNo notifications\nto display.");
                lv_obj_set_style_text_align(lv_info, LV_TEXT_ALIGN_CENTER, 0);
                lv_obj_center(lv_info);
            }

            //closeApp = lv_timer_create(Notifications::close_app, 40000, this);

            set_update_interval(5000);

        };

        void show_notification() {
            uint8_t notification_count = smartwatch->notification.get_notification_index();

            lv_obj_t * lv_title = lv_label_create(this->screen);
            lv_label_set_recolor(lv_title, true);
            lv_label_set_text_fmt(lv_title, "#ffffff \xEE\xA4\x80# %s", smartwatch->notification.get_notification_type(notification_count));
            lv_obj_set_style_text_align(lv_title, LV_TEXT_ALIGN_CENTER, 0);
            lv_obj_set_style_text_color(lv_title, lv_color_hex(0x00ffff), 0);
            lv_obj_align(lv_title, LV_ALIGN_TOP_MID, 0, 10);

            lv_obj_t * lv_count = lv_label_create(this->screen);
            lv_label_set_recolor(lv_count, true);
            lv_label_set_text_fmt(lv_count, "#909090 %i# / #909090 %i#", smartwatch->notification.get_notification_count() - notification_count, smartwatch->notification.get_notification_count());
            lv_obj_set_style_text_align(lv_count, LV_TEXT_ALIGN_CENTER, 0);
            lv_obj_set_style_text_color(lv_count, lv_color_hex(0x009090), 0);
            lv_obj_align(lv_count, LV_ALIGN_BOTTOM_RIGHT, -10, -10);

            lv_obj_t * lv_subject = lv_label_create(this->screen);
            lv_label_set_text_static(lv_subject, smartwatch->notification.get_notification_subject(notification_count));
            lv_obj_set_style_text_align(lv_subject, LV_TEXT_ALIGN_CENTER, 0);
            lv_label_set_long_mode(lv_subject, LV_LABEL_LONG_SCROLL_CIRCULAR);
            lv_obj_set_style_text_color(lv_subject, lv_color_hex(0x909090), 0);
            lv_obj_set_width(lv_subject, 235);
            lv_obj_align(lv_subject, LV_ALIGN_TOP_MID, 0, 40);

            lv_obj_t * lv_message = lv_label_create(this->screen);
            lv_label_set_text_static(lv_message, smartwatch->notification.get_notification_message(notification_count));
            lv_obj_set_style_text_align(lv_message, LV_TEXT_ALIGN_CENTER, 0);
            lv_label_set_long_mode(lv_message, LV_LABEL_LONG_WRAP);
            lv_obj_set_style_text_color(lv_message, lv_color_hex(0xf0f000), 0);
            lv_obj_set_width(lv_message, 239);
            lv_obj_align(lv_message, LV_ALIGN_CENTER, 0, 10);

            lv_obj_t * lv_time = lv_label_create(this->screen);
            uint8_t time_past = smartwatch->rtc_time.diff_minutes(smartwatch->notification.get_notification_timestamp(notification_count));
            if ( time_past > 30 ) {
                lv_label_set_text_fmt(lv_time, "%02i:%02i", smartwatch->notification.get_notification_hour(notification_count), smartwatch->notification.get_notification_minute(notification_count));
            } else if ( time_past < 1 ) {
                lv_label_set_text_static(lv_time, "Just now");
            } else {
                lv_label_set_text_fmt(lv_time, "%i min", time_past);
            }
            lv_obj_set_style_text_color(lv_time, lv_color_hex(0x909090), 0);
            lv_obj_align(lv_time, LV_ALIGN_BOTTOM_LEFT, 10, -10);
        }

        static void close_app(lv_timer_t * timer) {
            auto user_data = static_cast<Notifications *>(timer->user_data);
            user_data->close_application();
            lv_timer_del(timer);
        }

        void close_application(void) {
            //smartwatch->load_application(Applications::Clock, Smartwatch::RefreshDirections::Up);
        };

        bool gestures(Touch::Gestures gesture) {
            switch (gesture) {
                case Touch::Gestures::SlideDown:
                    if ( smartwatch->notification.get_notification_index() > 0) {
                        smartwatch->notification.set_notification_index(smartwatch->notification.get_notification_index() - 1);
                        smartwatch->load_application(Applications::Notifications, Smartwatch::RefreshDirections::Down);
                    } else {
                        smartwatch->load_application(Applications::Clock, Smartwatch::RefreshDirections::Down);
                    }
                    return true;
                case Touch::Gestures::SlideUp:
                    if ( smartwatch->notification.get_notification_index() < smartwatch->notification.get_notification_count() - 1) {
                        smartwatch->notification.set_notification_index(smartwatch->notification.get_notification_index() + 1);
                        smartwatch->load_application(Applications::Notifications, Smartwatch::RefreshDirections::Up);
                    }
                    return true;
                case Touch::Gestures::LongPress:
                    if (smartwatch->notification.get_notification_count() > 0) {
                        smartwatch->notification.clear_notifications();
                        smartwatch->load_application(Applications::Notifications, Smartwatch::RefreshDirections::Right);
                    }
                    return true;
                default:
                    return false;
            }
            return false;
        };

    protected:
        Smartwatch * smartwatch;
        lv_timer_t * closeApp;

};

