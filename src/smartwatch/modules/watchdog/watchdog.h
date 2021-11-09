#ifndef WATCHDOG_H
#define WATCHDOG_H

#include "stdint.h"

class Watchdog
{
  public:
    Watchdog(void);
    void init(int timeout);
    void feed(void);

  protected:
    
};

#endif //WATCHDOG_H