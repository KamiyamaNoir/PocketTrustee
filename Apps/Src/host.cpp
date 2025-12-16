#include "host.hpp"
#include "zcbor_common.h"
#include "zcbor_encode.h"
#include "zcbor_decode.h"
#include <cstdio>
#include "aes.h"
#include "simple_buffer.hpp"
#include "crypto_base.hpp"
#include "gui.hpp"
#include "lfs_base.h"
#include "cmsis_os.h"
#include "bsp_rtc.h"
#include "pin_code.hpp"

// CDC Transmit and Receive
extern StaticRingBuffer cdc_receive_ring_buffer;
extern void cdc_acm_data_send(const uint8_t* data, uint8_t len, uint16_t timeout);
// GUI Handler
extern gui::Display gui_main;
extern gui::Window wn_manager_mode;
extern gui::Window wn_manager_respond;
extern void clickon_manager_exit(gui::Window& wn, gui::Display& dis, gui::ui_operation& opt);

extern osThreadId defaultTaskHandle;

// Marcos
#define send(data, len) cdc_acm_data_send((const uint8_t*)data, len, 100)
#define send_msg(msg) send(msg, sizeof(msg) - 1)

#define CHECK_SUCCESS(flag) \
    if (!flag) { \
    return false; }

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
volatile bool in_managermode = false;
char connect_user[24] {};
char manager_user[24] {};
#ifdef DEBUG_ENABLE
char device_name[24] = "debug_xx_device";
#else
char device_name[24] = "PocketTrustee";
#endif

// Local function
static uint16_t polling_for_data(uint8_t* dst, uint16_t size, uint16_t timeout);
static bool check_user(zcbor_state_t* state);
static bool command_connect_req();
static bool invoke_disconnect();
static bool invoke_device_rename(zcbor_state_t* state);

static bool invoke_fswrite(zcbor_state_t* zcbor_state);
static bool invoke_fsread(zcbor_state_t* zcbor_state);
static bool invoke_fs_ls(zcbor_state_t* zcbor_state);
static bool invoke_fs_rm(zcbor_state_t* zcbor_state);
static bool invoke_fs_rename(zcbor_state_t* zcbor_state);
static bool invoke_fs_mkdir(zcbor_state_t* zcbor_state);
static bool invoke_fs_state();

uint8_t hostCommandInvoke(bool from_startup)
{
    if (cdc_receive_ring_buffer.is_empty() || !in_managermode)
        return 0;
    // Decode request
    uint16_t nread = cdc_receive_ring_buffer.read(receive_buffer, HOST_CACHE_SIZE);
    if (nread == 0) return 0;
    zcbor_string zcbor_str = {};
    ZCBOR_STATE_D(zcbor_state, 2, receive_buffer, HOST_CACHE_SIZE, 1, 0);
    bool success = zcbor_map_start_decode(zcbor_state);
    success = success && zcbor_tstr_expect_ptr(zcbor_state, "req", 3);
    CHECK_SUCCESS(success);
    // Get request content
    success = zcbor_tstr_decode(zcbor_state, &zcbor_str);
    CHECK_SUCCESS(success);
    char require_desc[32];
    ZCBOR_TO_CSTRING(zcbor_str, require_desc);
    // Req : Hello -> send back version
    if (IS_COMMAND(require_desc, "hello"))
    {
        char v_str[32] {};
        sprintf(v_str, "trustee:%u.%u.%u", version_c1, version_c2, version_c3);
        send(v_str, strlen(v_str));
        return 1;
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
        return 1;
    }
    // Req : Initialize -> reset the device
    if (IS_COMMAND(require_desc, "initialize") && from_startup)
    {
        // Format all data
        int err = LittleFS::fs_format();
        if (err < 0)
        {
            send_msg("format error");
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
        zcbor_tstr_expect_ptr(zcbor_state, "user", 4);
        zcbor_string usr_str = {};
        zcbor_tstr_decode(zcbor_state, &usr_str);
        ZCBOR_TO_CSTRING(usr_str, manager_user);
        // Register device name
        zcbor_tstr_expect_ptr(zcbor_state, "device", 6);
        zcbor_tstr_decode(zcbor_state, &usr_str);
        ZCBOR_TO_CSTRING(usr_str, device_name);
        // Register pin code
        char pin[6];
        zcbor_tstr_expect_ptr(zcbor_state, "pin", 3);
        zcbor_tstr_decode(zcbor_state, &usr_str);
        ZCBOR_TO_CSTRING(usr_str, pin);
        PIN_CODE::setPinCode(pin);
        // Calibrate RTC
        zcbor_tstr_expect_ptr(zcbor_state, "timestamp", 9);
        uint32_t timestamp;
        zcbor_uint32_decode(zcbor_state, &timestamp);
        //NOLINTNEXTLINE
        rtc::TimeDate dt;
        rtc::UnixToTimedate(timestamp, &dt, TIME_ZONE_OFFSET_Shanghai);
        rtc::setTimedate(&dt);
        // send key
        send(key, 16);
        return 1;
    }
    /* ====== Any other command shall be authenticated before executing ======*/
    static char max_trial = 3;
    if (max_trial < 1)
    {
        // When running out of max_trial, host can not connect to device any more
        return 0;
    }
    if (!from_startup)
    {
        // Req : Disconnect -> turn off host mode
        if (IS_COMMAND(require_desc, "disconnect"))
        {
            invoke_disconnect();
            return 1;
        }
        // Req : connect
        if (IS_COMMAND(require_desc, "connect"))
        {
            manager_user[0] = '\0';
            // Temperarily load user content
            success = zcbor_tstr_expect_ptr(zcbor_state, "user", 4);
            success = success && zcbor_tstr_decode(zcbor_state, &zcbor_str);
            CHECK_SUCCESS(success);
            ZCBOR_TO_CSTRING(zcbor_str, connect_user);
            // Invoke authencator
            success = command_connect_req();
            if (!success)
            {
                max_trial--;
            }
            else
            {
                ZCBOR_TO_CSTRING(zcbor_str, manager_user);
            }
            return success;
        }
    }
    /* ====== All commands below based on trustable connection ======*/
    success = check_user(zcbor_state);
    if (!success)
    {
        manager_user[0] = '\0';
        return 0;
    }
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
    if (IS_COMMAND(require_desc, "su_exit"))
    {
        manager_user[0] = '\0';
        return 2;
    }
    return 0;
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

bool invoke_disconnect()
{
    manager_user[0] = '\0';
    gui_main.switchFocusLag(&wn_manager_mode);
    gui_main.refresh_count = 0;
    GUI_OP_NULL();
    return true;
}

bool check_user(zcbor_state_t* state)
{
    if (manager_user[0] == '\0') return false;
    bool success = zcbor_tstr_expect_ptr(state, "user", 4);
    if (!success) return false;
    zcbor_string usr_str = {};
    zcbor_tstr_decode(state, &usr_str);
    if (memcmp(manager_user, usr_str.value, usr_str.len) != 0)
        return false;
    return true;
}

bool invoke_device_rename(zcbor_state_t* state)
{
    zcbor_string zcbor_str = {};
    bool success = false;
    success = zcbor_tstr_expect_ptr(state, "name", 4);
    success = success && zcbor_tstr_decode(state, &zcbor_str);
    CHECK_SUCCESS(success);
    ZCBOR_TO_CSTRING(zcbor_str, device_name);
    send(zcbor_str.value, zcbor_str.len);
    return true;
}

bool command_connect_req()
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
    if (nread == 0) return false;
    ZCBOR_STATE_D(decode_state, 2, receive_buffer, HOST_CACHE_SIZE, 1, 0);
    zcbor_string response = {};
    zcbor_bstr_decode(decode_state, &response);
    // Check response
    if (response.len != 16)
        return false;
    if (memcmp(expected, response.value, 16) != 0)
        return false;
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
    if (nread == 0) return true;
    bool success;
    zcbor_new_decode_state(decode_state, 4, receive_buffer, HOST_CACHE_SIZE, 1, reinterpret_cast<uint8_t*>(&decode_state[3]), 0);
    success = zcbor_map_start_decode(decode_state);
    success = success && zcbor_tstr_expect_ptr(decode_state, "timestamp", 9);
    if (!success) return true;
    uint32_t timestamp;
    zcbor_uint32_decode(decode_state, &timestamp);
    rtc::TimeDate dt {};
    rtc::UnixToTimedate(timestamp, &dt, TIME_ZONE_OFFSET_Shanghai);
    rtc::setTimedate(&dt);
    // Display Status
    gui_main.switchFocusLag(&wn_manager_respond);
    gui_main.refresh_count = 11;
    GUI_OP_NULL();
    return true;
}

bool invoke_fswrite(zcbor_state_t* zcbor_state)
{
    zcbor_string zcbor_str = {};
    bool success = false;
    char path[32];
    success = zcbor_tstr_expect_ptr(zcbor_state, "path", 4);
    success = success && zcbor_tstr_decode(zcbor_state, &zcbor_str);
    CHECK_SUCCESS(success);
    ZCBOR_TO_CSTRING(zcbor_str, path);
    auto file = LittleFS::fs_file_handler(path);
    send("ok", 2);
    // File transmit begin
    for (;;)
    {
        bool more = false;
        uint16_t fsread = polling_for_data(receive_buffer, HOST_CACHE_SIZE, 1000);
        if (fsread == 0) return false;
        ZCBOR_STATE_D(state, 2, receive_buffer, HOST_CACHE_SIZE, 1, 0);
        success = zcbor_map_start_decode(state);
        success = success && zcbor_tstr_expect_ptr(state, "more", 4);
        success = success && zcbor_bool_decode(state, &more);
        success = success && zcbor_tstr_expect_ptr(state, "data", 4);
        success = success && zcbor_bstr_decode(state, &zcbor_str);
        CHECK_SUCCESS(success);
        auto err = file.write(zcbor_str.value, zcbor_str.len);
        if (err < 0)
        {
            send("error", 5);
            return false;
        }
        send("ok", 2);
        if (!more) break;
    }
    // SHA1 Check
    uint8_t feedback[20] = {};
    cmox_sha1_handle_t sha1;
    cmox_hash_handle_t* hash = cmox_sha1_construct(&sha1);
    cmox_hash_init(hash);
    cmox_hash_setTagLen(hash, 20);
    file.seek(0);
    for (;;)
    {
        uint8_t buffer[128];
        int fsread = file.read(buffer, sizeof(buffer));
        if (fsread < 0)
        {
            send("error", 5);
            return false;
        }
        cmox_hash_append(hash, buffer, fsread);
        if (fsread < 128)
            break;
    }
    size_t computed = 0;
    cmox_hash_generateTag(hash, feedback, &computed);
    if (computed != 20)
    {
        send("error", 5);
        return false;
    }
    send(feedback, 20);
    return true;
}

bool invoke_fsread(zcbor_state_t* zcbor_state)
{
    bool success = false;
    zcbor_string zcbor_str = {};
    char path[64];
    success = zcbor_tstr_expect_ptr(zcbor_state, "path", 4);
    success = success && zcbor_tstr_decode(zcbor_state, &zcbor_str);
    CHECK_SUCCESS(success);
    ZCBOR_TO_CSTRING(zcbor_str, path);
    int err = LFS_ERR_EXIST;
    auto file = LittleFS::fs_file_handler(path, LFS_O_RDONLY, &err);
    if (err != LFS_ERR_OK)
    {
        send("error", 5);
        return false;
    }
    send("ok", 2);
    // Synchronize
    polling_for_data(receive_buffer, HOST_CACHE_SIZE, 1000);
    // File transmit begin
    for (;;)
    {
        constexpr int p_size = 128;
        uint8_t buffer[p_size];
        int fsread = file.read(buffer, p_size);
        if (fsread < 0)
            return false;
        bool more = fsread == p_size;
        ZCBOR_STATE_E(state, 2, payload_buffer, HOST_CACHE_SIZE, 1);
        success = zcbor_map_start_encode(state, 4);
        success = success && zcbor_tstr_encode_ptr(state, "more", 4);
        success = success && zcbor_bool_put(state, more);
        success = success && zcbor_tstr_encode_ptr(state, "data", 4);
        success = success && zcbor_bstr_encode_ptr(state, reinterpret_cast<char*>(buffer), fsread);
        success = success && zcbor_map_end_encode(state, 4);
        CHECK_SUCCESS(success);
        send(payload_buffer, state->payload - payload_buffer);
        // wait for response
        uint16_t nread = polling_for_data(receive_buffer, HOST_CACHE_SIZE, 1000);
        if (nread == 0) return false;
        if (memcmp("ok", receive_buffer, 2) != 0) return false;
        if (!more) break;
    }
    // CRC Check
    // file.seek(0);
    // auto crc = crypto::CRC_Handler();
    // crc.resume();
    // for (;;)
    // {
    //     uint8_t buffer[128];
    //     int fsread = file.read(buffer, sizeof(buffer));
    //     if (fsread < 0) return false;
    //     crc.next(buffer, fsread);
    //     if (fsread < 128)
    //         break;
    // }
    // uint32_t crc_result = crc.result();
    // uint8_t feedback[4] = {
    //     static_cast<uint8_t>(crc_result >> 24),
    //     static_cast<uint8_t>(crc_result >> 16),
    //     static_cast<uint8_t>(crc_result >> 8),
    //     static_cast<uint8_t>(crc_result),
    // };
    // send(feedback, 4);

    // SHA1 Check
    uint8_t feedback[20] = {};
    cmox_sha1_handle_t sha1;
    cmox_hash_handle_t* hash = cmox_sha1_construct(&sha1);
    cmox_hash_init(hash);
    cmox_hash_setTagLen(hash, 20);
    file.seek(0);
    for (;;)
    {
        uint8_t buffer[128];
        int fsread = file.read(buffer, sizeof(buffer));
        if (fsread < 0)
        {
            send("error", 5);
            return false;
        }
        cmox_hash_append(hash, buffer, fsread);
        if (fsread < 128)
            break;
    }
    size_t computed = 0;
    cmox_hash_generateTag(hash, feedback, &computed);
    if (computed != 20)
    {
        send("error", 5);
        return false;
    }
    send(feedback, 20);
    return true;
}

bool invoke_fs_ls(zcbor_state_t* zcbor_state)
{
    zcbor_string zcbor_str = {};
    bool success = false;
    char path[32];
    success = zcbor_tstr_expect_ptr(zcbor_state, "path", 4);
    success = success && zcbor_tstr_decode(zcbor_state, &zcbor_str);
    CHECK_SUCCESS(success);
    ZCBOR_TO_CSTRING(zcbor_str, path);
    auto dir = LittleFS::fs_dir_handler(path);
    char ls_str[256] {};
    int err = dir.list_str(ls_str, sizeof(ls_str));
    if (err < 0)
    {
        sprintf(ls_str, "error:%d", err);
    }
    send(ls_str, strlen(ls_str));
    return true;
}

bool invoke_fs_rm(zcbor_state_t* zcbor_state)
{
    zcbor_string zcbor_str = {};
    bool success = false;
    char path[32];
    success = zcbor_tstr_expect_ptr(zcbor_state, "path", 4);
    success = success && zcbor_tstr_decode(zcbor_state, &zcbor_str);
    CHECK_SUCCESS(success);
    ZCBOR_TO_CSTRING(zcbor_str, path);
    auto err = LittleFS::fs_remove(path);
    if (err < 0)
        send("error", 5);
    else
        send("success", 7);
    return true;
}

bool invoke_fs_rename(zcbor_state_t* zcbor_state)
{
    zcbor_string zcbor_str = {};
    bool success = false;
    char old_path[64];
    char new_path[64];
    success = zcbor_tstr_expect_ptr(zcbor_state, "old", 3);
    success = success && zcbor_tstr_decode(zcbor_state, &zcbor_str);
    CHECK_SUCCESS(success);
    ZCBOR_TO_CSTRING(zcbor_str, old_path);
    success = zcbor_tstr_expect_ptr(zcbor_state, "new", 3);
    success = success && zcbor_tstr_decode(zcbor_state, &zcbor_str);
    CHECK_SUCCESS(success);
    ZCBOR_TO_CSTRING(zcbor_str, new_path);
    auto err = LittleFS::fs_rename(old_path, new_path);
    if (err < 0)
        send("error", 5);
    else
        send("success", 7);
    return true;
}

bool invoke_fs_mkdir(zcbor_state_t* zcbor_state)
{
    zcbor_string zcbor_str = {};
    bool success = false;
    char path[32];
    success = zcbor_tstr_expect_ptr(zcbor_state, "path", 4);
    success = success && zcbor_tstr_decode(zcbor_state, &zcbor_str);
    CHECK_SUCCESS(success);
    ZCBOR_TO_CSTRING(zcbor_str, path);
    auto err = LittleFS::fs_dir_handler::mkdir(path);
    if (err < 0)
        send("error", 5);
    else
        send("success", 7);
    return true;
}

bool invoke_fs_state()
{
    auto err = LittleFS::fs_getUsed();
    if (err == 0)
        send("error", 5);
    else
    {
        char feedback[32];
        sprintf(feedback, "%ld / %ld", err, 512UL);
        send(feedback, strlen(feedback));
    }
    return true;
}



