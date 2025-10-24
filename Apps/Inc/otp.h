#ifndef OTP_H
#define OTP_H

#include "main.h"

__PACKED_STRUCT TOTP_File_Struct
{
    char name[32];
    uint8_t key[32];
};

class TOTP_File
{
public:
    explicit TOTP_File(const char* path);
    explicit TOTP_File(const TOTP_File_Struct* file);
    TOTP_File();
    ~TOTP_File() = default;

    int calculate(uint32_t* pResult);
    static int save(TOTP_File_Struct* src);

    const char* getName() const;
private:
    char path[48];
    char name[32];
    uint8_t key[20];
};

#endif
