#ifndef SMARTWATCH_H
#define SMARTWATCH_H

#include "stdint.h"
#include "lvgl.h"
#include "backlight.h"
#include "display.h"
#include "lvglmodule.h"


class Smartwatch
{

  public:
    Backlight   backlight;
    Display     display;
    LvglModule  lvglmodule;    

    enum class States {Idle, Running};
    enum class Messages {
      WakeUp, GoToSleep, BleConnected, BleDisconnected, OnTouchEvent, OnButtonEvent, ReloadIdleTimer
    };
    
    States state = States::Running;

    Smartwatch(void);
    void init(void);
    void run(void);
    void push_message(Smartwatch::Messages msg);
    void hardware_update(void);

  protected:
    
    QueueHandle_t msgQueue;
    static constexpr uint8_t queueSize = 10;
    static constexpr uint8_t itemSize = 1;

    bool doNotGoToSleep = false;
    
    TaskHandle_t  _smartwatchHandle;
    TimerHandle_t idleTimer;

    lv_obj_t *main_scr;

    uint32_t displayTimeout;

    static void task(void *instance);    
    static void idle_callback(TimerHandle_t xTimer);
    void on_idle();

    void sleep();
    void wakeup();

};

#endif //SMARTWATCH_H