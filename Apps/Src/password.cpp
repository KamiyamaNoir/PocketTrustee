#include "password.h"
#include "lfs_base.h"
#include "zcbor_common.h"
#include "zcbor_decode.h"
#include "zcbor_encode.h"
#include "aes.h"
#include <cstring>

int PasswordFile::load(const char* pwd_name)
{
    if (pwd_name == nullptr || pwd_name[0] == '\0')
    {
        return -1;
    }

    if (strlen(pwd_name) > PWD_NAME_MAX)
    {
        return -1;
    }

    strcpy(this->name, pwd_name);

    char path[PWD_NAME_MAX + sizeof(pwd_dir) + sizeof(pwd_suffix)];
    strcpy(path, pwd_dir);
    strcat(path, pwd_name);
    strcat(path, pwd_suffix);

    uint8_t fbytes_encrypto[128];
    uint8_t fbytes_plaintext[sizeof(fbytes_encrypto)];

    int err;
    auto fs = LittleFS::fs_file_handler(path, LFS_O_RDONLY, &err);
    if (err < 0)
        return err;
    err = fs.read(fbytes_encrypto, sizeof(fbytes_encrypto));

    if (err < 0)
        return err;

    HAL_CRYP_AESECB_Decrypt(&hcryp, fbytes_encrypto, sizeof(fbytes_encrypto), fbytes_plaintext, 1000);

    //NOLINTNEXTLINE
    zcbor_string zcbor_str;
    bool success;
    ZCBOR_STATE_D(state, 2, fbytes_plaintext, sizeof(fbytes_plaintext), 1, 0);
    zcbor_list_start_decode(state);
    // Decode account
    success = zcbor_tstr_decode(state, &zcbor_str);
    if (!success)
        return -1;
    memcpy(this->account, zcbor_str.value, zcbor_str.len);
    this->account[zcbor_str.len] = '\0';
    // Decode password
    success = zcbor_tstr_decode(state, &zcbor_str);
    if (!success)
        return -1;
    memcpy(this->pwd, zcbor_str.value, zcbor_str.len);
    this->pwd[zcbor_str.len] = '\0';
    return 0;
}

int PasswordFile::save()
{
    if (name[0] == '\0')
    {
        return -1;
    }

    char path[PWD_NAME_MAX + sizeof(pwd_dir) + sizeof(pwd_suffix)];
    strcpy(path, pwd_dir);
    strcat(path, name);
    strcat(path, pwd_suffix);

    uint8_t payload[128];
    uint8_t encrypto_payload[sizeof(payload)];

    ZCBOR_STATE_E(state, 2, payload, sizeof(payload), 1);
    zcbor_list_start_encode(state, 2);
    zcbor_tstr_encode_ptr(state, account, strlen(account));
    zcbor_tstr_encode_ptr(state, pwd, strlen(pwd));
    zcbor_list_end_encode(state, 2);

    HAL_CRYP_AESECB_Encrypt(&hcryp, payload, sizeof(payload), encrypto_payload, 1000);

    auto fs = LittleFS::fs_file_handler(path);
    // Always write full data
    int err = fs.write(encrypto_payload, sizeof(encrypto_payload));
    if (err < 0)
        return err;
    return 0;
}

int PasswordFile::remove(const char* name)
{
    if (name[0] == '\0')
    {
        return -1;
    }

    char path[PWD_NAME_MAX + sizeof(pwd_dir) + sizeof(pwd_suffix)];
    strcpy(path, pwd_dir);
    strcat(path, name);
    strcat(path, pwd_suffix);

    int err = LittleFS::fs_remove(path);

    if (err < 0)
        return err;

    return 0;
}

int PasswordFile::remove()
{
    char path[PWD_NAME_MAX + sizeof(pwd_dir) + sizeof(pwd_suffix)];
    strcpy(path, pwd_dir);
    strcat(path, name);
    strcat(path, pwd_suffix);

    int err = LittleFS::fs_remove(path);

    if (err < 0)
        return err;

    return 0;
}


