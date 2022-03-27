#include "touch.h"
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
    delay_ns(5);
    digitalWrite(TP_RST, LOW);
    delay_ns(5);
    digitalWrite(TP_RST, HIGH);
    delay_ns(5);

    user_i2c_read(TP_TWI_ADDR, 0x15, &version15, 1);
    delay_ns(5);
    user_i2c_read(TP_TWI_ADDR, 0xA7, versionInfo, 3);
    delay_ns(5);

    /*
    [2] EnConLR - Continuous operation can slide around
    [1] EnConUD - Slide up and down to enable continuous operation
    [0] EnDClick - Enable Double-click action
    */
    const uint8_t motionMask = 0b00000001;
    user_i2c_write(TP_TWI_ADDR, 0xEC, &motionMask, 1);
    delay_ns(15);
    /*
    [7] EnTest interrupt pin test, and automatically send out low pulse periodically after being enabled.
    [6] When EnTouch detects a touch, it periodically sends out low pulses.
    [5] When EnChange detects a touch state change, it sends out a low pulse.
    [4] When EnMotion detects a gesture, it sends out a low pulse.
    [0] OnceWLP Long press gesture only sends out a low pulse signal.
    */
    //const uint8_t irqCtl = 0b00000001;
    //user_i2c_write(TP_TWI_ADDR, 0xFA, &irqCtl, 1);
    //delay_ns(15);

    //user_i2c_read(TP_TWI_ADDR, 0xEC, &version15, 1);
    //delay_ns(5);
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
    user_i2c_read(TP_TWI_ADDR, 0x02, data_raw, 6);
}


void Touch::get(void) {

    byte raw[8];

    user_i2c_read(TP_TWI_ADDR, 0x01, raw, 6);

    gesture = static_cast<Gestures>(raw[0]);

    touchpoints = raw[1];

    event = raw[2] >> 6;
    xpos = raw[3];
    ypos = raw[5];

}


Touch::Gestures Touch::readGesture(void) {
    
    byte raw[1];

    user_i2c_read(TP_TWI_ADDR, 0x01, raw, 1);
    gesture = static_cast<Gestures>(raw[0]);
    return gesture;
}
