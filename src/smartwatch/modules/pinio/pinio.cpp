#include "pinio.h"
#include "pinetime_board.h"
#include "nrf52.h"
#include "fast_spi.h"
#include "i2c.h"

/**
 * Constructor
 */
PinIO::PinIO(void) {

}

extern void button_callback(void);
extern void tp_callback(void);

void PinIO::init() {
   pinMode(KEY_ENABLE, OUTPUT);
   digitalWrite(KEY_ENABLE, HIGH);

   pinMode(KEY_ACTION, INPUT_SENSE_HIGH);
   attachInterrupt(KEY_ACTION, button_callback, ISR_DEFERRED | CHANGE);
   
   pinMode(TP_IRQ, INPUT_SENSE_LOW);
   attachInterrupt(TP_IRQ, tp_callback, ISR_DEFERRED | CHANGE);

   init_fast_spi();
   init_i2c();
}
