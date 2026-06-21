#ifndef BSP_RTC_H
#define BSP_RTC_H

#include "bsp_core.h"

#define TIME_ZONE_OFFSET_UTC 0
#define TIME_ZONE_OFFSET_Shanghai (+8*3600)

#ifdef __cplusplus

namespace rtc
{
    struct TimeDate
    {
        uint16_t year;
        uint8_t month;
        uint8_t day;
        uint8_t hour;
        uint8_t minute;
        uint8_t second;
    };

    typedef uint32_t UnixTime;

    void UnixToTimedate(UnixTime t, TimeDate* dt, uint32_t timezone_offset);
    void TimedateToUnix(TimeDate* dt, UnixTime* t, uint32_t timezone_offset);

    void setTimedate(TimeDate* t);
    void getTimedate(TimeDate* dt);
}

#endif

#endif //BSP_RTC_H
