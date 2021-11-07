#include "smartwatch.h"
#include "nrf52.h"
#include "pinetime_board.h"
#include "lvgl.h"

#define SW_STACK_SZ       (256*5)
/**
 * Constructor
 */
Smartwatch::Smartwatch(void) {

}

void Smartwatch::init(void) {

    // Initialize the modules
    display.init();
    lvglmodule.init(display);
    backlight.init();
    backlight.set_level(1);

    main_scr = lv_scr_act();

    lv_obj_t * spinner = lv_spinner_create(main_scr, 1000, 60);
    lv_obj_set_size(spinner, 200, 200);
    lv_obj_center(spinner);

    lv_obj_t * label1 = lv_label_create(main_scr);
    lv_label_set_text(label1, "MY-Time\nReady to rock");
    lv_obj_set_style_text_color(label1, lv_color_make(0xff, 0xff, 0xff), 0);
    lv_obj_set_style_text_align(label1, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(label1, LV_ALIGN_CENTER, 0, 0);

    // Create a task for smartwatch
    xTaskCreate(Smartwatch::task, "sw", SW_STACK_SZ, this, TASK_PRIO_LOW, &_smartwatchHandle);
}

void Smartwatch::task(void *instance) {
    auto *sw = static_cast<Smartwatch *>(instance);

    while (1) {
        sw->run();
        //yield();
    }
}

void Smartwatch::run(void) {
    //delay(500);
    //digitalToggle(LCD_LIGHT_3);
    vTaskDelay(1);
    lv_timer_handler();
    lv_tick_inc(5);
}