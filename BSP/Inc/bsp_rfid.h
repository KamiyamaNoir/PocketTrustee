#ifndef BSP_RFID_H
#define BSP_RFID_H

#include "bsp_core.h"

#ifdef __cplusplus

namespace rfid
{
    enum DriveMode
    {
        STOP,
        READ,
        EMULATE
    };

    struct IDCard
    {
        uint32_t raw_mcode[2];
        uint32_t idcode;
    };

    void newSample();
    uint8_t availablePoint();
    BSP_Status parseSample(IDCard* dest);
    void loadEmulator(const IDCard* card);
    void clearEmulator();

    void lf_oa_callback();
    void emulate_callback();
    void set_drive_mode(DriveMode mode);
    void RFID_BufferFullCallback();
}

#endif

#endif
