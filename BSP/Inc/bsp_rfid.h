#ifndef BSP_RFID_H
#define BSP_RFID_H

#include "bsp_core.h"
#include <cstring>

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
        enum {IDCARD_NAME_MAX=25};
        static constexpr char idcard_dir_base[] = "idcards/";
        static constexpr char idcard_suffix[] = ".idcard";

        int load(const char* name)
        {
            if (strlen(name) > IDCARD_NAME_MAX)
            {
                return -1;
            }

            char path[IDCARD_NAME_MAX + sizeof(idcard_dir_base) + sizeof(idcard_suffix)];
            strcpy(path, idcard_dir_base);
            strcat(path, name);
            strcat(path, idcard_suffix);

            auto fs = LittleFS::fs_file_handler(path);
            int err = fs.read(reinterpret_cast<uint8_t*>(this), sizeof(IDCard));
            if (err < 0)
                return err;
            return 0;
        }

        int save(const char* name)
        {
            if (strlen(name) > IDCARD_NAME_MAX)
            {
                return -1;
            }

            char path[IDCARD_NAME_MAX + sizeof(idcard_dir_base) + sizeof(idcard_suffix)];
            strcpy(path, idcard_dir_base);
            strcat(path, name);
            strcat(path, idcard_suffix);

            auto fs = LittleFS::fs_file_handler(path);
            fs.seek(0);
            int err = fs.write(reinterpret_cast<uint8_t*>(this), sizeof(IDCard));
            if (err < 0)
                return err;
            return 0;
        }

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
