#ifndef WATCHDOG_H
#define WATCHDOG_H

#include "stdint.h"


void watchdog_init(int timeout);
void watchdog_feed(void);

#endif //WATCHDOG_H