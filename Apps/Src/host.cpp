#include "host.hpp"
#include "zcbor_common.h"
#include "zcbor_encode.h"
#include "zcbor_decode.h"
#include <cstdio>
#include "aes.h"
#include "simple_buffer.hpp"
#include "crypto_base.hpp"
#include "gui.hpp"
#include "little_fs.hpp"
#include "cmsis_os.h"
#include "bsp_rtc.h"
#include "pin_code.hpp"
#include "gui_manager_mode.hpp"

// CDC Transmit and Receive
extern StaticRingBuffer cdc_receive_ring_buffer;
extern void cdc_acm_data_send(const uint8_t* data, uint8_t len, uint16_t timeout);
// GUI Handler
extern gui::Display gui_main;
extern void clickon_manager_exit(gui::Window& wn, gui::Display& dis, gui::ui_operation& opt);

// Marcos
#define send(data, len) cdc_acm_data_send((const uint8_t*)data, len, 100)
#define send_msg(msg) send(msg, sizeof(msg) - 1)

#define CHECK_SUCCESS(flag) \
    if (!flag) { \
    return {.err = __LINE__, .err_fs = LFS_ERR_OK, .msg = "unknown"}; }

#define ZCBOR_TO_CSTRING(zcbor, str) \
    do {\
    memcpy(str, zcbor.value, zcbor.len); \
    str[zcbor.len] = '\0'; \
    }while(0)

#define IS_COMMAND(from, expect) (memcmp(expect, from, sizeof(expect)-1) == 0)

// Cache size, which determines the maximum decoding and encoding size
#define HOST_CACHE_SIZE 512
static uint8_t receive_buffer[HOST_CACHE_SIZE];
static uint8_t payload_buffer[HOST_CACHE_SIZE];

// Export variables, showing the status of host
static char connect_user[24] {};
static char manager_user[24] {};

#ifdef DEBUG_ENABLE
static char device_name[24] = "debug_xx_device";
#else
static char device_name[24] = "PocketTrustee";
#endif

// Local function
static uint16_t polling_for_data(uint8_t* dst, uint16_t size, uint16_t timeout);

static PKT_ERR command_invoke(bool from_startup);

static PKT_ERR check_user(zcbor_state_t* state);
static PKT_ERR command_connect_req();
static PKT_ERR invoke_disconnect();
static PKT_ERR invoke_device_rename(zcbor_state_t* state);

static PKT_ERR invoke_fswrite(zcbor_state_t* zcbor_state);
static PKT_ERR invoke_fsread(zcbor_state_t* zcbor_state);
static PKT_ERR invoke_fs_ls(zcbor_state_t* zcbor_state);
static PKT_ERR invoke_fs_rm(zcbor_state_t* zcbor_state);
static PKT_ERR invoke_fs_rename(zcbor_state_t* zcbor_state);
static PKT_ERR invoke_fs_mkdir(zcbor_state_t* zcbor_state);
static PKT_ERR invoke_fs_state();

PKT_ERR Host::hostCommandInvoke(bool from_startup)
{
    auto err = command_invoke(from_startup);
    if (err.err != 0)
    {
        char report[128];
        int len = sprintf(report, "error\nERROR:%d\nFS:%d\nMSG:%s", err.err, err.err_fs, err.msg);
        if (len < 0 || len > sizeof(report)) return {
            .err = __LINE__,
            .err_fs = LFS_ERR_OK,
            .msg = "fatial error"
        };
        send(report, len);
    }
    return err;
}

char* Host::getConnectUser()
{
    return connect_user;
}

char* Host::getHostName()
{
    return manager_user;
}

char* Host::getDeviceName()
{
    return device_name;
}

static PKT_ERR command_invoke(bool from_startup)
{
    if (cdc_receive_ring_buffer.is_empty()) return {
        .err = 0,
        .err_fs = LFS_ERR_OK,
        .msg = nullptr
    };

    // Decode request
    uint16_t nread = cdc_receive_ring_buffer.read(receive_buffer, HOST_CACHE_SIZE);
    if (nread == 0) return {
        .err = 0,
        .err_fs = LFS_ERR_OK,
        .msg = nullptr
    };

    zcbor_string zcbor_str = {
        .value = nullptr,
        .len = 0,
    };

    char require_desc[32];

    ZCBOR_STATE_D(zcbor_state, 2, receive_buffer, HOST_CACHE_SIZE, 1, 0);
    bool success = zcbor_map_start_decode(zcbor_state);
    success = success && zcbor_tstr_expect_ptr(zcbor_state, "req", 3);
    success = success && zcbor_tstr_decode(zcbor_state, &zcbor_str);

    if (!success || zcbor_str.len > sizeof(require_desc)) return {
        .err = __LINE__,
        .err_fs = LFS_ERR_OK,
        .msg = "err load req"
    };

    ZCBOR_TO_CSTRING(zcbor_str, require_desc);

    // Req : Hello -> send back version
    if (IS_COMMAND(require_desc, "hello"))
    {
        char v_str[32] {};
        sprintf(v_str, "trustee:%u.%u.%u", version_c1, version_c2, version_c3);
        send(v_str, strlen(v_str));
        return {
            .err = 0,
            .err_fs = LFS_ERR_OK,
            .msg = nullptr
        };
    }

    // Req : reset
    if (IS_COMMAND(require_desc, "reset"))
    {
        if (!from_startup)
        {
            send_msg("reset");
            __set_FAULTMASK(1);
            NVIC_SystemReset();
        }
        send_msg("ok");
        return {
            .err = 0,
            .err_fs = LFS_ERR_OK,
            .msg = nullptr
        };
    }

    // Req : Initialize -> reset the device
    if (IS_COMMAND(require_desc, "initialize") && from_startup)
    {
        // Format all data
        int err = LittleFS_W25Q16::Format();

        if (err < 0)
        {
            send_msg("error\nformat error");
            __set_FAULTMASK(1);
            NVIC_SystemReset();
        }

        // Set AES key
        auto* key = new uint8_t[16];
        for (int i = 0; i < 16; i++)
        {
            key[i] = TRNG();
        }
        hcryp.Init.pKey = key;

        // Register manager user
        success = zcbor_tstr_expect_ptr(zcbor_state, "user", 4);
        zcbor_string usr_str = {
            .value = nullptr,
            .len = 0
        };
        success = success && zcbor_tstr_decode(zcbor_state, &usr_str);
        if (!success || usr_str.len > sizeof(manager_user)) return {
            .err = __LINE__,
            .err_fs = LFS_ERR_OK,
            .msg = "err load usr"
        };
        ZCBOR_TO_CSTRING(usr_str, manager_user);

        // Register device name
        success = zcbor_tstr_expect_ptr(zcbor_state, "device", 6);
        success = success && zcbor_tstr_decode(zcbor_state, &usr_str);
        if (!success || usr_str.len > sizeof(device_name)) return {
            .err = __LINE__,
            .err_fs = LFS_ERR_OK,
            .msg = "err load device name"
        };
        ZCBOR_TO_CSTRING(usr_str, device_name);

        // Register pin code
        char pin[6];
        success = zcbor_tstr_expect_ptr(zcbor_state, "pin", 3);
        success = success && zcbor_tstr_decode(zcbor_state, &usr_str);
        if (!success || usr_str.len != sizeof(pin)) return {
            .err = __LINE__,
            .err_fs = LFS_ERR_OK,
            .msg = "err load pin"
        };
        ZCBOR_TO_CSTRING(usr_str, pin);
        PIN_CODE::setPinCode(pin);

        // Calibrate RTC
        uint32_t timestamp;
        success = zcbor_tstr_expect_ptr(zcbor_state, "timestamp", 9);
        success = success && zcbor_uint32_decode(zcbor_state, &timestamp);
        if (!success) return {
            .err = __LINE__,
            .err_fs = LFS_ERR_OK,
            .msg = "err load timestamp"
        };
        rtc::TimeDate dt {};
        rtc::UnixToTimedate(timestamp, &dt, TIME_ZONE_OFFSET_Shanghai);
        rtc::setTimedate(&dt);

        // send key
        send(key, 16);

        return {
            .err = 0,
            .err_fs = 1,
            .msg = nullptr
        };
    }

    /* ====== Any other command shall be authenticated before executing ======*/
    if (!from_startup)
    {
        // Req : Disconnect -> turn off host mode
        if (IS_COMMAND(require_desc, "disconnect"))
        {
            invoke_disconnect();
            return {
                .err = 0,
                .err_fs = LFS_ERR_OK,
                .msg = nullptr
            };
        }
        // Req : connect
        if (IS_COMMAND(require_desc, "connect"))
        {
            memset(connect_user, '\0', sizeof(connect_user));
            memset(manager_user, '\0', sizeof(manager_user));
            // Temperarily load user content
            success = zcbor_tstr_expect_ptr(zcbor_state, "user", 4);
            success = success && zcbor_tstr_decode(zcbor_state, &zcbor_str);

            if (!success) return {
                .err = __LINE__,
                .err_fs = LFS_ERR_OK,
                .msg = "err load user"
            };

            ZCBOR_TO_CSTRING(zcbor_str, connect_user);

            // Invoke authencator
            auto err = command_connect_req();
            if (err.err == 0)
            {
                ZCBOR_TO_CSTRING(zcbor_str, manager_user);
            }
            return err;
        }
    }

    /* ====== All commands below based on trustable connection ======*/
    auto err = check_user(zcbor_state);
    if (err.err != 0) return err;

    if (IS_COMMAND(require_desc, "device-rename"))
    {
        return invoke_device_rename(zcbor_state);
    }

    /* ====== Super Debug Command ======*/
    if (IS_COMMAND(require_desc, "su_ls"))
    {
        return invoke_fs_ls(zcbor_state);
    }
    if (IS_COMMAND(require_desc, "su_rm"))
    {
        return invoke_fs_rm(zcbor_state);
    }
    if (IS_COMMAND(require_desc, "su_rename"))
    {
        return invoke_fs_rename(zcbor_state);
    }
    if (IS_COMMAND(require_desc, "su_write"))
    {
        return invoke_fswrite(zcbor_state);
    }
    if (IS_COMMAND(require_desc, "su_read"))
    {
        return invoke_fsread(zcbor_state);
    }
    if (IS_COMMAND(require_desc, "su_mkdir"))
    {
        return invoke_fs_mkdir(zcbor_state);
    }
    if (IS_COMMAND(require_desc, "su_state"))
    {
        return invoke_fs_state();
    }

    return {
        .err = 0,
        .err_fs = LFS_ERR_OK,
        .msg = nullptr
    };
}


static uint16_t polling_for_data(uint8_t* dst, uint16_t size, uint16_t timeout)
{
    while (--timeout)
    {
        if (!cdc_receive_ring_buffer.is_empty() && cdc_receive_ring_buffer.is_eof())
        {
            return cdc_receive_ring_buffer.read(dst, size);
        }
        HAL_Delay(1);
    }
    if (!cdc_receive_ring_buffer.is_empty())
    {
        // clear EOF
        return cdc_receive_ring_buffer.read(dst, size);
    }
    return 0;
}

PKT_ERR invoke_disconnect()
{
    memset(manager_user, '\0', sizeof(manager_user));
    gui_main.switchFocusLag(&wn_manager_mode);
    gui_main.refresh_count = 0;
    GUI_OP_NULL();
    return {
        .err = 0,
        .err_fs = LFS_ERR_OK,
        .msg = nullptr
    };
}

PKT_ERR check_user(zcbor_state_t* state)
{
    zcbor_string usr_str = {
        .value = nullptr,
        .len = 0
    };
    bool success = zcbor_tstr_expect_ptr(state, "user", 4);
    success = success && zcbor_tstr_decode(state, &usr_str);
    if (!success || usr_str.len > sizeof(manager_user)) return {
        .err = __LINE__,
        .err_fs = LFS_ERR_OK,
        .msg = "err load user"
    };

    if (memcmp(manager_user, usr_str.value, usr_str.len) != 0)
        return {.err = __LINE__,.err_fs = LFS_ERR_OK,.msg="err usr"};

    return {.err = 0,.err_fs = LFS_ERR_OK,.msg=nullptr};
}

PKT_ERR invoke_device_rename(zcbor_state_t* state)
{
    zcbor_string zcbor_str = {
        .value = nullptr,
        .len = 0,
    };

    bool success = zcbor_tstr_expect_ptr(state, "name", 4);
    success = success && zcbor_tstr_decode(state, &zcbor_str);

    if (!success || zcbor_str.len > sizeof(device_name)) return {
        .err = __LINE__,
        .err_fs = LFS_ERR_OK,
        .msg = "err load name"
    };

    ZCBOR_TO_CSTRING(zcbor_str, device_name);

    send(zcbor_str.value, zcbor_str.len);

    return {.err = 0,.err_fs = LFS_ERR_OK,.msg=nullptr};
}

PKT_ERR command_connect_req()
{
    // Generate challenge
    uint8_t challenge[16];
    uint8_t expected[16];
    for (auto& i : expected)
        i = TRNG();
    HAL_CRYP_AESECB_Encrypt(&hcryp, expected, 16, challenge, 1000);
    ZCBOR_STATE_E(encode_state, 2, payload_buffer, HOST_CACHE_SIZE, 1);
    zcbor_map_start_encode(encode_state, 4);
    zcbor_tstr_encode_ptr(encode_state, "device", 6);
    zcbor_tstr_encode_ptr(encode_state, device_name, strlen(device_name));
    zcbor_tstr_encode_ptr(encode_state, "challenge", 9);
    zcbor_bstr_encode_ptr(encode_state, reinterpret_cast<char*>(challenge), 16);
    zcbor_map_end_encode(encode_state, 4);
    send(payload_buffer, encode_state->payload - payload_buffer);

    // Polling for feedback
    uint16_t nread = polling_for_data(receive_buffer, HOST_CACHE_SIZE, 1000);
    if (nread == 0) return {
        .err = __LINE__,
        .err_fs = LFS_ERR_OK,
        .msg = "host timeout"
    };
    ZCBOR_STATE_D(decode_state, 2, receive_buffer, HOST_CACHE_SIZE, 1, 0);
    zcbor_string response = {
        .value = nullptr,
        .len = 0,
    };
    zcbor_bstr_decode(decode_state, &response);
    // Check response
    if (response.len != 16)
        return {
            .err = __LINE__,
            .err_fs = LFS_ERR_OK,
            .msg = "host resp err"
        };
    if (memcmp(expected, response.value, 16) != 0)
        return {
            .err = __LINE__,
            .err_fs = LFS_ERR_OK,
            .msg = "mismatch"
        };

    // Send back success message
    zcbor_new_encode_state(encode_state, 4, payload_buffer, HOST_CACHE_SIZE, 1);
    zcbor_map_start_encode(encode_state, 4);
    zcbor_tstr_encode_ptr(encode_state, "success", 7);
    zcbor_bool_put(encode_state, true);
    zcbor_tstr_encode_ptr(encode_state, "device", 6);
    zcbor_tstr_encode_ptr(encode_state, device_name, strlen(device_name));
    zcbor_map_end_encode(encode_state, 4);
    send(payload_buffer, encode_state->payload - payload_buffer);

    // Polling for timestamp caliberation
    nread = polling_for_data(receive_buffer, HOST_CACHE_SIZE, 1000);
    if (nread == 0) return {
        .err = __LINE__,
        .err_fs = LFS_ERR_OK,
        .msg = "host timeout"
    };

    zcbor_new_decode_state(decode_state, 4, receive_buffer, HOST_CACHE_SIZE, 1, reinterpret_cast<uint8_t*>(&decode_state[3]), 0);
    bool success = zcbor_map_start_decode(decode_state);
    success = success && zcbor_tstr_expect_ptr(decode_state, "timestamp", 9);
    if (!success) return {
        .err = __LINE__,
        .err_fs = LFS_ERR_OK,
        .msg = "timestamp err"
    };

    uint32_t timestamp;
    zcbor_uint32_decode(decode_state, &timestamp);
    rtc::TimeDate dt {};
    rtc::UnixToTimedate(timestamp, &dt, TIME_ZONE_OFFSET_Shanghai);
    rtc::setTimedate(&dt);

    // Display Status
    gui_main.switchFocusLag(&wn_manager_respond);
    gui_main.refresh_count = 11;
    GUI_OP_NULL();
    return {.err = 0,.err_fs = LFS_ERR_OK,.msg=nullptr};
}

PKT_ERR invoke_fswrite(zcbor_state_t* zcbor_state)
{
    zcbor_string zcbor_str = {
        .value = nullptr,
        .len = 0,
    };

    bool success = false;
    char path[64];

    success = zcbor_tstr_expect_ptr(zcbor_state, "path", 4);
    success = success && zcbor_tstr_decode(zcbor_state, &zcbor_str);

    if (!success || zcbor_str.len > sizeof(path)) return {
        .err = __LINE__,
        .err_fs = LFS_ERR_OK,
        .msg = "path load err"
    };

    ZCBOR_TO_CSTRING(zcbor_str, path);

    uint8_t file_buffer[128];
    lfs_file_config open_cfg = {
        .buffer = file_buffer,
    };
    FileDelegate file;
    int err = file.open(path, LFS_O_RDWR | LFS_O_CREAT, &open_cfg);

    if (err < 0) return {
        .err = __LINE__,
        .err_fs = err,
        .msg = "fail to open file"
    };

    // SHA1 Check
    uint8_t feedback[20];
    cmox_sha1_handle_t sha1;
    cmox_hash_handle_t* hash = cmox_sha1_construct(&sha1);
    cmox_hash_init(hash);
    cmox_hash_setTagLen(hash, 20);

    // Synchronize
    send_msg("ok");
    polling_for_data(receive_buffer, HOST_CACHE_SIZE, 1000);

    // File transmit begin
    for (;;)
    {
        bool more = false;
        uint16_t fsread = polling_for_data(receive_buffer, HOST_CACHE_SIZE, 1000);

        if (fsread == 0) return {
            .err = __LINE__,
            .err_fs = LFS_ERR_OK,
            .msg = "host timeout"
        };

        ZCBOR_STATE_D(state, 2, receive_buffer, HOST_CACHE_SIZE, 1, 0);
        success = zcbor_map_start_decode(state);
        success = success && zcbor_tstr_expect_ptr(state, "more", 4);
        success = success && zcbor_bool_decode(state, &more);
        success = success && zcbor_tstr_expect_ptr(state, "data", 4);
        success = success && zcbor_bstr_decode(state, &zcbor_str);

        if (!success) return {
            .err = __LINE__,
            .err_fs = LFS_ERR_OK,
            .msg = "err decode data"
        };

        auto wr_err = lfs_file_write(&fs_w25q16, &file.instance, zcbor_str.value, zcbor_str.len);

        if (wr_err < 0) return {
            .err = __LINE__,
            .err_fs = wr_err,
            .msg = "fail to write file"
        };

        cmox_hash_append(hash, zcbor_str.value, zcbor_str.len);

        send_msg("ok");

        if (!more) break;
    }

    size_t computed = 0;
    cmox_hash_generateTag(hash, feedback, &computed);

    if (computed != 20) return {
        .err = __LINE__,
        .err_fs = LFS_ERR_OK,
        .msg = "hash calc err"
    };

    send(feedback, 20);

    return {
        .err = 0,
        .err_fs = LFS_ERR_OK,
        .msg = nullptr
    };
}

PKT_ERR invoke_fsread(zcbor_state_t* zcbor_state)
{
    bool success = false;
    zcbor_string zcbor_str = {
        .value = nullptr,
        .len = 0,
    };

    char path[64];
    success = zcbor_tstr_expect_ptr(zcbor_state, "path", 4);
    success = success && zcbor_tstr_decode(zcbor_state, &zcbor_str);

    if (!success || zcbor_str.len > sizeof(path)) return {
        .err = __LINE__,
        .err_fs = LFS_ERR_OK,
        .msg = "path load err"
    };

    ZCBOR_TO_CSTRING(zcbor_str, path);

    uint8_t file_buffer[128];
    lfs_file_config open_cfg = {
        .buffer = file_buffer,
    };
    FileDelegate file;
    int err = file.open(path, LFS_O_RDONLY, &open_cfg);

    if (err < 0) return {
        .err = __LINE__,
        .err_fs = err,
        .msg = "fail to open file"
    };

    // SHA1 Check
    uint8_t feedback[20] = {};
    cmox_sha1_handle_t sha1;
    cmox_hash_handle_t* hash = cmox_sha1_construct(&sha1);
    cmox_hash_init(hash);
    cmox_hash_setTagLen(hash, 20);

    // Synchronize
    send_msg("ok");
    polling_for_data(receive_buffer, HOST_CACHE_SIZE, 1000);

    // File transmit begin
    for (;;)
    {
        constexpr int p_size = 128;
        uint8_t buffer[p_size];

        int fsread = lfs_file_read(&fs_w25q16, &file.instance, buffer, p_size);

        if (fsread < 0) return {
            .err = __LINE__,
            .err_fs = fsread,
            .msg = "fatial err reading file"
        };

        bool aval = fsread != 0;

        ZCBOR_STATE_E(state, 2, payload_buffer, HOST_CACHE_SIZE, 1);
        success = zcbor_map_start_encode(state, 4);

        success = success && zcbor_tstr_encode_ptr(state, "aval", 4);
        success = success && zcbor_bool_put(state, aval);

        if (aval)
        {
            success = success && zcbor_tstr_encode_ptr(state, "data", 4);
            success = success && zcbor_bstr_encode_ptr(state, reinterpret_cast<char*>(buffer), fsread);

            cmox_hash_append(hash, buffer, fsread);
        }

        success = success && zcbor_map_end_encode(state, 4);

        if (!success) return {
            .err = __LINE__,
            .err_fs = LFS_ERR_OK,
            .msg = "encode err"
        };

        send(payload_buffer, state->payload - payload_buffer);

        // wait for response
        uint16_t nread = polling_for_data(receive_buffer, HOST_CACHE_SIZE, 1000);
        if (nread == 0) return {
            .err = __LINE__,
            .err_fs = LFS_ERR_OK,
            .msg = "host timeout"
        };

        if (memcmp("ok", receive_buffer, 2) != 0) return {
            .err = __LINE__,
            .err_fs = LFS_ERR_OK,
            .msg = "host error"
        };

        if (!aval) break;
    }

    size_t computed = 0;
    cmox_hash_generateTag(hash, feedback, &computed);

    if (computed != 20) return {
        .err = __LINE__,
        .err_fs = LFS_ERR_OK,
        .msg = "hash calc err"
    };

    send(feedback, 20);

    return {
        .err = 0,
        .err_fs = LFS_ERR_OK,
        .msg = nullptr
    };
}

PKT_ERR invoke_fs_ls(zcbor_state_t* zcbor_state)
{
    zcbor_string zcbor_str = {
        .value = nullptr,
        .len = 0
    };

    bool success = false;
    char path[64];
    success = zcbor_tstr_expect_ptr(zcbor_state, "path", 4);
    success = success && zcbor_tstr_decode(zcbor_state, &zcbor_str);

    if (!success || zcbor_str.len > sizeof(path))
    {
        return {
            .err = __LINE__,
            .err_fs = LFS_ERR_OK,
            .msg = "path load err"
        };
    }

    ZCBOR_TO_CSTRING(zcbor_str, path);

    DirectoryDelegate dir;

    int err = dir.open(path);

    if (err < 0) return {
        .err = __LINE__,
        .err_fs = err,
        .msg = "fail open dir"
    };

    for (;;)
    {
        lfs_info info {};
        int read_err = lfs_dir_read(&fs_w25q16, &dir.instance, &info);
        if (read_err < 0) return {
            .err = __LINE__,
            .err_fs = read_err,
            .msg = "err reading dir"
        };

        char readback[64];
        char desc = 'F';

        if (info.type == LFS_TYPE_DIR)
            desc = 'D';

        int print_err = sprintf(readback, "%s %c %lu\n", info.name, desc, info.size);

        if (print_err < 0 || print_err > sizeof(readback)) return {
            .err = __LINE__,
            .err_fs = LFS_ERR_OK,
            .msg = "sprintf fatial err"
        };

        send(readback, print_err);

        if (read_err == 0)
        {
            send_msg("endl");
            break;
        }
    }

    return {
        .err = 0,
        .err_fs = LFS_ERR_OK,
        .msg = nullptr
    };
}

PKT_ERR invoke_fs_rm(zcbor_state_t* zcbor_state)
{
    zcbor_string zcbor_str = {
        .value = nullptr,
        .len = 0
    };

    bool success = false;
    char path[64];

    success = zcbor_tstr_expect_ptr(zcbor_state, "path", 4);
    success = success && zcbor_tstr_decode(zcbor_state, &zcbor_str);

    if (!success || zcbor_str.len > sizeof(path))
    {
        return {
            .err = __LINE__,
            .err_fs = LFS_ERR_OK,
            .msg = "path load err"
        };
    }

    ZCBOR_TO_CSTRING(zcbor_str, path);

    auto err = lfs_remove(&fs_w25q16, path);
    if (err < 0)
        return {
            .err = __LINE__,
            .err_fs = err,
            .msg = "rename err"
        };

    send_msg("success");

    return {
        .err = 0,
        .err_fs = LFS_ERR_OK,
        .msg = nullptr
    };
}

PKT_ERR invoke_fs_rename(zcbor_state_t* zcbor_state)
{
    zcbor_string zcbor_str = {
        .value = nullptr,
        .len = 0
    };

    bool success = false;
    char old_path[64];
    char new_path[64];

    success = zcbor_tstr_expect_ptr(zcbor_state, "old", 3);
    success = success && zcbor_tstr_decode(zcbor_state, &zcbor_str);

    if (!success || zcbor_str.len > sizeof(old_path))
    {
        return {
            .err = __LINE__,
            .err_fs = LFS_ERR_OK,
            .msg = "old path load err"
        };
    }

    ZCBOR_TO_CSTRING(zcbor_str, old_path);

    success = zcbor_tstr_expect_ptr(zcbor_state, "new", 3);
    success = success && zcbor_tstr_decode(zcbor_state, &zcbor_str);

    if (!success || zcbor_str.len > sizeof(new_path))
    {
        return {
            .err = __LINE__,
            .err_fs = LFS_ERR_OK,
            .msg = "new path load err"
        };
    }

    ZCBOR_TO_CSTRING(zcbor_str, new_path);

    auto err = lfs_rename(&fs_w25q16, old_path, new_path);
    if (err < 0)
        return {
            .err = __LINE__,
            .err_fs = err,
            .msg = "rename err"
        };

    send_msg("success");

    return {
        .err = 0,
        .err_fs = LFS_ERR_OK,
        .msg = nullptr
    };
}

PKT_ERR invoke_fs_mkdir(zcbor_state_t* zcbor_state)
{
    zcbor_string zcbor_str = {
        .value = nullptr,
        .len = 0,
    };

    bool success = false;
    char path[64];
    success = zcbor_tstr_expect_ptr(zcbor_state, "path", 4);
    success = success && zcbor_tstr_decode(zcbor_state, &zcbor_str);

    if (!success || zcbor_str.len > sizeof(path))
    {
        return {
            .err = __LINE__,
            .err_fs = LFS_ERR_OK,
            .msg = "path load err"
        };
    }

    ZCBOR_TO_CSTRING(zcbor_str, path);

    auto err = lfs_mkdir(&fs_w25q16, path);

    if (err < 0)
        return {
            .err = __LINE__,
            .err_fs = err,
            .msg = "mkdir err"
        };
    send_msg("success");
    return {
        .err = 0,
        .err_fs = LFS_ERR_OK,
        .msg = nullptr
    };
}

PKT_ERR invoke_fs_state()
{
    auto err = lfs_fs_size(&fs_w25q16);

    if (err < 0)
        return {
            .err = __LINE__,
            .err_fs = err,
            .msg = "fs size read failed"
        };

    char feedback[32];
    sprintf(feedback, "%ld / %ld", err, 512UL);
    send(feedback, strlen(feedback));

    return {
        .err = 0,
        .err_fs = LFS_ERR_OK,
        .msg = nullptr
    };
}



