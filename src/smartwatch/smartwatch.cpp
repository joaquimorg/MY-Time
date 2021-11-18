#include "smartwatch.h"
#include "nrf52.h"
#include "pinetime_board.h"
#include "utils.h"
#include "lvgl.h"
#include "watchdog.h"
#include "touch.h"

#include "Clock.h"
#include "AppDebug.h"
#include "Passkey.h"
#include "ShowMessage.h"
#include "QMenu.h"


#define SW_STACK_SZ       (256*6)
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

void Smartwatch::lv_update_app(lv_timer_t * timer) {  
    auto user_data = static_cast<Smartwatch *>(timer->user_data);
    user_data->update_application();
}

void Smartwatch::init(void) {

    resetReason = actual_reset_reason();

    // Initialize the modules
    rtc_time.init();
    display.init();
    touch.init();
    lvglmodule.init();
    backlight.init();
    backlight.set_level(2);
    vibration.init();

    set_charging(false);

    // Main queue
    msgQueue = xQueueCreate(queueSize, itemSize);

    appUpdate = lv_timer_create(Smartwatch::lv_update_app, 1000, this);
    lv_timer_pause(appUpdate);

    main_scr = lv_scr_act();

    load_application(Applications::Clock, RefreshDirections::None);

    idleTimer = xTimerCreate ("idleTimer", ms2tick(displayTimeout / 2), pdFALSE, this, Smartwatch::idle_callback);
    xTimerStart(idleTimer, 0);
    
    // Create a task for smartwatch
    xTaskCreate(Smartwatch::task, "sw", SW_STACK_SZ, this, TASK_PRIO_NORMAL, &_smartwatchHandle);

    // Create a task for lvgl
    xTaskCreate(Smartwatch::lvgl_task, "lvgl", LVGL_STACK_SZ, this, TASK_PRIO_NORMAL, &_lvglHandle);
        
}

void Smartwatch::update_application(void) {
    if(currentApplication) {
        currentApplication->update();
    }
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
            lv_tick_inc(10);
        }
        vTaskDelay(1);
    } else {
        vTaskDelay(ms2tick(2000));
    }
    if (digitalRead(KEY_ACTION) == HIGH) return;
    watchdog_feed();
}


void Smartwatch::hardware_update(void) {
    if ( state == States::Idle ) {
        vTaskDelay(ms2tick(60000));
    } else {
        vTaskDelay(ms2tick(5000));
    }
    
    battery.read();

    if (digitalRead(CHARGE_IRQ) == HIGH) {
        if (!is_charging()) {
            //push_message(Messages::OnChargingEvent);
            set_charging(true);
        }               
    } else {
        set_charging(false);
    }

    if (digitalRead(CHARGE_BASE_IRQ) == HIGH) {
        //push_message(Messages::OnPowerEvent);
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
    if ( app == Applications::None ) return;
    
    stopLvgl = true;
    currentApp = app;
    lv_timer_pause(appUpdate);
    currentApplication.reset(nullptr);
    switch (app) {
        case Applications::Clock:
            currentApplication = std::make_unique<Clock>(this);
            return_app(Applications::Clock, Touch::Gestures::None, RefreshDirections::None);
            break;
        case Applications::Passkey:
            currentApplication = std::make_unique<Passkey>(this);
            return_app(Applications::Clock, Touch::Gestures::SlideDown, RefreshDirections::Down);
            break;
        case Applications::ShowMessage:
            currentApplication = std::make_unique<ShowMessage>(this);
            return_app(Applications::Clock, Touch::Gestures::SlideDown, RefreshDirections::Down);
            break;
        case Applications::QMenu:
            currentApplication = std::make_unique<QMenu>(this);
            return_app(Applications::Clock, Touch::Gestures::SlideUp, RefreshDirections::Up);
            break;
        case Applications::Debug:
            currentApplication = std::make_unique<AppDebug>(this);
            return_app(Applications::QMenu, Touch::Gestures::SlideUp, RefreshDirections::Up);
            break;

        default:
            break;
    }
    set_refresh_direction(dir);
    lv_timer_set_period(appUpdate, currentApplication->get_update_interval());
    lv_timer_resume(appUpdate);
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

            case Messages::OnChargingEvent:
                if (currentApp == Applications::ShowMessage) break;
                if (is_charging()) {
                    set_notification("\xEE\xA4\xA1 Power", "Charging...", Smartwatch::MessageType::Info);
                    push_message(Messages::WakeUp);
                    load_application(Applications::ShowMessage, RefreshDirections::Up);
                } else {
                    set_notification("\xEE\xA4\xA1 Power", "Discharging...", Smartwatch::MessageType::Info);
                    push_message(Messages::WakeUp);
                    load_application(Applications::ShowMessage, RefreshDirections::Up);
                }
                push_message(Messages::ReloadIdleTimer);
                break;

            case Messages::OnPowerEvent:
                break;

            case Messages::BleConnected:
                push_message(Messages::BleData);
                set_bluetooth_connected(true);
                break;

            case Messages::BleDisconnected:
                push_message(Messages::BleData);
                set_bluetooth_connected(false);
                break;

            case Messages::OnTouchEvent:
                touch.get();
                gesture = touch.getGesture();
                lvglmodule.set_touch_data(gesture, touch.getEvent(), touch.getX(), touch.getY());
                if (state == States::Idle) {
                    if ( gesture == Touch::Gestures::DoubleTap ) {
                        push_message(Messages::WakeUp);
                    }
                    break;
                } else {
                    push_message(Messages::ReloadIdleTimer);

                    if (currentApp == Applications::Clock && gesture == Touch::Gestures::DoubleTap) {
                        push_message(Messages::GoToSleep);
                        break;
                    }
                }

                if ( gesture == Touch::Gestures::None ) break;
                
                if ( !currentApplication->gestures(gesture) ) {
                    if ( gesture == returnGesture ) {
                        load_application(returnToApp, returnDirection);
                    } else {
                        //lvglmodule.set_touch_data(gesture, touch.getEvent(), touch.getX(), touch.getY());
                    }
                }

                break;

            case Messages::BleData:
                push_message(Messages::WakeUp);
                push_message(Messages::ReloadIdleTimer);
                break;    

            case Messages::OnButtonEvent:
                if (state == States::Idle) {
                    push_message(Messages::WakeUp);
                    break;
                }
                if (state == States::Running) {
                    if (currentApp == Applications::Clock) {
                        push_message(Messages::GoToSleep);
                        break;
                    }
                    load_application(returnToApp, returnDirection);
                    break;
                }
                
                push_message(Messages::ReloadIdleTimer);
                
                break;

            case Messages::ReloadIdleTimer:
                xTimerReset(idleTimer, 0);
                if (backlight.is_dimmed()) {
                    backlight.set_level(backlight.get_saved_level());
                }
                break;
                        
            case Messages::ShowPasskey:
                push_message(Messages::WakeUp);
                load_application(Applications::Passkey, RefreshDirections::Up);
                break;
            
            case Messages::ShowMessage:
                push_message(Messages::WakeUp);
                load_application(Applications::ShowMessage, RefreshDirections::Up);
                break;
        }
    }

}

void Smartwatch::on_idle() {
    if(doNotGoToSleep) return;
    if (backlight.is_dimmed()) {
        backlight.restore_dim();
        push_message(Messages::GoToSleep);
    } else {
        backlight.dim();
        xTimerReset(idleTimer, 0);
    }
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
    backlight.save_level();
    backlight.set_level(0);
    while (backlight.get_level() > 0) {
        vTaskDelay(500);
    }
    display.sleep();
    touch.sleep(true);
    xTimerStop(idleTimer, 0);
    lv_timer_pause(appUpdate);
}

void Smartwatch::wakeup() {
    display.wake_up();
    update_application();
    lv_timer_resume(appUpdate);
    
    lv_timer_handler();
    lv_tick_inc(1);

    touch.sleep(false);
    xTimerStart(idleTimer, 0);
    
    backlight.set_level(backlight.get_saved_level());
}
