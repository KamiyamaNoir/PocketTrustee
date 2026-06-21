#ifndef BSP_FLASH_H
#define BSP_FLASH_H

#include "bsp_core.h"

#ifdef __cplusplus

namespace bsp_flash
{
    void read_data(uint32_t addr, uint8_t* rx_buffer, uint16_t len);
    void page_program(uint32_t addr, const uint8_t* data, uint16_t len);
    void sector_erase(uint32_t addr);
    void chip_erase();
    void read_device_id(uint8_t* rx_buffer);
    void reset_device();
    void enter_powerdown(bool enter);
    bool read_busy();
}

#endif

#endif
