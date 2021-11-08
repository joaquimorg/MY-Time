#include "smartwatch.h"
#include "nrf52.h"
#include "pinetime_board.h"
#include "lvgl.h"

#define SW_STACK_SZ       (256*10)
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
    lvglmodule.init(display);
    backlight.init();
    backlight.set_level(2);    

    // Main queue
    msgQueue = xQueueCreate(queueSize, itemSize);

    main_scr = lv_scr_act();

    lv_obj_t * spinner = lv_spinner_create(main_scr, 1000, 60);
    lv_obj_set_size(spinner, 220, 220);
    lv_obj_center(spinner);

    lv_obj_t * label1 = lv_label_create(main_scr);
    lv_label_set_text(label1, "MY-Time\nReady to rock");
    lv_obj_set_style_text_color(label1, lv_color_make(0xff, 0xff, 0xff), 0);
    lv_obj_set_style_text_align(label1, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(label1, LV_ALIGN_CENTER, 0, 0);

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
            lv_tick_inc(15);
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
                break;

            case Messages::BleDisconnected:
                break;

            case Messages::OnTouchEvent:
                push_message(Messages::ReloadIdleTimer);
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
