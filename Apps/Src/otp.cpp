#include "otp.h"
#include <cstring>
#include "aes.h"
#include "crypto_base.h"
#include "lfs_base.h"

TOTP_File::TOTP_File(const char* path) :
TOTP_File()
{
    uint8_t cache[64];
    uint8_t plaintext[64];

    strcpy(this->path, path);
    auto fs = LittleFS::fs_file_handler(path);
    int err = fs.read(cache, sizeof(cache));
    if (err != sizeof(cache))
    {
        return;
    }

    auto status = HAL_CRYP_AESECB_Decrypt(&hcryp, cache, sizeof(cache), plaintext, 1000);
    if (status != HAL_OK)
    {
        return;
    }

    auto* totp_file = reinterpret_cast<TOTP_File_Struct*>(plaintext);
    strcpy(this->name, totp_file->name);
    memcpy(this->key, totp_file->key, 20);
}

TOTP_File::TOTP_File(const TOTP_File_Struct* file) :
TOTP_File()
{
    strcpy(this->name, file->name);
    memcpy(this->key, file->key, 20);
}

TOTP_File::TOTP_File() :
path{}, name{}, key{}
{

}

const char* TOTP_File::getName() const
{
    return this->name;
}

int TOTP_File::save(TOTP_File_Struct* src)
{
    uint8_t encrypto_text[sizeof(TOTP_File_Struct)];
    auto* fbytes = reinterpret_cast<uint8_t*>(src);
    auto status = HAL_CRYP_AESECB_Encrypt(&hcryp, fbytes, sizeof(TOTP_File_Struct), encrypto_text, 1000);
    if (status != HAL_OK)
    {
        return -1;
    }

    char path[48] = "otp/";
    strcat(path, src->name);
    strcat(path, ".totp");
    auto fs = LittleFS::fs_file_handler(path);
    int err = fs.write(encrypto_text, sizeof(TOTP_File_Struct));
    if (err < 0) return err;
    return 0;
}

int TOTP_File::calculate(uint32_t* pResult)
{
    int err = crypto::totp_calculate(this->key, pResult);
    return err;
}

