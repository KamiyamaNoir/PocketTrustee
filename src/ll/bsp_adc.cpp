#include "adc.h"
#include "bsp_adc.h"

static uint16_t adc_buffer[3];

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    UNUSED(hadc);
}

void HAL_ADC_LevelOutOfWindowCallback(ADC_HandleTypeDef *hadc)
{
    if (hadc == &hadc1)
    {
    }
}

void bsp_adc::start_convert()
{
    MX_ADC1_Init();
    // ADC_AnalogWDGConfTypeDef awdg = {
    //     .WatchdogNumber = ADC_ANALOGWATCHDOG_1,
    //     .WatchdogMode = ADC_ANALOGWATCHDOG_SINGLE_REG,
    //     .Channel = ADC_CHANNEL_12,
    //     .ITMode = ENABLE,
    //     .HighThreshold = 125,
    //     .LowThreshold = 0,
    // };
    HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
    // HAL_ADC_AnalogWDGConfig(&hadc1, &awdg);
    HAL_ADC_Start_DMA(&hadc1, reinterpret_cast<uint32_t*>(adc_buffer), 3);
}

void bsp_adc::stop_convert()
{
    HAL_ADC_Stop_DMA(&hadc1);
    HAL_ADCEx_EnterADCDeepPowerDownMode(&hadc1);
}

float bsp_adc::getVoltageBattery()
{
    return (static_cast<float>(adc_buffer[0]) / static_cast<float>(adc_buffer[2])) * 2.4f;
}

float bsp_adc::getLF_RSSI()
{
    return (static_cast<float>(adc_buffer[1]) / static_cast<float>(adc_buffer[2])) * 1.2f;
}

