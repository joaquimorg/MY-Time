#ifndef RTC_TIME_H
#define RTC_TIME_H

#include "stdint.h"
#include "stdbool.h"

class RTCTime
{
  protected:
    uint16_t    t_year;     // 2000+
    uint8_t     t_month;    // 1-12
    uint8_t     t_day;      // 1-31
    uint8_t     t_hour;     // 0-23
    uint8_t     t_minutes;  // 0-59
    uint8_t     t_seconds;  // 0-59
    uint8_t     t_week;     // 0-6

  public:

    RTCTime(void);
    void init(void);

    void set_time(uint32_t timestampNow);
    void get_time(void);
    uint32_t get_timestamp(void);

    uint16_t year() const { return t_year; }
    uint8_t day() const { return t_day; }
    uint8_t week() const { return t_week; }
    uint8_t month() const { return t_month; }
    uint8_t hours() const { return t_hour; }
    uint8_t minutes() const { return t_minutes; }
    uint8_t seconds() const { return t_seconds; }

    const char *get_months_low(void);
    const char *get_days_low(void);
    const char *get_days_low_short(void);

    const char *get_months(void);
    const char *get_days(void);

    uint8_t diff_minutes(uint32_t timestamp) {
        uint32_t timestampNow = get_timestamp();
        uint32_t diff = timestampNow - timestamp;
        return (uint8_t)(diff / 60);
    }

};

#endif //RTC_TIME_H