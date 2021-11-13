#ifndef BASE_APP_H_
#define BASE_APP_H_


#include "stdint.h"
#include "lvgl.h"
#include "touch.h"

class Application
{
    public:
        Application( lv_obj_t *parrent ) {

            this->updateInterval = 1000;
            this->screen = lv_obj_create(parrent);
            lv_obj_clear_flag(this->screen, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(this->screen, LV_OBJ_FLAG_SCROLL_ELASTIC);
            lv_obj_remove_style_all(this->screen);                            /*Make it transparent*/
            lv_obj_set_size(this->screen, lv_pct(100), lv_pct(100));
            
            /*lv_obj_t * backgoundLabel = lv_label_create( this->screen );
            lv_label_set_text_static(backgoundLabel, "");
            lv_obj_set_size(backgoundLabel, 240, 240);
            lv_obj_set_pos(backgoundLabel, 0, 0);*/
        };

        ~Application() {
            lv_obj_clean(this->screen);
            lv_obj_del(this->screen);
            this->screen = NULL;
        };

        virtual void update(void) {};
        virtual bool gestures(Touch::Gestures gesture) { return false; };
        uint32_t get_update_interval(void) { return this->updateInterval; };

    protected:
        
        uint32_t updateInterval;
        
        lv_obj_t *screen;

};

#endif // BASE_APP_H_