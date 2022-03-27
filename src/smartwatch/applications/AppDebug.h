#include "stdint.h"
#include "lvgl.h"
#include "base_app.h"
#include "smartwatch.h"

extern uint32_t NoInit_BackUpTime __attribute__((section(".noinit")));

/*static void drag_event_handler(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_target(e);

    lv_indev_t * indev = lv_indev_get_act();
    lv_point_t vect;
    lv_indev_get_vect(indev, &vect);

    lv_coord_t x = lv_obj_get_x(obj) + vect.x;
    lv_coord_t y = lv_obj_get_y(obj) + vect.y;
    lv_obj_set_pos(obj, x, y);
}*/

class AppDebug : public Application
{
    public:
        AppDebug( Smartwatch * smartwatch ) : Application(smartwatch->get_main_screen()), smartwatch{smartwatch} {

            lv_obj_set_style_bg_opa(this->screen, LV_OPA_COVER, 0);
            lv_obj_set_style_bg_color(this->screen, lv_color_hex(0x005500), 0);
            lv_obj_set_style_radius(this->screen, 15, 0);

            lv_info = lv_label_create( this->screen );
            update();
            //lv_label_set_text(lv_info, "");
            lv_obj_set_style_text_color(lv_info, lv_color_hex(0xffffff), 0);
            lv_obj_set_style_text_align(lv_info, LV_TEXT_ALIGN_CENTER, 0);
            lv_obj_align(lv_info, LV_ALIGN_CENTER, 0, 0);

            set_update_interval(500);            

            /*lv_obj_t * obj;
            obj = lv_obj_create(this->screen);
            lv_obj_set_size(obj, 150, 100);
            lv_obj_add_event_cb(obj, drag_event_handler, LV_EVENT_PRESSING, NULL);

            lv_obj_t * label = lv_label_create(obj);
            lv_label_set_text(label, "Drag me");
            lv_obj_center(label);*/
        };

        void update(void) {
            //smartwatch->touch.get();
            lv_label_set_text_fmt(lv_info, "%1i.%02i volts %d%%\n%s\nver: %s\ntouch: 0x%02x\n%li", 
                smartwatch->battery.get_voltage() / 1000,
                smartwatch->battery.get_voltage() % 1000 / 10,
                smartwatch->battery.get_percent_remaining(),
                smartwatch->get_reset_reason(),
                getBootloaderVersion(),
                smartwatch->touch.getEvent(),
                NoInit_BackUpTime
                );
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

};

