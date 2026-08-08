#include "driver_adc.hpp"
#include "adc.h"

namespace {
    uint16_t adc_buffer[3];
}

int ll_adc_init() {
    MX_ADC1_Init();
    HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
    return 0;
}

int ll_adc_powerdown() {
    HAL_ADC_Stop_DMA(&hadc1);
    HAL_ADCEx_EnterADCDeepPowerDownMode(&hadc1);
    return 0;
}

int ll_adc_get_value(CoreADC::LL_ADC_Data * p_value, CancellationToken * token) {
    HAL_ADC_Start_DMA(&hadc1, reinterpret_cast<uint32_t*>(adc_buffer), 3);
    HAL_ADC_PollForConversion(&hadc1, 1000);
    p_value->vbat = adc_buffer[0];
    p_value->lf_rssi = adc_buffer[1];
    p_value->vref = adc_buffer[2];
    return 0;
}
