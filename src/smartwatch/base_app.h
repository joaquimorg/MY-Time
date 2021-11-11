#ifndef BASE_APP_H_
#define BASE_APP_H_


#include "stdint.h"
#include "lvgl.h"
#include "touch.h"

class Application
{
    public:
        Application( lv_obj_t * parent ) {
            this->updateInterval = 1000;
            this->screen = lv_obj_create(parent);
        };

        ~Application() {
            lv_obj_clean(this->screen);
            lv_obj_del(this->screen);
            this->screen = NULL;
        };

        virtual void update() {};
        virtual bool gestures(Touch::Gestures gesture) { return false; };

    protected:

        uint32_t updateInterval;
        
        lv_obj_t *screen;

};

#endif // BASE_APP_H_