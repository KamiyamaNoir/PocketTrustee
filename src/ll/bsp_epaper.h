#ifndef BSP_EPAPER_H
#define BSP_EPAPER_H

#include "bsp_core.h"

#ifdef __cplusplus

namespace epaper
{
    enum EpaperMode
    {
        EP_FULL,
        EP_FAST,
        EP_PART,
        EP_GREY
    };
    void pre_update(const uint8_t* data);
    void update_fast(const uint8_t* data);
    void update_part_full(const uint8_t* data);

}

#endif

#endif //BSP_EPAPER_H
