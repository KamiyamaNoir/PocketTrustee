#include "driver_rtc.hpp"
#include "common_crc.hpp"

__PACKED_STRUCT RtcBackData {
    uint32_t uuid1, uuid2, uuid3;
#ifndef STM32L4
    uint32_t store_unixtime_h, store_unixtime_l;
#endif
    uint32_t crash_id;
    uint32_t crc32;
} static rtc_record_;

int CoreRtc::Init() {
    ll_rtc_base_init();

    RtcBackData rtc_data{};
    int err {};
    auto* p_data = reinterpret_cast<uint32_t*>(&rtc_data);
    for (int i = 0;i < sizeof(RtcBackData) / sizeof(uint32_t);i++) {
        err += ll_rtc_read_backup_ram(i, p_data + i);
    }

    if (err) {
        return err;
    }

    Crc32Mpeg2 crc_calc {};
    crc_calc.Continue(reinterpret_cast<uint8_t*>(&rtc_data), sizeof(RtcBackData) - 4);

    if (
       rtc_data.uuid1 != DEVICE_UID1 ||
       rtc_data.uuid2 != DEVICE_UID2 ||
       rtc_data.uuid3 != DEVICE_UID3 ||
       rtc_data.crc32 != crc_calc.Value()
    ) {
        // Initialize RTC
        ll_rtc_clock_init();
        // Clear crash id in memory while keeping it in rtc_record_
        RecordSystemStatus(0);
        return 1;
    }

    // Already initialize before
    // recover data

    rtc_record_ = rtc_data;

    return 0;
}

int CoreRtc::RecordSystemStatus(uint32_t crash_id) {
    int err {};

    rtc_record_.uuid1 = DEVICE_UID1;
    rtc_record_.uuid2 = DEVICE_UID2;
    rtc_record_.uuid3 = DEVICE_UID3;

    rtc_record_.crash_id = crash_id;
    Crc32Mpeg2 crc_calc {};
    crc_calc.Continue(reinterpret_cast<uint8_t*>(&rtc_record_), sizeof(RtcBackData) - 4);
    rtc_record_.crc32 = crc_calc.Value();

    auto* p_data = reinterpret_cast<uint32_t*>(&rtc_record_);
    for (int i = 0;i < sizeof(RtcBackData) / sizeof(uint32_t);i++) {
        err += ll_rtc_write_backup_ram(i, p_data[i]);
    }

    return err;
}

int CoreRtc::GetCrashId(uint32_t* p_value) {
    *p_value = rtc_record_.crash_id;
    return 0;
}

#ifdef STM32L4

DateTime CoreRtc::GetDateTime() {
    DateTime dt{};
    ll_rtc_get_timedate(&dt);
    dt.zone = TIME_ZONE_Shanghai;
    return dt;
}

void CoreRtc::SetDateTime(DateTime* t) {
    ll_rtc_set_timedate(t);
}

#else

DateTime CoreRtc::GetDateTime() {
    uint64_t base_unixtime = (uint64_t)rtc_record_.store_unixtime_h << 32 | (uint64_t)rtc_record_.store_unixtime_l;
    uint32_t rtc_counter {};
    ll_rtc_get_counter(&rtc_counter);

    base_unixtime += rtc_counter;

    return DateTime::FromUnixTime(base_unixtime, TIME_ZONE_Shanghai);
}

void CoreRtc::SetDateTime(DateTime* t) {
    uint64_t unixtime = t->ToUnixTime();

    rtc_record_.store_unixtime_h = (uint32_t)((unixtime >> 32) & 0xFFFFFFFF);
    rtc_record_.store_unixtime_l = (uint32_t)(unixtime & 0xFFFFFFFF);

    ll_rtc_set_counter(0);
}
#endif
