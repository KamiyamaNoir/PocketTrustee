#include "otp.hpp"
#include <cstring>
#include "aes.h"
#include "crypto_base.hpp"
#include "little_fs.hpp"
#include "zcbor_common.h"
#include "zcbor_decode.h"

PKT_ERR OTP_TOTP::load(const char* totp_name)
{
    uint8_t file_buffer[LittleFS_W25Q16::CACHE_SIZE];
    uint8_t cache[TOTP_FILE_SIZE_MAX];
    uint8_t plaintext[TOTP_FILE_SIZE_MAX];
    char path[TOTP_NAME_MAX + sizeof(totp_dir_base) + sizeof(totp_suffix)] {};

    if (totp_name == nullptr || totp_name[0] == '\0')
        return {
            .err = -1,
            .err_fs = LFS_ERR_INVAL,
            .msg = "Null pointer to path"
        };

    if (strlen(totp_name) > TOTP_NAME_MAX)
    {
        return {
            .err = -1,
            .err_fs = LFS_ERR_OK,
            .msg = "totp name too long"
        };
    }

    strcat(path, totp_dir_base);
    strcpy(this->name, totp_name);
    strcat(path, totp_name);
    strcat(path, totp_suffix);

    lfs_file_config open_cfg = {
        .buffer = file_buffer,
    };
    FileDelegate file;
    int err = file.open(path, LFS_O_RDONLY, &open_cfg);
    if (err < 0)
        return {
            .err = -1,
            .err_fs = err,
            .msg = "Totp fail to open file"
        };

    err = lfs_file_read(&fs_w25q16, &file.instance, cache, TOTP_FILE_SIZE_MAX);
    if (err < 0)
        return {
            .err = -1,
            .err_fs = err,
            .msg = "Totp fail to read file"
        };
    if (err == TOTP_FILE_SIZE_MAX)
        return {
            .err = -1,
            .err_fs = LFS_ERR_OK,
            .msg = "Bad totp file"
        };

    auto status = HAL_CRYP_AESECB_Decrypt(&hcryp, cache, err, plaintext, 1000);
    if (status != HAL_OK)
    {
        return {
            .err = -1,
            .err_fs = LFS_ERR_OK,
            .msg = "Totp fail to decrypt file"
        };
    }

    zcbor_string zcbor_str = {
        .value = nullptr,
        .len = 0,
    };
    uint32_t t;
    ZCBOR_STATE_D(zcbor_state, 2, plaintext, sizeof(plaintext), 1, 0);
    bool success = zcbor_list_start_decode(zcbor_state);
    success = success && zcbor_bstr_decode(zcbor_state, &zcbor_str);

    if (!success || zcbor_str.len > 20)
        return {
            .err = -1,
            .err_fs = LFS_ERR_OK,
            .msg = "Bad totp file"
        };

    memcpy(this->key, zcbor_str.value, zcbor_str.len);
    this->key_length = zcbor_str.len;

    success = zcbor_uint32_decode(zcbor_state, &t);
    this->step = t;
    success = success && zcbor_uint32_decode(zcbor_state, &t);
    this->otp_len = t;

    if (!success)
        return {
            .err = -1,
            .err_fs = LFS_ERR_OK,
            .msg = "Bad totp file"
        };
    return {
        .err = 0,
        .err_fs = LFS_ERR_OK,
        .msg = nullptr
    };
}

// OTP_TOTP::OTP_TOTP(const char* name, const uint8_t* key, uint8_t key_length) :
// name{}, key{}, key_length(key_length)
// {
//     strcpy(this->name, name);
//     memcpy(this->key, key, key_length);
// }

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

