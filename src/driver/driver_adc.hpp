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

int ll_adc_init();
int ll_adc_powerdown();
int ll_adc_get_value(CoreADC::LL_ADC_Data * p_value, CancellationToken * token);

#endif //POCKETTRUSTEE_DRIVER_ADC_HPP
