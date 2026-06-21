#include "driver_adc.hpp"
#include "adc.h"
#include "FreeRTOS.h"
#include "task.h"

static uint16_t adc_buffer[3];

static void adc_ll_init() {
    MX_ADC1_Init();
    HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
}

static void adc_ll_deinit() {
    HAL_ADC_Stop_DMA(&hadc1);
    HAL_ADCEx_EnterADCDeepPowerDownMode(&hadc1);
}

float CoreADC::GetBatteryVoltage() {
    return static_cast<float>(adc_buffer[0]) / static_cast<float>(adc_buffer[2]) * 2.4f;
}

float CoreADC::GetRSSI() {
    return static_cast<float>(adc_buffer[1]) / static_cast<float>(adc_buffer[2]) * 1.2f;
}

void CoreADC::Refresh() {
    adc_ll_init();
    HAL_ADC_Start_DMA(&hadc1, reinterpret_cast<uint32_t*>(adc_buffer), 3);
    HAL_ADC_PollForConversion(&hadc1, 1000);
    adc_ll_deinit();
    DEBUG_INFO("adc conv finish");
}

void CoreADC::RefreshAsync() {
    xTaskCreate([](void*) {
        CoreADC::Refresh();
        vTaskDelete(nullptr);
    }, "adc", 256, nullptr, 2, nullptr);
}
