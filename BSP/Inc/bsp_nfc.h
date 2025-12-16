#ifndef BSP_NFC_H
#define BSP_NFC_H

#include "bsp_core.h"

#ifdef __cplusplus

namespace nfc
{
    enum NFC_Route
    {
        ROUTE_NONE = 0,
        ROUTE_C1 = 1,
        ROUTE_C2 = 2,
        ROUTE_C3 = 3,
        ROUTE_C4 = 4,
        ROUTE_PN532 = 5
    };

    void set_route();
    void enable_transparent_mode();
    void disable_transparent_mode();
    void transparent_send_cb();
    void transparent_recv_cb(uint16_t size);

    extern NFC_Route nfc_current_route;
}

#endif

#endif
