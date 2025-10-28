#include "crypto_base.h"
#include "cmox_crypto.h"
#include "crc.h"
#include <cstring>
#include "bsp_rtc.h"

using namespace crypto;

#define DIGITS "0123456789"
#define UPPER_CASE "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define LOWER_CASE "abcdefghijklmnopqrstuvwxyz"
#define SPECIAL_CHARS "!@#$%^&*()_-+=[]{}|;:,.<>?`~"

CRC_Handler::CRC_Handler()
{
    crc_result = 0xFFFFFFFF;
    retval = CRYPTO_FAILED;
}

CRC_Handler& CRC_Handler::resume()
{
    hcrc.Instance->INIT = crc_result;
    // when reset, the value in register INIT will be written in DR
    __HAL_CRC_DR_RESET(&hcrc);
    return *this;
}

CRC_Handler& CRC_Handler::next(uint8_t* buffer, uint16_t length)
{
    crc_result = HAL_CRC_Accumulate(&hcrc, reinterpret_cast<uint32_t*>(buffer), length);
    return *this;
}

CRC_Handler& CRC_Handler::reset()
{
    crc_result = 0xFFFFFFFF;
    return resume();
}

uint32_t CRC_Handler::result() const
{
    return crc_result ^ 0xFFFFFFFF;
}

// CRYPTO_RESULT crypto::sha256(const uint8_t* message, uint8_t* sha, const uint16_t msg_len)
// {
//     size_t compute_size;
//     cmox_hash_retval_t cmox_ret = cmox_hash_compute(
//         CMOX_SHA256_ALGO,
//         message,
//         msg_len,
//         sha,
//         CMOX_SHA256_SIZE,
//         &compute_size);
//     if (cmox_ret != CMOX_HASH_SUCCESS || compute_size != CMOX_SHA256_SIZE)
//     {
//         return CRYPTO_FAILED;
//     }
//     return CRYPTO_SUCCESS;
// }

CRYPTO_RESULT crypto::skeygen(const KeygenConfiguration* cfg, char* dst)
{
    uint8_t chars_len = 0;
    char char_map[91] = "";
    if (cfg->containNumber)
    {
        chars_len += sizeof(DIGITS) - 1;
        strcat(char_map, DIGITS);
    }
    if (cfg->containLowercase)
    {
        chars_len += sizeof(LOWER_CASE) - 1;
        strcat(char_map, LOWER_CASE);
    }
    if (cfg->containUppercase)
    {
        chars_len += sizeof(UPPER_CASE) - 1;
        strcat(char_map, UPPER_CASE);
    }
    if (cfg->containSChar)
    {
        chars_len += sizeof(SPECIAL_CHARS) - 1;
        strcat(char_map, SPECIAL_CHARS);
    }
    if (chars_len == 0) return CRYPTO_FAILED;
    for (uint8_t i = 0; i < cfg->length; i++)
    {
        dst[i] = char_map[TRNG() % chars_len];
    }
    dst[cfg->length] = '\0';
    return CRYPTO_SUCCESS;
}

CRYPTO_RESULT crypto::totp_calculate(uint8_t* key, uint8_t key_length, uint32_t* result, uint8_t step, uint32_t mask)
{
    rtc::UnixTime timestamp = 0;
    rtc::TimeDate dt {};
    rtc::getTimedate(&dt);
    rtc::TimedateToUnix(&dt, &timestamp, TIME_ZONE_OFFSET_Shanghai);
    timestamp /= step;
    // Calculate HMAC
    uint8_t message[8] = {
        0,0,0,0,
        static_cast<uint8_t>(timestamp >> 24),
        static_cast<uint8_t>(timestamp >> 16),
        static_cast<uint8_t>(timestamp >> 8),
        static_cast<uint8_t>(timestamp),
    };
    uint8_t digest[20];
    size_t computed = 0;
    cmox_mac_compute(CMOX_HMAC_SHA1_ALGO, message, 8, key, key_length, nullptr, 0, digest, 20, &computed);

    if (computed != 20) return CRYPTO_FAILED;

    uint8_t offset = digest[19] & 0x0F;
    uint32_t d_t = ( (digest[offset] & 0x7F) << 24 ) |
                  ( digest[offset + 1] << 16 ) |
                  ( digest[offset + 2] << 8 ) |
                  ( digest[offset + 3] );

    *result = d_t % mask;

    return CRYPTO_SUCCESS;
}


