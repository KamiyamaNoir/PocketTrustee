#ifndef BSP_NFC_H
#define BSP_NFC_H

#include "bsp_core.h"

#ifdef __cplusplus

namespace nfc
{
    void enable_transparent_mode();
    void disable_transparent_mode();
    void transparent_send_cb();
    void transparent_recv_cb(uint16_t size);
}

#endif

#endif
