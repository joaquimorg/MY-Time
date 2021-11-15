#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include <stdint.h>
#include "nrf52.h"

const char* actual_reset_reason(void);


// usage:
//   float x, y, z;
//   int i = 0;
//   i += packFloat(&buffer[i], x);
//   i += packFloat(&buffer[i], y);
//   i += packFloat(&buffer[i], z);
int packFloat(void *buf, float x);

int packInt(void *buf, int x);

int packByte(void *buf, uint8_t x);

#endif //UTILS_H
