#ifndef BSP_CORE_H
#define BSP_CORE_H

#include "main.h"

#ifdef __cplusplus
// < CPP FILE >

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
#ifdef __cplusplus
}
#endif

#endif
