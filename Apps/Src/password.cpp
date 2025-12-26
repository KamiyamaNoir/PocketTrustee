#include "password.hpp"
#include "little_fs.hpp"
#include "zcbor_common.h"
#include "zcbor_decode.h"
#include "zcbor_encode.h"
#include "aes.h"
#include <cstring>

PKT_ERR PasswordFile::load(const char* pwd_name)
{
    if (pwd_name == nullptr || pwd_name[0] == '\0')
    {
        return {
            .err = -1,
            .err_fs = LFS_ERR_OK,
            .msg = "nullptr to pwd file"
        };
    }

    if (strlen(pwd_name) > PWD_NAME_MAX)
    {
        return {
            .err = -1,
            .err_fs = LFS_ERR_OK,
            .msg = "pwd file name too long"
        };
    }

    strcpy(this->name, pwd_name);

    char path[PWD_NAME_MAX + sizeof(pwd_dir) + sizeof(pwd_suffix)];
    strcpy(path, pwd_dir);
    strcat(path, pwd_name);
    strcat(path, pwd_suffix);

    uint8_t file_buffer[128];
    lfs_file_config open_cfg = {
        .buffer = file_buffer,
    };
    uint8_t fbytes_encrypto[PWD_FILE_SIZE_MAX];
    uint8_t fbytes_plaintext[PWD_FILE_SIZE_MAX];

    FileDelegate file;
    int err = file.open(path, LFS_O_RDONLY, &open_cfg);
    if (err < 0)
        return {
            .err = -1,
            .err_fs = err,
            .msg = "fail to open pwd file"
        };

    err = lfs_file_read(&fs_w25q16, &file.instance, fbytes_encrypto, PWD_FILE_SIZE_MAX);
    if (err < 0)
        return {
            .err = -1,
            .err_fs = err,
            .msg = "fail to read pwd file"
        };
    if (err == PWD_FILE_SIZE_MAX)
        return {
            .err = -1,
            .err_fs = LFS_ERR_OK,
            .msg = "bad pwd file"
        };

    auto status = HAL_CRYP_AESECB_Decrypt(&hcryp, fbytes_encrypto, err, fbytes_plaintext, 1000);
    if (status != HAL_OK)
        return {
            .err = status,
            .err_fs = LFS_ERR_OK,
            .msg = "fail to decrypt pwd file"
        };

    zcbor_string zcbor_str = {
        .value = nullptr,
        .len = 0,
    };
    bool success;
    ZCBOR_STATE_D(state, 2, fbytes_plaintext, sizeof(fbytes_plaintext), 1, 0);
    success = zcbor_list_start_decode(state);
    // Decode account
    success = success && zcbor_tstr_decode(state, &zcbor_str);
    if (!success || zcbor_str.len > PWD_ACCOUNT_MAX)
        return {
            .err = __LINE__,
            .err_fs = LFS_ERR_OK,
            .msg = "Bad pwd account"
        };
    memcpy(this->account, zcbor_str.value, zcbor_str.len);
    this->account[zcbor_str.len] = '\0';
    // Decode password
    success = zcbor_tstr_decode(state, &zcbor_str);
    if (!success || zcbor_str.len > PWD_PWD_MAX)
        return {
            .err = __LINE__,
            .err_fs = LFS_ERR_OK,
            .msg = "Bad pwd password"
        };
    memcpy(this->pwd, zcbor_str.value, zcbor_str.len);
    this->pwd[zcbor_str.len] = '\0';
    return {
        .err = 0,
        .err_fs = LFS_ERR_OK,
        .msg = nullptr
    };
}

PKT_ERR PasswordFile::save()
{
    if (name[0] == '\0' || strlen(name) > PWD_NAME_MAX)
    {
        return {
            .err = __LINE__,
            .err_fs = LFS_ERR_OK,
            .msg = "pwd save bad name"
        };
    }

    if (account[0] == '\0' || strlen(account) > PWD_ACCOUNT_MAX)
    {
        return {
            .err = __LINE__,
            .err_fs = LFS_ERR_OK,
            .msg = "pwd save bad account"
        };
    }

    if (pwd[0] == '\0' || strlen(pwd) > PWD_PWD_MAX)
    {
        return {
            .err = __LINE__,
            .err_fs = LFS_ERR_OK,
            .msg = "pwd save bad pwd"
        };
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

    auto status = HAL_CRYP_AESECB_Encrypt(&hcryp, payload, sizeof(payload), encrypto_payload, 1000);

    if (status != HAL_OK)
        return {
            .err = status,
            .err_fs = LFS_ERR_OK,
            .msg = "pwd encryption failed"
        };

    lfs_file_config open_cfg = {
        .buffer = payload,
    };
    FileDelegate file;
    int err = file.open(path, LFS_O_CREAT | LFS_O_WRONLY, &open_cfg);
    if (err < 0)
        return {
            .err = __LINE__,
            .err_fs = err,
            .msg = "pwd file open failed"
        };

    err = lfs_file_write(&fs_w25q16, &file.instance, encrypto_payload, state->payload - payload);

    if (err < 0)
        return {
            .err = __LINE__,
            .err_fs = err,
            .msg = "pwd file write failed"
        };

    return {
        .err = 0,
        .err_fs = LFS_ERR_OK,
        .msg = nullptr
    };
}

PKT_ERR PasswordFile::remove(const char* name)
{
    if (name == nullptr || strlen(name) > PWD_NAME_MAX)
        return {
            .err = __LINE__,
            .err_fs = LFS_ERR_OK,
            .msg = "pwd bad name"
        };

    char path[PWD_NAME_MAX + sizeof(pwd_dir) + sizeof(pwd_suffix)];
    strcpy(path, pwd_dir);
    strcat(path, name);
    strcat(path, pwd_suffix);

    int err = lfs_remove(&fs_w25q16, path);

    if (err < 0)
        return {
            .err = __LINE__,
            .err_fs = err,
            .msg = "pwd remove failed"
        };

    return {
        .err = 0,
        .err_fs = LFS_ERR_OK,
        .msg = nullptr
    };
}

