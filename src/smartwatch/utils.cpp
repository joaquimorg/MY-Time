#include <stdbool.h>
#include <stdint.h>

#include "utils.h"

const char* actual_reset_reason(void) {
  uint32_t reason = NRF_POWER->RESETREAS;
  NRF_POWER->RESETREAS = 0xffffffff;

  if (reason & 0x01u)
    return "Reset pin";
  if ((reason >> 1u) & 0x01u)
    return "Watchdog";
  if ((reason >> 2u) & 0x01u)
    return "Soft reset";
  if ((reason >> 3u) & 0x01u)
    return "CPU Lock-up";
  if ((reason >> 16u) & 0x01u)
    return "System OFF";
  if ((reason >> 17u) & 0x01u)
    return "LPCOMP";
  if ((reason) &0x01u)
    return "Debug interface";
  if ((reason >> 19u) & 0x01u)
    return "NFC";
  return "Hard reset";
}



// usage:
//   float x, y, z;
//   int i = 0;
//   i += packFloat(&buffer[i], x);
//   i += packFloat(&buffer[i], y);
//   i += packFloat(&buffer[i], z);
int packFloat(void *buf, float x) {
    unsigned char *b = (unsigned char *)buf;
    unsigned char *p = (unsigned char *) &x;
    b[0] = p[3];
    b[1] = p[2];
    b[2] = p[1];
    b[3] = p[0];
    return 4;
}

int packInt(void *buf, int x) {
    unsigned char *b = (unsigned char *)buf;
    unsigned char *p = (unsigned char *) &x;
    b[0] = p[3];
    b[1] = p[2];
    b[2] = p[1];
    b[3] = p[0];
    return 4;
}

int packByte(void *buf, uint8_t x) {
    unsigned char *b = (unsigned char *)buf;
    unsigned char *p = (unsigned char *) &x;
    b[0] = p[0];
    return 1;
}