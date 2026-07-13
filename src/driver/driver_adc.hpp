#ifndef POCKETTRUSTEE_DRIVER_ADC_HPP
#define POCKETTRUSTEE_DRIVER_ADC_HPP

#include "main.h"

class CoreADC
{
public:
    struct LL_ADC_Data {
        uint32_t vbat, lf_rssi, vref;
    };
    
    static float GetBatteryVoltage();
    static float GetRSSI();
};

#endif //POCKETTRUSTEE_DRIVER_ADC_HPP
