#ifndef OTP_H
#define OTP_H

#include "main.h"

class OTP_TOTP
{
public:
    enum {TOTP_NAME_MAX=26};

    static constexpr char totp_dir_base[] = "otp/";
    static constexpr char totp_suffix[] = ".totp";

    explicit OTP_TOTP(const char* totp_name);
    OTP_TOTP(const char* name, const uint8_t* key, uint8_t key_length);
    ~OTP_TOTP() = default;

    int calculate(uint32_t* pResult);
    // int save();

    const char* getName() const;
private:
    char name[TOTP_NAME_MAX];
    uint8_t key[20];
    uint8_t key_length = 20;
    uint8_t step = 30;
    uint8_t otp_len = 6;
};

#endif
