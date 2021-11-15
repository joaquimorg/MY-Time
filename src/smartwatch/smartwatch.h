#ifndef SMARTWATCH_H
#define SMARTWATCH_H

#include <memory>
#include <stdint.h>
#include "lvgl.h"
#include "base_app.h"
#include "rtc_time.h"
#include "backlight.h"
#include "display.h"
#include "touch.h"
#include "battery.h"
#include "lvglmodule.h"

enum class Applications
{
    None,
    Debug,
    Clock,
};

class Smartwatch
{

    public:
        RTCTime rtc_time;
        Battery battery;
        Backlight backlight;
        Display display;
        Touch touch;
        LvglModule lvglmodule{display, touch};

        enum class States
        {
            Idle,
            Running
        };
        
        enum class RefreshDirections { None, Up, Down, Left, Right };

        enum class Messages : uint8_t {
            WakeUp,
            GoToSleep,
            BleConnected,
            BleDisconnected,
            BleData,
            OnTouchEvent,
            OnButtonEvent,
            OnChargingEvent,
            OnPowerEvent,
            ReloadIdleTimer
        };

        States state = States::Running;

        Smartwatch(void);
        void init(void);        
        void push_message(Messages msg);
        void hardware_update(void);
        void setDebug(uint32_t debug) { this->debug = debug; };

        const char* get_reset_reason(void) { return this->resetReason; };
        lv_obj_t * get_main_screen(void) { return this->main_scr; };

        void set_refresh_direction(RefreshDirections dir);
        void load_application(Applications app, RefreshDirections dir);

        void set_ble_data(uint8_t msgType, uint8_t* data, uint8_t len);

    protected:
        QueueHandle_t msgQueue;
        static constexpr uint8_t queueSize = 10;
        static constexpr uint8_t itemSize = 1;

        bool doNotGoToSleep = false;
        bool stopLvgl = false;

        TaskHandle_t _smartwatchHandle;
        TaskHandle_t _lvglHandle;
        TimerHandle_t idleTimer;

        lv_timer_t * appUpdate;

        lv_obj_t * main_scr;

        uint32_t debug = 0;
        const char * resetReason;

        uint32_t displayTimeout;
        Touch::Gestures gesture = Touch::Gestures::None;
        Touch::Gestures returnGesture = Touch::Gestures::None;

        std::unique_ptr<Application> currentApplication;
        Applications currentApp = Applications::None;
        Applications returnToApp = Applications::None;

        RefreshDirections returnDirection = RefreshDirections::None;

        static void task(void *instance);
        void run(void);
        static void lvgl_task(void *instance);
        void run_lvgl(void);
        static void idle_callback(TimerHandle_t xTimer);
        void on_idle();

        void sleep();
        void wakeup();

        void return_app(Applications app, Touch::Gestures gesture, RefreshDirections dir);
        static void lv_update_app(lv_timer_t * timer);
        void update_application(void);
};

#endif //SMARTWATCH_H