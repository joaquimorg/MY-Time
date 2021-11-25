#include "stdint.h"
#include "lvgl.h"
#include "base_app.h"
#include "smartwatch.h"

extern uint32_t NoInit_BackUpTime __attribute__((section(".noinit")));

class AppDebug : public Application
{
    public:
        AppDebug( Smartwatch * smartwatch ) : Application(smartwatch->get_main_screen()), smartwatch{smartwatch} {

            lv_info = lv_label_create( this->screen );
            lv_label_set_text(lv_info, "");
            lv_obj_set_style_text_color(lv_info, lv_color_hex(0x00ffff), 0);
            lv_obj_set_style_text_align(lv_info, LV_TEXT_ALIGN_CENTER, 0);
            lv_obj_align(lv_info, LV_ALIGN_CENTER, 0, 0);

            set_update_interval(5000);
            update();
        };

        void update(void) {
            lv_label_set_text_fmt(lv_info, "%1i.%02i volts %d%%\n%s\nver: %s\nHfree: %i\nSfree: %i\n%li", 
                smartwatch->battery.get_voltage() / 1000,
                smartwatch->battery.get_voltage() % 1000 / 10,
                smartwatch->battery.get_percent_remaining(),
                smartwatch->get_reset_reason(),
                getBootloaderVersion(),
                dbgHeapFree(),
                (dbgStackTotal() - dbgStackUsed()),
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

