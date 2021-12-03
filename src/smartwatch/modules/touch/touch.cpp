#include "Touch.h"
#include "pinetime_board.h"
#include "nrf52.h"
#include "i2c.h"

/**
 * Constructor
 */
Touch::Touch(void) {

}

void Touch::init(void) {

    pinMode(TP_RST, OUTPUT);

    digitalWrite(TP_RST, HIGH);
    delay_ns(50);
    digitalWrite(TP_RST, LOW);
    delay_ns(5);
    digitalWrite(TP_RST, HIGH);
    delay_ns(50);

    uint8_t dummy;
    user_i2c_read(TP_TWI_ADDR, 0x15, &dummy, 1);
    delay_ns(5);
    user_i2c_read(TP_TWI_ADDR, 0xA7, &dummy, 1);
    delay_ns(5);

    /*
    [2] EnConLR - Continuous operation can slide around
    [1] EnConUD - Slide up and down to enable continuous operation
    [0] EnDClick - Enable Double-click action
    */
    const uint8_t motionMask = 0b00000101;
    user_i2c_write(TP_TWI_ADDR, 0xEC, &motionMask, 1);
    delay_ns(15);
    /*
    [7] EnTest interrupt pin test, and automatically send out low pulse periodically after being enabled.
    [6] When EnTouch detects a touch, it periodically sends out low pulses.
    [5] When EnChange detects a touch state change, it sends out a low pulse.
    [4] When EnMotion detects a gesture, it sends out a low pulse.
    [0] OnceWLP Long press gesture only sends out a low pulse signal.
    */
    const uint8_t irqCtl = 0b00111001;
    user_i2c_write(TP_TWI_ADDR, 0xFA, &irqCtl, 1);
    delay_ns(15);
}


void Touch::sleep(bool state) {
    digitalWrite(TP_RST, LOW);
    delay_ns(5);
    digitalWrite(TP_RST, HIGH);
    delay_ns(50);
    if (state) {
        byte standby_value = 0x03;
        user_i2c_write(TP_TWI_ADDR, 0xA5, &standby_value, 1);
    }
}

void Touch::read(void) {
    user_i2c_read(TP_TWI_ADDR, 0x01, data_raw, 6);
}


void Touch::get(void) {

    user_i2c_read(TP_TWI_ADDR, 0x01, data_raw, 10);

    if (data_raw[0] == 0xe0) {
        gesture = Touch::Gestures::None;
        return;
    }

    gesture = static_cast<Gestures>(data_raw[0]);
    touchpoints = data_raw[1];
    event = data_raw[2] >> 6;
    xpos = data_raw[3];
    ypos = data_raw[5];
    /*if (xpos == 255 && ypos == 255) {
        xpos = last_xpos;
        ypos = last_ypos;
    }
    else {
        last_xpos = xpos;
        last_ypos = ypos;
    }*/
}