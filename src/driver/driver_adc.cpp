#include "driver_adc.hpp"
#include "adc.h"

float CoreADC::GetBatteryVoltage() {
    LL_ADC_Data data {};
    ll_adc_init();
    ll_adc_get_value(&data, nullptr);
    ll_adc_powerdown();
    return static_cast<float>(data.vbat) / static_cast<float>(data.vref) * 2.4f;
}

float CoreADC::GetRSSI() {
    LL_ADC_Data data {};
    ll_adc_init();
    ll_adc_get_value(&data, nullptr);
    ll_adc_powerdown();
    return static_cast<float>(data.lf_rssi) / static_cast<float>(data.vref) * 1.2f;
}
