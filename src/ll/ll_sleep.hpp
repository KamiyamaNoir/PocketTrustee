#ifndef POCKETTRUSTEE_LL_SLEEP_HPP
#define POCKETTRUSTEE_LL_SLEEP_HPP

#include "main.h"

class CoreSleepTimer
{
public:
    static void StartCountingDown(uint32_t milisecond);
    static void Trigger();
};

#endif //POCKETTRUSTEE_LL_SLEEP_HPP
