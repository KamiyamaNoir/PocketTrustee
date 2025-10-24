#ifndef CRYPTO_BASE_H
#define CRYPTO_BASE_H

#include "main.h"
//NOLINTNEXTLINE
#include "rng.h"

#define DEVICE_UNIQUE_ID1 (*(uint32_t*)(0x1FFF7590))
#define DEVICE_UNIQUE_ID2 (*(uint32_t*)(0x1FFF7594))
#define DEVICE_UNIQUE_ID3 (*(uint32_t*)(0x1FFF7598))

#define TRNG() HAL_RNG_GetRandomNumber(&hrng)

namespace crypto
{
    enum CRYPTO_RESULT
    {
        CRYPTO_SUCCESS,
        CRYPTO_FAILED
    };

    struct KeygenConfiguration
    {
        bool containNumber;
        bool containUppercase;
        bool containLowercase;
        bool containSChar;
        uint8_t length;
    };

    /**
     * Calculate CRC by hardware
     * Although STM32 only contain one CRC moudle,
     * multi instance of CRC_Hnadler is possible, however, a call of resume is required ever operation.
     * When operating individually, it is only requied to call resume once after construction.
     */
    class CRC_Handler
    {
    public:
        CRYPTO_RESULT retval;

        CRC_Handler();
        CRC_Handler& resume();
        CRC_Handler& reset();
        CRC_Handler& next(uint8_t* buffer, uint16_t length);
        uint32_t result() const;
    private:
        uint32_t crc_result;
    };

    CRYPTO_RESULT skeygen(const KeygenConfiguration* cfg, char* dst);

    CRYPTO_RESULT totp_calculate(uint8_t key[20], uint32_t* result, uint8_t step=30, uint32_t mask=1000000UL);
}

#endif //CRYPTO_BASE_H
