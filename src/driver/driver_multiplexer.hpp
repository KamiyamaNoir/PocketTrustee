#ifndef POCKETTRUSTEE_DRIVER_MULTIPLEXER_HPP
#define POCKETTRUSTEE_DRIVER_MULTIPLEXER_HPP

#include "main.h"

class CoreMultiplexer
{
public:
    enum MultiplexerChannel
    {
        MUX_NONE,
        MUX_CHANNEL1,
        MUX_CHANNEL2,
        MUX_CHANNEL3,
        MUX_CHANNEL4,
        MUX_BYPASS,
    };

    static MultiplexerChannel Get();
    static void Set(MultiplexerChannel channel);
};

int ll_multiplexer_set(CoreMultiplexer::MultiplexerChannel channel);

#endif //POCKETTRUSTEE_DRIVER_MULTIPLEXER_HPP
