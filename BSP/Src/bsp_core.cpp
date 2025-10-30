#include "bsp_core.h"
#include "bsp_adc.h"
#include "bsp_nfc.h"
#include "bsp_rfid.h"
#include "cmsis_os.h"
#include "crypto_base.h"
#include "tim.h"
#include "gui.h"
#include "host.h"
#include "usart.h"
#include "lfs_base.h"

extern gui::Display gui_main;

extern void cdc_acm_init();
extern void usbd_deinit();
extern volatile bool in_managermode;
extern void fingerprint_uart_callback(uint16_t size);

extern osThreadId defaultTaskHandle;
extern osThreadId manager_taskHandle;
extern osThreadId ADCSampleTaskHandle;
extern osThreadId IdealTaskHandle;

void SystemClockConfig(core::SYSTEM_CLK clk);

void HAL_Delay(uint32_t Delay)
{
    osDelay(Delay);
}

// static uint8_t uart_buffer[128];

void sys_startup()
{
    rfid::set_drive_mode(rfid::STOP);
    LittleFS::init();
#ifndef DEBUG_ENABLE
    cdc_acm_init();
    in_managermode = true;
    for (;;)
    {
        auto err = hostCommandInvoke(true);
        if (err == 2)
            break;
    }
    in_managermode = false;
    usbd_deinit();
#endif
    // HAL_UARTEx_ReceiveToIdle_IT(&huart1, uart_buffer, sizeof(uart_buffer));
    fingerprint_uart_callback(0);
}

void core::StartIdealTask()
{
    vTaskResume(IdealTaskHandle);
}

void core::StopIdealTask()
{
    vTaskSuspend(IdealTaskHandle);
}

void StartDefaultTask(void const * argument)
{
    sys_startup();
    gui::GUI_Task();
}

void StartManagerTask(void const * argument)
{
    for (;;)
    {
        if (in_managermode)
        {
            hostCommandInvoke();
        }
        osDelay(1);
    }
}

void StartADCSample(void const* argument)
{
    for (;;)
    {
        // osDelay(100);
        bsp_adc::start_convert();
        osDelay(500);
        bsp_adc::stop_convert();
        vTaskSuspend(nullptr);
    }
}

void PreSleepProcessing(uint32_t ulExpectedIdleTime)
{
#ifndef DEBUG_ENABLE
    HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);
#endif
}

void PostSleepProcessing(uint32_t ulExpectedIdleTime)
{
#ifndef DEBUG_ENABLE
    SystemClockConfig(core::SCLK_FULLSPEED);
#endif
    vTaskResume(ADCSampleTaskHandle);
}

void SystemClockConfig(core::SYSTEM_CLK clk)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    __disable_irq();
    // Cut off periphery
    usbd_deinit();
    // HAL_UART_DeInit(&hlpuart1);
    // HAL_UART_DeInit(&huart1);
    // HAL_UART_DeInit(&huart3);

    // Switch to MSI before reconfiguration
    HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);

    // reconfigurate pll
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48|RCC_OSCILLATORTYPE_LSE
                              |RCC_OSCILLATORTYPE_MSI;
    RCC_OscInitStruct.LSEState = RCC_LSE_ON;
    RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
    RCC_OscInitStruct.MSIState = RCC_MSI_ON;
    RCC_OscInitStruct.MSICalibrationValue = 0;
    RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
    RCC_OscInitStruct.PLL.PLLM = 1;
    RCC_OscInitStruct.PLL.PLLN = 16;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV8;
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV8;
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV4;
    if (clk == core::SCLK_HIGHSPEED)
        RCC_OscInitStruct.PLL.PLLN = 80;
    else if (clk == core::SCLK_FULLSPEED)
        RCC_OscInitStruct.PLL.PLLN = 32;
    else
        RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                  |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    // reconfigurate sysclk according to clk
    if (clk == core::SCLK_FULLSPEED)
    {
        RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
        HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);
        HAL_RCCEx_EnableMSIPLLMode();
    }
    else if (clk == core::SCLK_HIGHSPEED)
    {
        RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
        HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4);
        HAL_RCCEx_EnableMSIPLLMode();
    }
    else if (clk == core::SCLK_SLEEPSPEED)
    {
        RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
        RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV4;
        HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);
        HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE2);
    }

    __enable_irq();
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == FLG_CHG_Pin)
    {
        GUI_OP_NULL_ISR();
    }
    else if (GPIO_Pin == FLG_USB_Pin)
    {
    }
    else
    {
        switch (GPIO_Pin)
        {
        case KUP_Pin:
            if (KUP_GPIO_Port->IDR & KUP_Pin) break;
            // gui_main.process(gui::OP_UP);
            GUI_OP_UP_ISR();
            break;
        case KDN_Pin:
            if (KDN_GPIO_Port->IDR & KDN_Pin) break;
            // gui_main.process(gui::OP_DOWN);
            GUI_OP_DOWN_ISR();
            break;
        case KEN_Pin:
            if (KEN_GPIO_Port->IDR & KEN_Pin) break;
            // gui_main.process(gui::OP_ENTER);
            GUI_OP_ENTER_ISR();
            break;
        default:
            break;
        }
    }
}

void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{
    UNUSED(hrtc);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == &htim16)
    {
        rfid::emulate_callback();
    }
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (huart == &huart3)
    {
        nfc::transparent_recv_cb(Size);
    }
    else if (huart == &hlpuart1)
    {
        fingerprint_uart_callback(Size);
    }
}
