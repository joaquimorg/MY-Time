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