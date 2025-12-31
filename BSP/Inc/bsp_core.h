#ifndef BSP_CORE_H
#define BSP_CORE_H

#include "main.h"

struct PKT_ERR
{
    int err;
    int err_fs;
    const char* msg;
};

#ifdef __cplusplus
// < CPP FILE >

constexpr uint8_t version_c1 = 1;
constexpr uint8_t version_c2 = 1;
constexpr uint8_t version_c3 = 0;

enum BSP_Status
{
    BSP_OK,
    BSP_TIMEOUT,
    BSP_EXCEPTION
};

namespace core
{
    enum SYSTEM_CLK
    {
        SCLK_HIGHSPEED,
        SCLK_FULLSPEED,
        SCLK_SLEEPSPEED
    };

    void StartIdealTask();
    void StopIdealTask();

    void StartManagerTask();
    void StopManagerTask();

    void RegisterACMDevice();
    void RegisterHIDDevice();
    void USB_HID_Send(const char* content);
    void DeinitUSB();

}

#endif


#ifdef __cplusplus
extern "C"{
#endif
// < C FILE >
    void sys_startup();

    void StartDefaultTask(void const * argument);
    void StartManagerTask(void const * argument);
    void StartADCSample(void const * argument);

    void SysFaultHandler(int err);
#ifdef __cplusplus
}
#endif

#endif
