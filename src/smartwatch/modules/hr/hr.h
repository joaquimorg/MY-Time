#ifndef __HR_H
#define __HR_H

#include "stdint.h"
#include "nrf52.h"

class HR
{  
    public:
        HR(void);
        void init(void);
        void beginHR(void);
        void endHR(void);
        byte getHR(void);
        void getHRms(void);
        byte getLastHR(void) { return last_heartrate; }

    protected:
        bool heartrate_enable = false;
        byte last_heartrate_ms;
        byte last_heartrate = 0;

};

#endif //__HR_H