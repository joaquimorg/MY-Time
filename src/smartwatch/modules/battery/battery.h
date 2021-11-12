#ifndef BATTERY_H
#define BATTERY_H

#include "stdint.h"
#include "pinetime_board.h"

void battery_init(void);
void battery_read(void);
char * battery_get_icon(void);


#endif //BATTERY_H