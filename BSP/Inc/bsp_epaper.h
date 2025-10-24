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
    void update(const uint8_t* data, EpaperMode mode);
    void pre_update(const uint8_t* data);
    void partial_preconfig();
    void partial_write_ram(const uint8_t* data, uint16_t rx, uint8_t cy, uint16_t rw, uint8_t ch);
    void partial_update();
    void update_full(const uint8_t* data);
    void update_fast(const uint8_t* data);
    void update_part_full(const uint8_t* data);

}

#endif

#endif //BSP_EPAPER_H
