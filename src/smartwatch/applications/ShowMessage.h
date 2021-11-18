#include "stdint.h"
#include "lvgl.h"
#include "base_app.h"
#include "smartwatch.h"

class ShowMessage : public Application
{
    public:
        ShowMessage( Smartwatch * smartwatch ) : Application(smartwatch->get_main_screen()), smartwatch{smartwatch} {

            lv_obj_t * lv_title = lv_label_create(this->screen);
            lv_label_set_text_fmt(lv_title, "%s", smartwatch->get_notification_title());
            lv_obj_set_style_text_align(lv_title, LV_TEXT_ALIGN_CENTER, 0);
            lv_obj_set_style_text_color(lv_title, lv_color_hex(0x00ffff), 0);
            lv_obj_align(lv_title, LV_ALIGN_TOP_MID, 0, 10);
            
            lv_obj_t * lv_msg = lv_label_create(this->screen);    
            lv_label_set_text_fmt(lv_msg, "%s", smartwatch->get_notification_text());
            lv_obj_set_style_text_align(lv_msg, LV_TEXT_ALIGN_CENTER, 0);
            if ( smartwatch->get_notification_type() == Smartwatch::MessageType::Error ) {
                lv_obj_set_style_text_color(lv_msg, lv_color_hex(0xff0000), 0);
            } else {
                lv_obj_set_style_text_color(lv_msg, lv_color_hex(0x00ff00), 0);
            }            
            lv_obj_align(lv_msg, LV_ALIGN_CENTER, 0, -30);
            lv_obj_set_width(lv_msg, 230);
            lv_label_set_long_mode(lv_msg, LV_LABEL_LONG_WRAP);

            closeMessage = lv_timer_create(ShowMessage::close_msg, 40000, this);

        };

        static void close_msg(lv_timer_t * timer) {  
            auto user_data = static_cast<ShowMessage *>(timer->user_data);
            user_data->close_application();
            lv_timer_del(timer);
        }

        void close_application(void) {
            smartwatch->load_application(Applications::Clock, Smartwatch::RefreshDirections::Down);
        };

        bool gestures(Touch::Gestures gesture) { 
            /*switch (gesture) {
                
                default:
                    return false;
            }*/
            return true; 
        };

    protected:
        Smartwatch * smartwatch;
        lv_timer_t * closeMessage;

};

