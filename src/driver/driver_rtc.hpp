#ifndef POCKETTRUSTEE_DRIVER_RTC_HPP
#define POCKETTRUSTEE_DRIVER_RTC_HPP

#include "main.h"

enum TimeZoneOffset
{
    TIME_ZONE_None = 0,
    TIME_ZONE_Shanghai = (+8 * 3600)
};

struct DateTime
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;

    TimeZoneOffset zone = TIME_ZONE_None;

    static bool is_leap_year(uint16_t year) {
        return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    }

    static constexpr uint8_t days_in_month[12] = {
        31, 28, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31
    };

    uint64_t ToUnixTime() const {
        uint64_t total_seconds = 0;

        for (uint16_t _year = 1970; _year < year; _year++)
        {
            uint32_t year_days = is_leap_year(_year) ? 366 : 365;
            total_seconds += year_days * 24 * 3600;
        }

        for (uint8_t _month = 1; _month < month; _month++)
        {
            uint8_t month_days = days_in_month[_month - 1];
            if (_month == 2 && is_leap_year(year))
                month_days = 29;
            total_seconds += month_days * 24 * 3600;
        }

        total_seconds += (day - 1) * 24 * 3600;

        total_seconds += hour * 3600;
        total_seconds += minute * 60;
        total_seconds += second;

        return total_seconds - zone;
    }

    static DateTime FromUnixTime(uint64_t unix_time, TimeZoneOffset offset = TIME_ZONE_None) {
        unix_time += offset;
        DateTime dt {};

        dt.second = unix_time % 60;
        unix_time /= 60;
        dt.minute = unix_time % 60;
        unix_time /= 60;
        dt.hour = unix_time % 24;
        unix_time /= 24;
        uint32_t days = unix_time;

        dt.year = 1970;
        for (;;)
        {
            uint16_t year_days = is_leap_year(dt.year) ? 366 : 365;
            if (days < year_days)
                break;
            days -= year_days;
            dt.year++;
        }

        dt.month = 1;
        for (;;)
        {
            uint8_t month_days = days_in_month[dt.month - 1];
            if (dt.month == 2 && is_leap_year(dt.year))
                month_days = 29;

            if (days < month_days)
                break;

            days -= month_days;
            dt.month++;
        }

        dt.day = days + 1;
        dt.zone = offset;

        return dt;
    }
};

class CoreRtc
{
public:
    static int Init();

    static int RecordSystemStatus(uint32_t crash_id);
    static int GetCrashId(uint32_t * p_value);

    static void SetDateTime(DateTime * t);
    static DateTime GetDateTime();
};

int ll_rtc_clock_init();
int ll_rtc_base_init();
int ll_rtc_write_backup_ram(int offset, uint32_t value);
int ll_rtc_read_backup_ram(int offset, uint32_t * p_value);
#ifdef STM32L4

int ll_rtc_set_timedate(DateTime * dt);
int ll_rtc_get_timedate(DateTime * dt);

#else
int ll_rtc_set_counter(uint32_t count);
int ll_rtc_get_counter(uint32_t * p_value);
#endif

#endif //POCKETTRUSTEE_DRIVER_RTC_HPP
