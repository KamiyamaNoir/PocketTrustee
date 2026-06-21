#ifndef POCKETTRUSTEE_LL_CLOCK_HPP
#define POCKETTRUSTEE_LL_CLOCK_HPP

#include "main.h"

class CoreClock
{
public:
    enum CoreSpeedEnum
    {
        CoreSpeedVeryLow,
        CoreSpeedLow,
        CoreSpeedHigh
    };

    static void SystemClockConfig(CoreSpeedEnum speed);

    static uint32_t GetHCLK_Value()
    {
        return HAL_RCC_GetHCLKFreq();
    }
};

#endif //POCKETTRUSTEE_LL_CLOCK_HPP
