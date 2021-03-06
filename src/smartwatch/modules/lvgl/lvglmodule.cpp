#include "lvglmodule.h"
#include "nrf52.h"
#include "lvgl.h"
#include "lv_theme_pinetime.h"
#include "fast_spi.h"
#include "I2S.h"

/**
 * Constructor
 */
LvglModule::LvglModule(Display& display, Touch& touch) : display{ display }, touch{ touch } {

}

void LvglModule::set_refresh_direction(refreshDirections direction) {
    this->scrollDirection = direction;
}

void LvglModule::disp_flush(lv_disp_drv_t* disp_drv, const lv_area_t* area, lv_color_t* color_p) {
    auto* lvgl = static_cast<LvglModule*>(disp_drv->user_data);
    lvgl->flush_display(area, color_p);
}

void LvglModule::touchpad_read(lv_indev_drv_t* indev_drv, lv_indev_data_t* data) {
    auto* lvgl = static_cast<LvglModule*>(indev_drv->user_data);
    lvgl->touchpad(data);
}

void LvglModule::init(void) {


    writeOffset = 0;
    scrollOffset = 0;

    lv_init();

    lv_disp_draw_buf_init(&draw_buf_dsc, buf_2_1, NULL, displayHOR * bufferSize);   // Initialize the display buffer

    lv_disp_drv_init(&disp_drv);    // Basic initialization

    // Set the resolution of the display
    disp_drv.hor_res = displayHOR;
    disp_drv.ver_res = displayVER;

    // Used to copy the buffer's content to the display
    disp_drv.flush_cb = LvglModule::disp_flush;
    disp_drv.user_data = this;

    // Set a display buffer
    disp_drv.draw_buf = &draw_buf_dsc;

    // Finally register the driver
    lv_disp_drv_register(&disp_drv);

    lv_theme_t* th = lv_theme_pinetime_init();

    lv_disp_set_theme(lv_disp_get_default(), th);    

    // Register a touchpad input device
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = LvglModule::touchpad_read;
    indev_drv.user_data = this;
    lv_indev_drv_register(&indev_drv);

}

void LvglModule::touchpad(lv_indev_data_t* data) {

    // Get the touchpad's position    
    touch.get();

    //data->point.x = touch.getX();
    //data->point.y = touch.getY();

    //touch_event = touch.getEvent();

    //touch_gesture = touch.getGesture();

    set_touch_data(touch.getGesture(), touch.getEvent(), touch.getX(), touch.getY());

    data->point.x = touch_xpos;
    data->point.y = touch_ypos;

    if (touch_event == 2) {
        data->state = LV_INDEV_STATE_PR;        
        //touch_gesture = Touch::Gestures::None;
    } else {
        data->state = LV_INDEV_STATE_REL;
        //data->point.x = 0;
        //data->point.y = 0;
    }
    //touch_gesture = Touch::Gestures::None;
}

uint16_t convertColor(uint8_t r, uint8_t g, uint8_t b) {
    return ((r >> 3) << 11 | (g >> 2) << 5 | (b >> 3));
}

void LvglModule::flush_display(const lv_area_t* area, lv_color_t* color_p) {

    
    uint16_t y1, y2, width, height = 0;    

    display.start_write_display();

    if ((this->scrollDirection == refreshDirections::Down) && (area->y2 == visibleNbLines - 1)) {
        writeOffset = ((writeOffset + totalNbLines) - visibleNbLines) % totalNbLines;
    }
    else if ((this->scrollDirection == refreshDirections::Up) && (area->y1 == 0)) {
        writeOffset = (writeOffset + visibleNbLines) % totalNbLines;
    }

    y1 = (area->y1 + writeOffset) % totalNbLines;
    y2 = (area->y2 + writeOffset) % totalNbLines;

    width = (area->x2 - area->x1) + 1;
    height = (area->y2 - area->y1) + 1;

    if (this->scrollDirection == refreshDirections::Down) {
        if (area->y2 < visibleNbLines - 1) {
            uint16_t toScroll = 0;
            if (area->y1 == 0) {
                toScroll = height * 2;
                this->scrollDirection = refreshDirections::None;
                lv_disp_set_direction(lv_disp_get_default(), 0);
            }
            else {
                toScroll = height;
            }

            if (scrollOffset >= toScroll)
                scrollOffset -= toScroll;
            else {
                toScroll -= scrollOffset;
                scrollOffset = (totalNbLines)-toScroll;
            }            
            display.vertical_scroll_definition(0, totalNbLines, 0, scrollOffset);            

        }
    }
    else if (this->scrollDirection == refreshDirections::Up) {

        if (area->y1 > 0) {
            if (area->y2 == visibleNbLines - 1) {
                scrollOffset += (height * 2);
                this->scrollDirection = refreshDirections::None;
                lv_disp_set_direction(lv_disp_get_default(), 0);
            }
            else {
                scrollOffset += height;
            }
            scrollOffset = scrollOffset % totalNbLines;            
            display.vertical_scroll_definition(0, totalNbLines, 0, scrollOffset);
        }
    }
    else if (this->scrollDirection == refreshDirections::Left) {
        if (area->x2 == visibleNbLines - 1) {
            this->scrollDirection = refreshDirections::None;
            lv_disp_set_direction(lv_disp_get_default(), 0);
        }
    }
    else if (this->scrollDirection == refreshDirections::Right) {
        if (area->x1 == 0) {
            this->scrollDirection = refreshDirections::None;
            lv_disp_set_direction(lv_disp_get_default(), 0);
        }
    }

    display.end_write_display();

    if (y2 < y1) {
        height = totalNbLines - y1;

        if (height > 0) {
            //display.set_addr_display(area->x1, y1, width, height);            
            //write_fast_spi(reinterpret_cast<const uint8_t*>(color_p), (width * height * 2));
            display.drawBitmap_I2S_wh(area->x1, y1, width, height, reinterpret_cast<uint8_t*>(color_p));

        }

        uint16_t pixOffset = width * height;

        height = y2 + 1;        
        //display.set_addr_display(area->x1, 0, width, height);
        //write_fast_spi(reinterpret_cast<const uint8_t*>(color_p + pixOffset), (width * height * 2));
        display.drawBitmap_I2S_wh(area->x1, 0, width, height, reinterpret_cast<uint8_t*>(color_p + pixOffset));

    }
    else {        
        //display.set_addr_display(area->x1, y1, width, height);
        //write_fast_spi(reinterpret_cast<const uint8_t*>(color_p), (width * height * 2));
        display.drawBitmap_I2S_wh(area->x1, y1, width, height, reinterpret_cast<uint8_t*>(color_p));
    }    
    
    
    
    //display.drawBitmap_I2S(area->x1, area->y1, area->x2, area->y2, reinterpret_cast<uint8_t*>(color_p));

    lv_disp_flush_ready(&disp_drv);
}