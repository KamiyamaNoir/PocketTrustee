#include "otp.h"
#include <cstring>
#include "aes.h"
#include "crypto_base.h"
#include "lfs_base.h"
#include "zcbor_common.h"
#include "zcbor_decode.h"

#define TOTP_FILE_SIZE 128

OTP_TOTP::OTP_TOTP(const char* totp_name) :
name{}, key{}
{
    uint8_t cache[TOTP_FILE_SIZE];
    uint8_t plaintext[TOTP_FILE_SIZE];
    char path[40] = "otp/";

    strcpy(this->name, totp_name);

    strcat(path, totp_name);
    strcat(path, ".totp");

    auto fs = LittleFS::fs_file_handler(path);
    int err = fs.read(cache, sizeof(cache));
    if (err <= 0)
    {
        return;
    }

    auto status = HAL_CRYP_AESECB_Decrypt(&hcryp, cache, sizeof(cache), plaintext, 1000);
    if (status != HAL_OK)
    {
        return;
    }

    //NOLINTNEXTLINE
    zcbor_string zcbor_str;
    uint32_t t;
    ZCBOR_STATE_D(zcbor_state, 2, plaintext, sizeof(plaintext), 1, 0);
    zcbor_list_start_decode(zcbor_state);
    zcbor_bstr_decode(zcbor_state, &zcbor_str);
    memcpy(this->key, zcbor_str.value, zcbor_str.len);
    this->key_length = zcbor_str.len;
    zcbor_uint32_decode(zcbor_state, &t);
    this->step = t;
    zcbor_uint32_decode(zcbor_state, &t);
    this->otp_len = t;
}

OTP_TOTP::OTP_TOTP(const char* name, const uint8_t* key, uint8_t key_length) :
name{}, key{}, key_length(key_length)
{
    strcpy(this->name, name);
    memcpy(this->key, key, key_length);
}

const char* OTP_TOTP::getName() const
{
    return this->name;
}

// In fact, there are no situations where device need to new a totp itself
/*
int OTP_TOTP::save()
{
    uint8_t plain_text[TOTP_FILE_SIZE];
    uint8_t ciphertext[TOTP_FILE_SIZE];
    ZCBOR_STATE_E(zcbor_state, 2, plain_text, sizeof(plain_text), 1);
    zcbor_list_start_encode(zcbor_state, 3);
    zcbor_bstr_encode_ptr(zcbor_state, reinterpret_cast<char*>(key), key_length);
    zcbor_uint32_put(zcbor_state, 30);
    zcbor_uint32_put(zcbor_state, 6);
    zcbor_list_end_encode(zcbor_state, 3);
    auto status = HAL_CRYP_AESECB_Encrypt(&hcryp, plain_text, zcbor_state->payload - plain_text, ciphertext, 1000);
    if (status != HAL_OK)
    {
        return -1;
    }
    char path[48] = "otp/";
    strcat(path, name);
    strcat(path, ".totp");
    auto fs = LittleFS::fs_file_handler(path);
    int err = fs.write(ciphertext, sizeof(ciphertext));
    if (err < 0) return err;
    return 0;
}
*/

int OTP_TOTP::calculate(uint32_t* pResult)
{
    int err = crypto::totp_calculate(this->key, this->key_length, pResult);
    return err;
}

