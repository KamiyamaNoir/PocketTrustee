#ifndef OTP_H
#define OTP_H

#include "bsp_core.h"

class OTP_TOTP
{
public:
    enum {TOTP_NAME_MAX=26, TOTP_FILE_SIZE_MAX=128};

    static constexpr char totp_dir_base[] = "otp/";
    static constexpr char totp_suffix[] = ".totp";

    PKT_ERR load(const char* totp_name);

    int calculate(uint32_t* pResult);
    // int save();

    char name[TOTP_NAME_MAX] {};
    uint8_t key[20] {};
    uint8_t key_length = 20;
    uint8_t step = 30;
    uint8_t otp_len = 6;
};

#endif
