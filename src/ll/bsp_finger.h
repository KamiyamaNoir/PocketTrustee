#ifndef BSP_FINGER_H
#define BSP_FINGER_H

#include "bsp_core.h"

#ifdef __cplusplus

namespace finger
{
    struct SearchResult
    {
        uint8_t result;
        uint16_t page_id;
        uint16_t score;
    };

    uint8_t getImageAuth();
    uint8_t genChar(uint8_t buffer_id=1);
    uint8_t accrMatch();
    SearchResult Search(uint8_t buffer_id=1, uint16_t start=0, uint16_t page_num=10);
    uint8_t regModel();
    uint8_t storeChar(uint16_t page_id, uint8_t buffer_id=1);
    uint8_t loadChar(uint16_t page_id, uint8_t buffer_id=2);
    uint8_t deleteChar(uint16_t page_id, uint16_t number=1);
    uint8_t clearChar();

    uint8_t getImageEnroll();
    uint8_t cancelOperation();
}

#endif

#endif
