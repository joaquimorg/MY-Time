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
#include "AppBacklight.h"
#include "Notifications.h"
#include "Steps.h"
//#include "HeartRate.h"


#define SW_STACK_SZ       (256*4)
#define LVGL_STACK_SZ     (256*2)

/**
 * Constructor
 */
Smartwatch::Smartwatch(void) {
    displayTimeout = 20000;
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

void Smartwatch::gesture_event_cb(lv_event_t * e) {
    auto sw = static_cast<Smartwatch *>(lv_event_get_user_data(e));

    lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_get_act());

    switch (dir) {
        case LV_DIR_LEFT:
            sw->gesture(Touch::Gestures::SlideLeft);    
            break;
        case LV_DIR_RIGHT:
            sw->gesture(Touch::Gestures::SlideRight);
            break;
        case LV_DIR_TOP:
            sw->gesture(Touch::Gestures::SlideUp);
            break;
        case LV_DIR_BOTTOM:
            sw->gesture(Touch::Gestures::SlideDown);
            break;
        default:
            break;
    }

}

void Smartwatch::gesture( Touch::Gestures gesture ) {
    /*if ( !currentApplication->gestures(gesture) ) {

    }*/
}

void Smartwatch::init(void) {

    resetReason = actual_reset_reason();

    // Initialize the modules
    rtc_time.init();    

    touch.init();        
    //heartRate.init();
    display.init();
    lvglmodule.init();
    backlight.init();
    backlight.set_value(44);
    vibration.init();
    
    //stepCount.initialize();

    set_charging(false);

    weather.newData = false;
    weather.hasData = false;

    // Main queue
    msgQueue = xQueueCreate(queueSize, itemSize);

    hardwareTimer.begin(ms2tick(5000), Smartwatch::hardware_callback, this, pdTRUE);    

    appUpdateTimer.begin(ms2tick(1000), Smartwatch::lv_update_app, this, pdTRUE);

    main_scr = lv_scr_act();
    //lv_obj_add_event_cb(main_scr, gesture_event_cb, LV_EVENT_GESTURE, this);

    load_application(Applications::Clock, RefreshDirections::None);

    idleTimer.begin(ms2tick(displayTimeout / 2), Smartwatch::idle_callback, this, pdFALSE);
    idleTimer.start();
    
    // Create a task for smartwatch
    xTaskCreate(Smartwatch::task, "sw", SW_STACK_SZ, this, TASK_PRIO_NORMAL, &_smartwatchHandle);

    // Create a task for lvgl
    xTaskCreate(Smartwatch::lvgl_task, "lvgl", LVGL_STACK_SZ, this, TASK_PRIO_NORMAL, &_lvglHandle);
        
    hardwareTimer.start();    

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
        if ( sw->runLvgl ) {
            sw->run_lvgl();
        } else {
            vTaskDelay(pdMS_TO_TICKS(500));
        }
    }
}

void Smartwatch::run_lvgl(void) {

    if ( !stopLvgl ) {
        lv_timer_handler();
        lv_tick_inc(10);
        vTaskDelay(1);
    } else {
        //vTaskDelay(pdMS_TO_TICKS(500));
        vTaskDelay(1);
    }
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
    if (lvglmodule.get_direction() != LvglModule::refreshDirections::None) {
        return;
    }
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
    set_refresh_direction(dir);
    appUpdateTimer.stop();
    currentApplication.reset(nullptr);
    dont_sleep(false);
    //heartRate.endHR();
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
        /*case Applications::HeartRate:
            currentApplication = std::make_unique<HeartRate>(this);
            return_app(Applications::Clock, Touch::Gestures::SlideRight, RefreshDirections::Right);
            break;*/
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
        case Applications::Backlight:
            currentApplication = std::make_unique<AppBacklight>(this);
            return_app(Applications::QMenu, Touch::Gestures::SlideUp, RefreshDirections::Up);
            break;
        case Applications::Debug:
            currentApplication = std::make_unique<AppDebug>(this);
            return_app(Applications::QMenu, Touch::Gestures::SlideUp, RefreshDirections::Up);
            break;

        default:
            break;
    }
    appUpdateTimer.setPeriod(pdMS_TO_TICKS(currentApplication->get_update_interval()));
    appUpdateTimer.start();
    stopLvgl = false;    
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
                sleep();
                if (currentApp != Applications::Clock) {
                    load_application(Applications::Clock, RefreshDirections::None);
                }                
                state = States::Idle;
                runLvgl = false;
                break;

            case Messages::OnChargingEvent:
                if(doNotGoToSleep) return;
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
                set_notification("\n\xEE\xA4\x83 Bluetooth", "\n\nPhone connected !", Smartwatch::MessageType::Info);
                push_message(Messages::WakeUp);
                load_application(Applications::ShowMessage, RefreshDirections::Up);
                ble_send_version();
                break;

            case Messages::BleDisconnected:
                push_message(Messages::BleData);
                set_bluetooth_connected(false);

                set_notification("\n\xEE\xA4\x83 Bluetooth", "\n\nPhone disconnected !", Smartwatch::MessageType::Info);
                push_message(Messages::WakeUp);
                load_application(Applications::ShowMessage, RefreshDirections::Up);
                break;

            case Messages::OnTouchEvent:
                if (state != States::Idle) {
                    push_message(Messages::ReloadIdleTimer);
                }
               
                /*
                //touch.get();
                //gesture = touch.readGesture();
                //gesture = touch.getGesture();
                //lvglmodule.set_touch_data(gesture, touch.getEvent(), touch.getX(), touch.getY());
                //touch.cleanGesture();        
                //gesture = lvglmodule.get_touch_gesture();  
                //lvglmodule.set_touch_gesture(Touch::Gestures::None);      
                //if ( gesture == Touch::Gestures::None ) break;

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
                */
                gesture = touch.readGesture();
                if ( gesture == Touch::Gestures::None ) break;
                
                if ( !currentApplication->gestures(gesture) ) {
                    /*if ( gesture == returnGesture ) {
                        load_application(returnToApp, returnDirection);
                    } else {
                        //lvglmodule.set_touch_data(gesture, touch.getEvent(), touch.getX(), touch.getY());
                    }*/
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
                    //push_message(Messages::ReloadIdleTimer);
                    idleTimer.reset();
                    if (currentApp == Applications::Clock) {                        
                        push_message(Messages::GoToSleep);
                        break;
                    }
                    push_message(Messages::ReloadIdleTimer);
                    load_application(returnToApp, returnDirection);
                    break;
                }
                
                break;

            case Messages::ReloadIdleTimer:
                idleTimer.reset();
                if (backlight.is_dimmed()) {
                    backlight.set_value(backlight.get_save_value());
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
                if (currentApp == Applications::Notifications) {
                    load_application(Applications::Notifications, RefreshDirections::Left);
                } else {
                    if (state == States::Idle) {
                        load_application(Applications::Notifications, RefreshDirections::Up);
                    }
                    vibration.vibrate(128, 50);
                }
                push_message(Messages::WakeUp);
                push_message(Messages::ReloadIdleTimer);
                break;
        }
    }

}

void Smartwatch::on_idle() {
    if(doNotGoToSleep) return;
    if (backlight.is_dimmed()) {        
        push_message(Messages::GoToSleep);
    } else {
        backlight.dim();
        idleTimer.reset();
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

void Smartwatch::sleep() {    
    backlight.save_value();
    backlight.set_value(0);
    while (backlight.get_value() > 0) {
        vTaskDelay(500);
    }
    
    display.sleep();

    //touch.sleep(true);
    //appUpdateTimer.stop();    
    idleTimer.stop();
    hardwareTimer.setPeriod(pdMS_TO_TICKS(60000));
    //vTaskSuspend( _lvglHandle );

}

void Smartwatch::wakeup() {    
    //resume_task( _lvglHandle );        
    //update_application();
    //lv_timer_handler();
    //lv_tick_inc(1);

    //touch.sleep(false);
    idleTimer.start();
    hardwareTimer.setPeriod(pdMS_TO_TICKS(5000));
    runLvgl = true;
    display.wake_up();
    //appUpdateTimer.start();
    backlight.set_value(backlight.get_save_value());
}
