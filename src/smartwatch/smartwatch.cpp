#include "smartwatch.h"
#include "nrf52.h"
#include "pinetime_board.h"
#include "utils.h"
#include "lvgl.h"
#include "touch.h"
#include "base.h"

#include "Clock.h"
#include "AppDebug.h"
#include "Passkey.h"
#include "ShowMessage.h"
#include "QMenu.h"
#include "Notifications.h"
#include "Steps.h"
#include "HeartRate.h"


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

void Smartwatch::lv_update_app(TimerHandle_t xTimer) {  
    auto sw = static_cast<Smartwatch *>(pvTimerGetTimerID(xTimer));
    sw->update_application();
}

void Smartwatch::hardware_callback(TimerHandle_t xTimer) {
    auto sw = static_cast<Smartwatch *>(pvTimerGetTimerID(xTimer));
    sw->hardware_update();
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

    weather.newData = false;
    weather.hasData = false;

    // Main queue
    msgQueue = xQueueCreate(queueSize, itemSize);

    hardwareTimer = xTimerCreate ("hardwareTimer", ms2tick(5000), pdTRUE, this, Smartwatch::hardware_callback);    

    appUpdateTimer = xTimerCreate ("appUpdateTimer", ms2tick(1000), pdTRUE, this, Smartwatch::lv_update_app);
    //xTimerStart(appUpdateTimer, 0);

    main_scr = lv_scr_act();

    load_application(Applications::Clock, RefreshDirections::None);

    idleTimer = xTimerCreate ("idleTimer", ms2tick(displayTimeout / 2), pdFALSE, this, Smartwatch::idle_callback);
    xTimerStart(idleTimer, 0);
    
    // Create a task for smartwatch
    xTaskCreate(Smartwatch::task, "sw", SW_STACK_SZ, this, TASK_PRIO_NORMAL, &_smartwatchHandle);

    // Create a task for lvgl
    xTaskCreate(Smartwatch::lvgl_task, "lvgl", LVGL_STACK_SZ, this, TASK_PRIO_NORMAL, &_lvglHandle);
        
    xTimerStart(hardwareTimer, 0);

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
    while (1) {
        sw->run_lvgl();
    }
}

void Smartwatch::run_lvgl(void) {

    if ( !stopLvgl ) {
        lv_timer_handler();
        lv_tick_inc(15);
    }
    vTaskDelay(1);
}

void Smartwatch::hardware_update(void) {
    
    battery.read();

    if (digitalRead(CHARGE_IRQ) == LOW) {
        if (!is_charging()) {
            //push_message(Messages::OnChargingEvent);
            set_charging(true);
        }               
    } else {
        set_charging(false);
    }

    if (digitalRead(CHARGE_BASE_IRQ) == LOW) {
        //push_message(Messages::OnPowerEvent);
    }

    send_ble_data();
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
    //if ( currentApp == app ) return;
    if ( app == Applications::None ) return;
    
    stopLvgl = true;
    currentApp = app;
    
    stop_timer(appUpdateTimer);
    currentApplication.reset(nullptr);
    switch (app) {
        case Applications::Clock:
            currentApplication = std::make_unique<Clock>(this);
            return_app(Applications::Clock, Touch::Gestures::None, RefreshDirections::None);
            break;
        case Applications::Notifications:
            currentApplication = std::make_unique<Notifications>(this);
            return_app(Applications::Clock, Touch::Gestures::SlideDown, RefreshDirections::Down);
            break;
        case Applications::Steps:
            currentApplication = std::make_unique<Steps>(this);
            return_app(Applications::Clock, Touch::Gestures::SlideLeft, RefreshDirections::Left);
            break;
        case Applications::HeartRate:
            currentApplication = std::make_unique<HeartRate>(this);
            return_app(Applications::Clock, Touch::Gestures::SlideRight, RefreshDirections::Right);
            break;
        case Applications::Passkey:
            dont_sleep(true);
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
    change_timer(appUpdateTimer, pdMS_TO_TICKS(currentApplication->get_update_interval()));
    start_timer(appUpdateTimer);
    stopLvgl = false;
    dont_sleep(false);
}

void Smartwatch::run(void) {

    Messages msg;
    TickType_t queueTimeout = portMAX_DELAY;
    Touch::Gestures gesture = Touch::Gestures::None;

    if (xQueueReceive(msgQueue, &msg, queueTimeout)) {
        switch (msg) {
            case Messages::WakeUp:
                if (state == States::Running) break;
                state = States::Running;
                wakeup();
                break;

            case Messages::GoToSleep:
                state = States::Idle;
                if (currentApp != Applications::Clock) {
                    load_application(Applications::Clock, RefreshDirections::None);
                }
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
                
                if (state == States::Idle) {
                    if ( gesture == Touch::Gestures::DoubleTap ) {
                        push_message(Messages::WakeUp);
                    }
                    break;
                } else {
                    push_message(Messages::ReloadIdleTimer);

                    if (currentApp == Applications::Clock && gesture == Touch::Gestures::DoubleTap) {
                        if (backlight.is_dimmed()) {
                            backlight.restore_dim();
                        }
                        push_message(Messages::GoToSleep);
                        break;
                    }
                }
                
                if ( gesture == Touch::Gestures::None ) break;
                lvglmodule.set_touch_data(gesture, touch.getEvent(), touch.getX(), touch.getY());
                
                if ( !currentApplication->gestures(gesture) ) {
                    /*if ( gesture == returnGesture ) {
                        load_application(returnToApp, returnDirection);
                    } else {
                        //lvglmodule.set_touch_data(gesture, touch.getEvent(), touch.getX(), touch.getY());
                    }*/
                }
                touch.cleanGesture();
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
                    push_message(Messages::ReloadIdleTimer);

                    if (currentApp == Applications::Clock) {
                        push_message(Messages::GoToSleep);
                        break;
                    }
                    load_application(returnToApp, returnDirection);
                    break;
                }
                
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

            case Messages::NewNotification:
                push_message(Messages::WakeUp);
                if (currentApp == Applications::Notifications) {
                    load_application(Applications::Notifications, RefreshDirections::None);
                } else {
                    load_application(Applications::Notifications, RefreshDirections::Up);
                }
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


void Smartwatch::resume_task( TaskHandle_t xTaskToResume ) {
    BaseType_t xYieldRequired;

    if( xTaskToResume == NULL ) return;
    if (isInISR()) {
        xYieldRequired = xTaskResumeFromISR(xTaskToResume);
        portYIELD_FROM_ISR( xYieldRequired );
    } else {
        vTaskResume(xTaskToResume);
    }
}

void Smartwatch::stop_timer( TimerHandle_t xTimer ) {
   BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if( xTimer == NULL ) return;
    if (isInISR()) {
        xYieldRequired = xTimerStopFromISR( xTimer, &xHigherPriorityTaskWoken )
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    } else {
        xTimerStop(xTimer, 0);
    }
}

void Smartwatch::change_timer( TimerHandle_t xTimer, TickType_t xNewPeriod ) {
   BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if( xTimer == NULL ) return;
    if (isInISR()) {
        xYieldRequired = xTimerChangePeriodFromISR( xTimer, xNewPeriod, &xHigherPriorityTaskWoken )
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    } else {
        xTimerChangePeriod(xTimer, xNewPeriod, 0);
    }
}

void Smartwatch::start_timer( TimerHandle_t xTimer ) {
   BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if( xTimer == NULL ) return;
    if (isInISR()) {
        xYieldRequired = xTimerStartFromISR( xTimer, &xHigherPriorityTaskWoken )
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    } else {
        xTimerStart(xTimer, 0);
    }
}

void Smartwatch::sleep() {
    backlight.save_level();
    backlight.set_level(0);
    while (backlight.get_level() > 0) {
        vTaskDelay(500);
    }
    display.sleep();
    //touch.sleep(true);
    stop_timer(appUpdateTimer);
    vTaskSuspend( _lvglHandle );
    stop_timer(idleTimer);
    change_timer(hardwareTimer, pdMS_TO_TICKS(60000));
}

void Smartwatch::wakeup() {
    display.wake_up();
    
    start_timer(appUpdateTimer);
    update_application();
    lv_timer_handler();
    lv_tick_inc(1);

    //touch.sleep(false);
    start_timer(idleTimer);
    change_timer(hardwareTimer, pdMS_TO_TICKS(5000));
    resume_task( _lvglHandle );
    backlight.set_level(backlight.get_saved_level());
}
