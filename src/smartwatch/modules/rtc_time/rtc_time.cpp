#include "rtc_time.h"
#include "nrf52.h"



/* Variable Declarations for variables in noinit SRAM 
   Increment NoInit_MagicValue upon adding variables to this area
*/
extern uint32_t __start_noinit_data;
extern uint32_t __stop_noinit_data;
static constexpr uint32_t NoInit_MagicValue = 0xDEAD0000;
uint32_t NoInit_MagicWord __attribute__((section(".noinit")));
uint32_t NoInit_BackUpTime __attribute__((section(".noinit")));


/*
*  RTC0 is used by Softdevice for bluetooth handling
*  RTC1 is used by the Arduino core for delay()
*/
#define RTC NRF_RTC2
#define RTC_IRQ RTC2_IRQn

#define DAY 86400UL  // 24 hours * 60 minutes * 60 seconds
#define BEGYEAR 2000 // UTC started at 00:00:00 January 1, 2000
#define is_leap_year(yr) (!((yr) % 400) || (((yr) % 100) && !((yr) % 4)))
#define year_length(yr) (is_leap_year(yr) ? 366 : 365)

volatile uint32_t gTimestamp = 1609459200 - 946684800;

uint8_t month_length(uint8_t lpyr, uint8_t mon) {
    uint8_t days = 31;

    if (mon == 1) {// feb 2
        days = (28 + lpyr);
    } else {
        if (mon > 6) {// aug-dec 8-12
            mon--;
        }

        if (mon & 1) {
            days = 30;
        }
    }

    return (days);
}

uint8_t get_week(uint16_t year, uint8_t month, uint8_t day) {
    const uint8_t table_week[12] = { 0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5 };
    uint16_t temp2;
    uint8_t yearH, yearL;
    uint8_t week;
    yearH = year / 100;
    yearL = year % 100;
    month = month % 13;

    if (yearH > 19)
        yearL += 100;

    temp2 = yearL + yearL / 4;

    temp2 = temp2 % 7;

    temp2 = temp2 + day + table_week[month - 1];

    if (yearL % 4 == 0 && month < 3)
        temp2--;

    if ((temp2 % 7) == 0) {
        temp2 = 7;
        week = temp2;
    } else {
        week = (temp2 % 7);
    }

    week--;
    return week;
}

/**
 * Constructor
 */
RTCTime::RTCTime(void) {

    if (NoInit_MagicWord == NoInit_MagicValue) {
        set_time(NoInit_BackUpTime);
    } else {
        //Clear Memory to known state
        memset(&__start_noinit_data,0,(uintptr_t)&__stop_noinit_data-(uintptr_t)&__start_noinit_data);
        NoInit_MagicWord = NoInit_MagicValue;
    }

}

/**
* Reset events and read back on nRF52
* http://infocenter.nordicsemi.com/pdf/nRF52_Series_Migration_v1.0.pdf
*/
#if __CORTEX_M == 0x04
#define NRF5_RESET_EVENT(event)                                                 \
event = 0;                                                                   \
(void)event
#else
#define NRF5_RESET_EVENT(event) event = 0
#endif

extern "C" {

  void RTC2_IRQHandler(void) {
    NRF5_RESET_EVENT(RTC->EVENTS_COMPARE[0]);
    gTimestamp++;
    NoInit_BackUpTime = gTimestamp;
    RTC->TASKS_CLEAR = 1;
  }
}

void RTCTime::init(void) {
    // Configure RTC
    RTC->TASKS_STOP = 1;
    RTC->PRESCALER = 31; //1024Hz frequency
    RTC->CC[0] = RTC->COUNTER + (1 * 1024);
    RTC->EVTENSET = RTC_EVTENSET_COMPARE0_Msk;
    RTC->INTENSET = RTC_INTENSET_COMPARE0_Msk;
    RTC->TASKS_START = 1;
    RTC->EVENTS_COMPARE[0] = 0;
    // Enable interrupt
    NVIC_SetPriority(RTC_IRQ, 15);
    NVIC_ClearPendingIRQ(RTC_IRQ);
    NVIC_EnableIRQ(RTC_IRQ);
}


void RTCTime::set_time(uint32_t timestampNow) {
    gTimestamp = timestampNow;
}

uint32_t RTCTime::get_timestamp(void) {
    return gTimestamp;
}

void RTCTime::get_time(void) {

    uint32_t secTime = gTimestamp;
    // calculate the time less than a day - hours, minutes, seconds
    {
        unsigned int day = secTime % DAY;
        t_seconds = day % 60UL;
        t_minutes = (day % 3600UL) / 60UL;
        t_hour = day / 3600UL;
    }

    // Fill in the calendar - day, month, year
    {
        unsigned short numDays = secTime / DAY;
        t_year = BEGYEAR;
        while (numDays >= year_length(t_year))
        {
            numDays -= year_length(t_year);
            t_year++;
        }

        t_month = 0;
        while (numDays >= month_length(is_leap_year(t_year), t_month))
        {
            numDays -= month_length(is_leap_year(t_year), t_month);
            t_month++;
        }

        t_day = numDays;
    }

    t_day++;
    t_month++;

    t_week = get_week(t_year, t_month, t_day);
}


const char *RTCTime::get_months_low(void) {

    char const *MonthsLow[] = {
        "",
        "January",
        "February",
        "March",
        "April",
        "May",
        "June",
        "July",
        "August",
        "September",
        "October",
        "November",
        "December"
    };

    return MonthsLow[t_month];

}

const char *RTCTime::get_days_low(void) {
    char const *DaysStringLow[] = {
        "Monday",
        "Tuesday",
        "Wednesday",
        "Thursday",
        "Friday",
        "Saturday",
        "Sunday"
    };
    return DaysStringLow[t_week];
}

const char *RTCTime::get_days_low_short(void) {
    char const *DaysStringLow[] = {
        "Mon",
        "Tue",
        "Wed",
        "Thu",
        "Fri",
        "Sat",
        "Sun"
    };
    return DaysStringLow[t_week];
}

const char *RTCTime::get_months(void) {
    char const *MonthsString[] = {
        "",
        "JAN",
        "FEB",
        "MAR",
        "APR",
        "MAY",
        "JUN",
        "JUL",
        "AUG",
        "SEP",
        "OCT",
        "NOV",
        "DEC"
    };
    return MonthsString[t_month];
}

const char *RTCTime::get_days(void) {
    char const *DaysString[] = {
        "MONDAY",
        "TUESDAY",
        "WEDNESDAY",
        "THURSDAY",
        "FRIDAY",
        "SATURDAY",
        "SUNDAY"
    };
    return DaysString[t_week];
}