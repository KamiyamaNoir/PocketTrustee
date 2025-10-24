#ifndef BSP_ADC_H
#define BSP_ADC_H

#ifdef __cplusplus

namespace bsp_adc
{
    void start_convert();
    void stop_convert();
    float getVoltageBattery();
    float getLF_RSSI();
}

#endif

#endif
