#include "bsp_rtc.h"
#include "rtc.h"

#define RTC_YEAR_OFFSET 2025
#define RTC_BKP_CALIB RTC_BKP0R

__STATIC_INLINE int is_leap_year(uint16_t year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

constexpr uint8_t days_in_month[12] = {
    31, 28, 31, 30, 31, 30,
    31, 31, 30, 31, 30, 31
};

void rtc::TimedateToUnix(TimeDate* dt, UnixTime* t, uint32_t timezone_offset)
{
    UnixTime total_seconds = 0;

    for (uint16_t year = 1970; year < dt->year; year++)
    {
        uint32_t year_days = is_leap_year(year) ? 366 : 365;
        total_seconds += year_days * 24 * 3600;
    }

    for (uint8_t month = 1; month < dt->month; month++)
    {
        uint8_t month_days = days_in_month[month - 1];
        if (month == 2 && is_leap_year(dt->year))
            month_days = 29;
        total_seconds += month_days * 24 * 3600;
    }

    total_seconds += (dt->day - 1) * 24 * 3600;

    total_seconds += dt->hour * 3600;
    total_seconds += dt->minute * 60;
    total_seconds += dt->second;

    *t = total_seconds - timezone_offset;
}

void rtc::UnixToTimedate(UnixTime t, TimeDate* dt, uint32_t timezone_offset)
{
    UnixTime seconds = t + timezone_offset;

    dt->second = seconds % 60;
    seconds /= 60;
    dt->minute = seconds % 60;
    seconds /= 60;
    dt->hour = seconds % 24;
    seconds /= 24;
    uint32_t days = seconds;

    dt->year = 1970;
    for (;;)
    {
        uint16_t year_days = is_leap_year(dt->year) ? 366 : 365;
        if (days < year_days)
            break;
        days -= year_days;
        dt->year++;
    }

    dt->month = 1;
    for (;;)
    {
        uint8_t month_days = days_in_month[dt->month - 1];
        if (dt->month == 2 && is_leap_year(dt->year))
            month_days = 29;

        if (days < month_days)
            break;

        days -= month_days;
        dt->month++;
    }

    dt->day = days + 1;
}

void rtc::setTimedate(TimeDate* t)
{
    RTC_TimeTypeDef sTime = {
        .Hours = t->hour,
        .Minutes = t->minute,
        .Seconds = t->second,
    };
    RTC_DateTypeDef sDate = {
        .Month = t->month,
        .Date = t->day,
        .Year = static_cast<uint8_t>(t->year - RTC_YEAR_OFFSET),
    };
    HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
    HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
}

void rtc::getTimedate(TimeDate* dt)
{
    RTC_TimeTypeDef sTime = {};
    RTC_DateTypeDef sDate = {};
    HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
    *dt = {
        .year = static_cast<uint16_t>(sDate.Year + RTC_YEAR_OFFSET),
        .month = sDate.Month,
        .day = sDate.Date,
        .hour = sTime.Hours,
        .minute = sTime.Minutes,
        .second = sTime.Seconds,
    };
}
