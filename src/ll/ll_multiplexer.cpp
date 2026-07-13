#include "main.h"
#include "driver_multiplexer.hpp"

int ll_multiplexer_set(CoreMultiplexer::MultiplexerChannel channel) {
    HF_EN1_GPIO_Port->BSRR = HF_EN1_Pin;
    if (channel == CoreMultiplexer::MUX_NONE) return 0;
    if (channel == CoreMultiplexer::MUX_BYPASS)
    {
        HF_S3_GPIO_Port->BSRR = HF_S3_Pin;
    }
    else
    {
        HF_EN1_GPIO_Port->BRR = HF_EN1_Pin;
        switch (channel)
        {
        case CoreMultiplexer::MUX_CHANNEL1:
            // Card1 -> L L
            HF_S1_GPIO_Port->BRR = HF_S1_Pin;
            HF_S2_GPIO_Port->BRR = HF_S2_Pin;
            break;
        case CoreMultiplexer::MUX_CHANNEL2:
            // Card2 -> H L
            HF_S1_GPIO_Port->BSRR = HF_S1_Pin;
            HF_S2_GPIO_Port->BRR = HF_S2_Pin;
            break;
        case CoreMultiplexer::MUX_CHANNEL3:
            // Card3 -> L H
            HF_S1_GPIO_Port->BRR = HF_S1_Pin;
            HF_S2_GPIO_Port->BSRR = HF_S2_Pin;
            break;
        case CoreMultiplexer::MUX_CHANNEL4:
            // Card4 -> H H
            HF_S1_GPIO_Port->BSRR = HF_S1_Pin;
            HF_S2_GPIO_Port->BSRR = HF_S2_Pin;
        default:
            break;
        }
    }
    return 0;
}
