#include "smartwatch.h"
#include "nrf52.h"
#include "pinetime_board.h"
#include "utils.h"
#include "lvgl.h"

#define SW_STACK_SZ       (256*8)

extern "C" {
    static void event_handler(lv_event_t * e)
    {
        lv_event_code_t code = lv_event_get_code(e);

        if(code == LV_EVENT_CLICKED) {
            
        }
        
    }
}

/**
 * Constructor
 */
Smartwatch::Smartwatch(void) {
    displayTimeout = 60000;
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

    /*lv_obj_t * spinner = lv_spinner_create(main_scr, 1000, 60);
    lv_obj_set_size(spinner, 220, 220);
    lv_obj_center(spinner);*/

    label1 = lv_label_create(main_scr);
    lv_label_set_text_fmt(label1, "MY-Time - Ready to rock - %s", resetReason);
    lv_obj_set_style_text_color(label1, lv_color_make(0xff, 0xff, 0xff), 0);
    lv_obj_set_width(label1, 220);
    lv_label_set_long_mode(label1, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_obj_set_style_text_align(label1, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(label1, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t * label;

    lv_obj_t * btn1 = lv_btn_create(lv_scr_act());
    lv_obj_add_event_cb(btn1, event_handler, LV_EVENT_ALL, NULL);
    lv_obj_align(btn1, LV_ALIGN_CENTER, 0, -60);

    label = lv_label_create(btn1);
    lv_label_set_text(label, "Button");
    lv_obj_center(label);
    

    idleTimer = xTimerCreate ("idleTimer", ms2tick(displayTimeout), pdFALSE, this, Smartwatch::idle_callback);
    xTimerStart(idleTimer, 0);
    
    // Create a task for smartwatch
    xTaskCreate(Smartwatch::task, "sw", SW_STACK_SZ, this, TASK_PRIO_NORMAL, &_smartwatchHandle);
}

void Smartwatch::task(void *instance) {
    auto *sw = static_cast<Smartwatch *>(instance);
    xTaskNotifyGive(xTaskGetCurrentTaskHandle());    
    while (1) {
        sw->run();
    }
}


void Smartwatch::hardware_update(void) {
    if ( state == States::Idle ) {
        vTaskDelay(ms2tick(30000));
    } else {
        vTaskDelay(ms2tick(500));
    }    

}

void Smartwatch::run(void) {

    Messages msg;

    TickType_t queueTimeout;
    switch (state) {
        case States::Idle:
            queueTimeout = portMAX_DELAY;
            break;
        case States::Running:
            queueTimeout = 5;
            lv_timer_handler();
            lv_tick_inc(20);
            break;
        default:
            queueTimeout = portMAX_DELAY;
            break;
    }

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
                //touch.read();

                gesture = touch.getGesture();
                if (state == States::Idle) {
                    if ( gesture == Touch::Gestures::DoubleTap ) {
                        push_message(Messages::WakeUp);
                    }
                    break;
                }

                push_message(Messages::ReloadIdleTimer);

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

    //feed_watchdog();

}

void Smartwatch::on_idle() {
    if(doNotGoToSleep) return;
    push_message(Messages::GoToSleep);
}


void Smartwatch::push_message(Smartwatch::Messages msg) {
    if( msgQueue == NULL ) return;

    if (isInISR()) {
        if (xQueueIsQueueFullFromISR(msgQueue)) {
            return;
        }
        BaseType_t xHigherPriorityTaskWoken;
        xHigherPriorityTaskWoken = pdFALSE;
        xQueueSendFromISR(msgQueue, &msg, &xHigherPriorityTaskWoken);
        if (xHigherPriorityTaskWoken) {
            /* Actual macro used here is port specific. */
            //portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

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
