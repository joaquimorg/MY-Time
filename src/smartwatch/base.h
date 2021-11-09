
#ifndef BASE_H_
#define BASE_H_

#include <stdio.h>
#include <string.h>

#include "nrf52.h"
#include "bluefruit.h"
#include "pinetime_board.h"

#include "smartwatch.h"
#include "pinio.h"

Smartwatch  smartwatch;
PinIO       pinio;

#define COMMAND_BASE                    0x00
#define COMMAND_TIME_UPDATE            COMMAND_BASE + 0x01
#define COMMAND_NOTIFICATION           COMMAND_BASE + 0x02
#define COMMAND_DELETE_NOTIFICATION    COMMAND_BASE + 0x03
#define COMMAND_SET_ALARMS             COMMAND_BASE + 0x04
#define COMMAND_SET_CALL               COMMAND_BASE + 0x05
#define COMMAND_SET_MUSIC              COMMAND_BASE + 0x06
#define COMMAND_SET_MUSIC_INFO         COMMAND_BASE + 0x07
#define COMMAND_ACTIVITY_STATUS        COMMAND_BASE + 0x08
#define COMMAND_FIND_DEVICE            COMMAND_BASE + 0x09
#define COMMAND_VIBRATION              COMMAND_BASE + 0x0a
#define COMMAND_WEATHER                COMMAND_BASE + 0x0b

#define COMMAND_PT_VERSION             COMMAND_BASE + 0x01
#define COMMAND_PT_BATTERY             COMMAND_BASE + 0x02

#endif // BASE_H_