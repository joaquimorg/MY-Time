#include "smartwatch.h"
#include "nrf52.h"
#include "pinetime_board.h"
#include "utils.h"
#include "lvgl.h"
#include "watchdog.h"
#include "touch.h"

#include "Clock.h"
#include "AppDebug.h"


#define SW_STACK_SZ       (256*8)
#define LVGL_STACK_SZ     (256*2)

/**
 * Constructor
 */
Smartwatch::Smartwatch(void) {
    displayTimeout = 15000;
}

void Smartwatch::idle_callback(TimerHandle_t xTimer) {
    auto sw = static_cast<Smartwatch *>(pvTimerGetTimerID(xTimer));
    sw->on_idle();
}

void Smartwatch::init(void) {

    // Initialize the modules
    display.init();
    touch.init();
    lvglmodule.init();
    backlight.init();
    backlight.set_level(2);

    resetReason = actual_reset_reason();

    // Main queue
    msgQueue = xQueueCreate(queueSize, itemSize);

    main_scr = lv_scr_act();

    load_application(Applications::Clock, RefreshDirections::None);

    idleTimer = xTimerCreate ("idleTimer", ms2tick(displayTimeout), pdFALSE, this, Smartwatch::idle_callback);
    xTimerStart(idleTimer, 0);
    
    // Create a task for smartwatch
    xTaskCreate(Smartwatch::task, "sw", SW_STACK_SZ, this, TASK_PRIO_NORMAL, &_smartwatchHandle);

    // Create a task for lvgl
    xTaskCreate(Smartwatch::lvgl_task, "lvgl", LVGL_STACK_SZ, this, TASK_PRIO_NORMAL, &_lvglHandle);
}

void Smartwatch::task(void *instance) {
    auto *sw = static_cast<Smartwatch *>(instance);
    xTaskNotifyGive(xTaskGetCurrentTaskHandle());    
    while (1) {
        sw->run();
    }
}

void Smartwatch::lvgl_task(void *instance) {
    auto *sw = static_cast<Smartwatch *>(instance);
    watchdog_init(5000);
    while (1) {
        sw->run_lvgl();
    }
}

void Smartwatch::run_lvgl(void) {
    if ( state == States::Running ) {
        if ( !stopLvgl ) {
            lv_timer_handler();
            lv_tick_inc(20);
        }
        vTaskDelay(1);
    } else {
        vTaskDelay(100);
    }
    if (digitalRead(KEY_ACTION) == HIGH) return;
    watchdog_feed();
}


void Smartwatch::hardware_update(void) {
    if ( state == States::Idle ) {
        vTaskDelay(ms2tick(30000));
    } else {
        vTaskDelay(ms2tick(500));
    }    

}

void Smartwatch::set_refresh_direction(RefreshDirections dir) {
    if ( dir == RefreshDirections::Up ) {
        lv_disp_set_direction(lv_disp_get_default(), 0);
        lvglmodule.set_refresh_direction(LvglModule::refreshDirections::Up);
    } else if ( dir == RefreshDirections::Down ) {
        lv_disp_set_direction(lv_disp_get_default(), 1);
        lvglmodule.set_refresh_direction(LvglModule::refreshDirections::Down);
    } else if ( dir == RefreshDirections::Left ) {
        lv_disp_set_direction(lv_disp_get_default(), 2);
        lvglmodule.set_refresh_direction(LvglModule::refreshDirections::Left);
    } else if ( dir == RefreshDirections::Right ) {
        lv_disp_set_direction(lv_disp_get_default(), 3);
        lvglmodule.set_refresh_direction(LvglModule::refreshDirections::Right);
    }
}

void Smartwatch::return_app(Applications app, Touch::Gestures gesture, RefreshDirections dir) {
    returnToApp = app;
    returnDirection = dir;
    returnGesture = gesture;
}

void Smartwatch::load_application(Applications app, RefreshDirections dir) {
    if ( currentApp == app ) return;
    stopLvgl = true;
    currentApp = app;
    currentApplication.reset(nullptr);
    set_refresh_direction(dir);
    switch (app) {
        case Applications::Clock:
            currentApplication = std::make_unique<Clock>(this);
            return_app(Applications::Clock, Touch::Gestures::None, RefreshDirections::None);
            break;

        case Applications::Debug:
            currentApplication = std::make_unique<AppDebug>(this);
            return_app(Applications::Clock, Touch::Gestures::SlideUp, RefreshDirections::Up);
            break;
        default:
            break;
    }
    stopLvgl = false;
}

void Smartwatch::run(void) {

    Messages msg;
    TickType_t queueTimeout = portMAX_DELAY;

    if (xQueueReceive(msgQueue, &msg, queueTimeout)) {
        switch (msg) {
            case Messages::WakeUp:
                if (state == States::Running) break;
                state = States::Running;
                wakeup();
                break;

            case Messages::GoToSleep:
                state = States::Idle;
                sleep();
                break;

            case Messages::BleConnected:
                push_message(Messages::BleData);
                break;

            case Messages::BleDisconnected:
                push_message(Messages::BleData);
                break;

            case Messages::OnTouchEvent:
                touch.read();
                gesture = touch.getGesture();
                if (state == States::Idle) {
                    if ( gesture == Touch::Gestures::DoubleTap ) {
                        push_message(Messages::WakeUp);
                    }
                    break;
                } else {
                    push_message(Messages::ReloadIdleTimer);
                }

                if ( gesture == Touch::Gestures::None ) break;
                
                if ( !currentApplication->gestures(gesture) ) {
                    if ( gesture == returnGesture ) {
                        load_application(returnToApp, returnDirection);
                    }
                }

                break;

            case Messages::BleData:
                if (state == States::Idle) {
                    push_message(Messages::WakeUp);
                    break;
                }
                break;    

            case Messages::OnButtonEvent:
                if (state == States::Idle) {
                    push_message(Messages::WakeUp);
                    break;
                }
                if (state == States::Running) {
                    push_message(Messages::GoToSleep);
                    break;
                }
                
                push_message(Messages::ReloadIdleTimer);
                
                break;

            case Messages::ReloadIdleTimer:
                xTimerReset(idleTimer, 0);
                break;
                        
        }
    }

}

void Smartwatch::on_idle() {
    if(doNotGoToSleep) return;
    push_message(Messages::GoToSleep);
}


void Smartwatch::push_message(Messages msg) {

    if( msgQueue == NULL ) return;

    if (isInISR()) {
        if (xQueueIsQueueFullFromISR(msgQueue)) {
            return;
        }
        BaseType_t xHigherPriorityTaskWoken;
        xHigherPriorityTaskWoken = pdFALSE;
        xQueueSendFromISR(msgQueue, &msg, &xHigherPriorityTaskWoken);
        if (xHigherPriorityTaskWoken) {
            // Actual macro used here is port specific.
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    } else {
        xQueueSend(msgQueue, &msg, ( TickType_t ) 0);
    }
    
}

void Smartwatch::sleep() {
    backlight.set_level(0);
    display.sleep();
    xTimerStop(idleTimer, 0);
}

void Smartwatch::wakeup() {
    backlight.set_level(backlight.get_level());
    display.wake_up();
    xTimerStart(idleTimer, 0);
}
