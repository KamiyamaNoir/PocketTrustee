#include "bsp_core.h"
// #include "bsp_epaper.h"
#include "bsp_nfc.h"
#include "bsp_rfid.h"
#include "cmsis_os.h"
#include "crypto_base.hpp"
#include "driver_adc.hpp"
#include "tim.h"
#include "gui.hpp"
#include "host.hpp"
#include "usart.h"
#include "little_fs.hpp"

extern gui::Display gui_main;

extern void cdc_acm_init();
extern void hid_keyboard_init();
extern void usbd_deinit();
extern void hid_keyboard_string(const char* str);
extern void fingerprint_uart_callback(uint16_t size);

extern osThreadId manager_taskHandle;
extern osThreadId IdealTaskHandle;

void HAL_Delay(uint32_t Delay)
{
    osDelay(Delay);
}

// static uint8_t uart_buffer[128];

__attribute__((section("._user_heap2_region"))) static uint8_t _userHeapRegion1[ SRAM2_SIZE ];
static uint8_t _userHeapRegion2[ 0x2000 ];

const HeapRegion_t xHeapRegions[] =
{
    {_userHeapRegion1, SRAM2_SIZE},
    {_userHeapRegion2, sizeof(_userHeapRegion2)},
    { nullptr, 0 }
};

void sys_startup()
{
    DEBUG_INFO("RTOS Entry");

    vPortDefineHeapRegions(xHeapRegions);

    DEBUG_INFO("Heap Regions %p %p", _userHeapRegion1, _userHeapRegion2);

    rfid::set_drive_mode(rfid::STOP);
    cmox_init_arg_t init_target = {CMOX_INIT_TARGET_L4, nullptr};
    cmox_initialize(&init_target);
    int fs_err = LittleFS_W25Q16::Mount();
    if (fs_err < 0)
    {
        LittleFS_W25Q16::Format();
        fs_err = LittleFS_W25Q16::Mount();
        if (fs_err < 0)
            SysFaultHandler(43);
    }
    CoreADC::Refresh();
#ifndef PKT_YES_DEBUG
    core::RegisterACMDevice();
    for (;;)
    {
        auto err = Host::hostCommandInvoke(true);
        if (err.err == 0 && err.err_fs == 1)
            break;
    }
    core::DeinitUSB();
#endif
    // HAL_UARTEx_ReceiveToIdle_IT(&huart1, uart_buffer, sizeof(uart_buffer));
    // fingerprint_uart_callback(0);
}

void SysFaultHandler(int err)
{
    // char err_code[8];
    // gui::Scheme sche(0, 0, GUI_WIDTH, GUI_HEIGHT, gui_main, gui_main.load_cache);
    // sche
    // .clear()
    // .put_string(0, 0, gui::ASCII_1608, ":( It looks like your device went something wrong");
    // sprintf(err_code, "%d", err);
    // sche.put_string(0, 20, gui::ASCII_3216, err_code);
    // epaper::pre_update(sche.data);
    __disable_irq();
    // HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);
    while (true) {}
}

void core::StartIdealTask()
{
    DEBUG_INFO("start ideal task");
    vTaskResume(IdealTaskHandle);
}

void core::StopIdealTask()
{
    DEBUG_INFO("stop ideal task");
    vTaskSuspend(IdealTaskHandle);
}

void core::StartManagerTask()
{
    DEBUG_INFO("start manager task");
    vTaskResume(manager_taskHandle);
}

void core::StopManagerTask()
{
    DEBUG_INFO("stop manager task");
    vTaskSuspend(manager_taskHandle);
}

void core::RegisterACMDevice()
{
    DEBUG_INFO("register acm device");
    cdc_acm_init();
}

void core::RegisterHIDDevice()
{
    DEBUG_INFO("register hid device");
    hid_keyboard_init();
}

void core::USB_HID_Send(const char* content)
{
    hid_keyboard_string(content);
}

void core::DeinitUSB()
{
    DEBUG_INFO("deinit usb");
    usbd_deinit();
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
        Host::hostCommandInvoke();
        osDelay(1);
    }
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
