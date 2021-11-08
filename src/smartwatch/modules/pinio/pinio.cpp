#include "pinio.h"
#include "pinetime_board.h"
#include "nrf52.h"

/**
 * Constructor
 */
PinIO::PinIO(void) {

}

extern void button_callback(void);

void PinIO::init() {
   pinMode(KEY_ENABLE, OUTPUT);
   digitalWrite(KEY_ENABLE, HIGH);

   pinMode(KEY_ACTION, INPUT_PULLDOWN);
   attachInterrupt(KEY_ACTION, button_callback, ISR_DEFERRED | CHANGE);
}
