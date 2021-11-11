#ifndef SMARTWATCH_H
#define SMARTWATCH_H

#include "stdint.h"
#include "lvgl.h"
#include "backlight.h"
#include "display.h"
#include "touch.h"
#include "lvglmodule.h"


class Smartwatch
{

  public:
    Backlight   backlight;
    Display     display;
    Touch       touch;
    LvglModule  lvglmodule {display, touch};    

    enum class States {Idle, Running};
    enum class Messages {
      WakeUp, GoToSleep, BleConnected, BleDisconnected, BleData, OnTouchEvent, OnButtonEvent, ReloadIdleTimer
    };
    
    States state = States::Running;

    Smartwatch(void);
    void init(void);
    void run(void);
    void push_message(Smartwatch::Messages msg);
    void hardware_update(void);
    void setDebug(uint32_t debug) {this->debug = debug;}

  protected:
    
    QueueHandle_t msgQueue;
    static constexpr uint8_t queueSize = 10;
    static constexpr uint8_t itemSize = 1;

    bool doNotGoToSleep = false;
    
    TaskHandle_t  _smartwatchHandle;
    TimerHandle_t idleTimer;

    lv_obj_t *main_scr;
    lv_obj_t * label1;

    uint32_t debug = 0;
    const char* resetReason;

    uint32_t displayTimeout;
    Touch::Gestures gesture = Touch::Gestures::None;

    static void task(void *instance);    
    static void idle_callback(TimerHandle_t xTimer);
    void on_idle();

    void sleep();
    void wakeup();
};

#endif //SMARTWATCH_H