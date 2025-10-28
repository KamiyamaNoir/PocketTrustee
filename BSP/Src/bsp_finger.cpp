#include "bsp_finger.h"
#include "usart.h"

using namespace finger;

#define CALC_CHECKSUM(payload) \
    do { \
        uint16_t payload_sum = CheckSum(&payload[6], sizeof(payload) - 8); \
        payload[sizeof(payload) - 2] = static_cast<uint8_t>(payload_sum >> 8); \
        payload[sizeof(payload) - 1] = static_cast<uint8_t>(payload_sum); \
    }while(0);

__STATIC_INLINE void TransmitData(uint8_t* data, uint16_t size)
{
    HAL_UART_Transmit(&hlpuart1, data, size, HAL_MAX_DELAY);
}

__STATIC_INLINE uint16_t CheckSum(const uint8_t* content, uint16_t len)
{
    uint16_t sum = 0;
    for (uint16_t i = 0; i < len; i++)
    {
        sum += content[i];
    }
    return sum;
}

static uint8_t finger_buffer[128];
static volatile uint8_t receive_size = 0;

void fingerprint_uart_callback(uint16_t size)
{
    receive_size = size;
    HAL_UARTEx_ReceiveToIdle_IT(&hlpuart1, finger_buffer, sizeof(finger_buffer));
}

static uint16_t polling_for_response(uint16_t timeout=1000)
{
    receive_size = 0;
    while (receive_size == 0)
    {
        timeout--;
        if (timeout == 0)
        {
            return 0;
        }
        HAL_Delay(1);
    }
    return receive_size;
}

// static uint8_t WriteRegister(uint8_t addr, uint8_t content)
// {
//     uint8_t payload[] = {
//         // Head
//         0xEF, 0x01,
//         // Device Address
//         0xFF, 0xFF, 0xFF, 0xFF,
//         // Command Package
//         0x01,
//         // Package Length
//         0x00, 0x05,
//         // Command
//         0x0E,
//         // Register and Content
//         addr, content,
//         // Check sum
//         0x00, 0x00,
//     };
//     CALC_CHECKSUM(payload);
//     TransmitData(payload, sizeof(payload));
//     uint16_t nread = polling_for_response();
//     if (nread != 12)
//         return 1;
//     return finger_buffer[9];
// }

uint8_t finger::getImageAuth()
{
    uint8_t payload[] = {
        // Head
        0xEF, 0x01,
        // Device Address
        0xFF, 0xFF, 0xFF, 0xFF,
        // Command Package
        0x01,
        // Package Length
        0x00, 0x03,
        // Command
        0x01,
        // Check sum
        0x00, 0x05,
    };
    TransmitData(payload, sizeof(payload));
    uint16_t nread = polling_for_response();
    if (nread == 13)
        return finger_buffer[10];
    if (nread == 12)
        return finger_buffer[9];
    return 1;
}

uint8_t finger::genChar(uint8_t buffer_id)
{
    uint8_t payload[] = {
        // Head
        0xEF, 0x01,
        // Device Address
        0xFF, 0xFF, 0xFF, 0xFF,
        // Command Package
        0x01,
        // Package Length
        0x00, 0x04,
        // Command
        0x02,
        // Buffer ID
        buffer_id,
        // Check sum
        0x00, 0x00,
    };
    CALC_CHECKSUM(payload);
    TransmitData(payload, sizeof(payload));
    uint16_t nread = polling_for_response();
    if (nread == 13)
        return finger_buffer[10];
    if (nread == 12)
        return finger_buffer[9];
    return 1;
}

/**
 * Only available when security level below 1
 * @return Result
 */
uint8_t finger::accrMatch()
{
    uint8_t payload[] = {
        // Head
        0xEF, 0x01,
        // Device Address
        0xFF, 0xFF, 0xFF, 0xFF,
        // Command Package
        0x01,
        // Package Length
        0x00, 0x03,
        // Command
        0x03,
        // Check sum
        0x00, 0x07,
    };
    TransmitData(payload, sizeof(payload));
    uint16_t nread = polling_for_response();
    if (nread == 13)
        return finger_buffer[10];
    if (nread == 12)
        return finger_buffer[9];
    return 1;
}

/**
 * Only available when security level below 1
 * @return Result
 */
SearchResult finger::Search(uint8_t buffer_id, uint16_t start, uint16_t page_num)
{
    uint8_t payload[] = {
        // Head
        0xEF, 0x01,
        // Device Address
        0xFF, 0xFF, 0xFF, 0xFF,
        // Command Package
        0x01,
        // Package Length
        0x00, 0x08,
        // Command
        0x04,
        // Buffer ID
        buffer_id,
        // Start page
        static_cast<uint8_t>(start >> 8),
        static_cast<uint8_t>(start),
        // Page num
        static_cast<uint8_t>(page_num >> 8),
        static_cast<uint8_t>(page_num),
        // Check sum
        0x00, 0x00,
    };
    CALC_CHECKSUM(payload);
    TransmitData(payload, sizeof(payload));
    uint16_t nread = polling_for_response();
    if (nread == 16)
        return {
        finger_buffer[9],
        static_cast<uint16_t>(finger_buffer[10] << 8 | finger_buffer[11]),
        static_cast<uint16_t>(finger_buffer[12] << 8 | finger_buffer[13])
    };
    if (nread == 17)
        return {
            finger_buffer[10],
            static_cast<uint16_t>(finger_buffer[11] << 8 | finger_buffer[12]),
            static_cast<uint16_t>(finger_buffer[13] << 8 | finger_buffer[14])
        };
    return {0};
}

uint8_t finger::regModel()
{
    uint8_t payload[] = {
        // Head
        0xEF, 0x01,
        // Device Address
        0xFF, 0xFF, 0xFF, 0xFF,
        // Command Package
        0x01,
        // Package Length
        0x00, 0x03,
        // Command
        0x05,
        // Check sum
        0x00, 0x09,
    };
    TransmitData(payload, sizeof(payload));
    uint16_t nread = polling_for_response();
    if (nread == 13)
        return finger_buffer[10];
    if (nread == 12)
        return finger_buffer[9];
    return 1;
}

uint8_t finger::storeChar(uint16_t page_id, uint8_t buffer_id)
{
    uint8_t payload[] = {
        // Head
        0xEF, 0x01,
        // Device Address
        0xFF, 0xFF, 0xFF, 0xFF,
        // Command Package
        0x01,
        // Package Length
        0x00, 0x06,
        // Command
        0x06,
        // Buffer ID
        buffer_id,
        // Page ID
        static_cast<uint8_t>(page_id >> 8),
        static_cast<uint8_t>(page_id),
        // Check sum
        0x00, 0x00,
    };
    CALC_CHECKSUM(payload);
    TransmitData(payload, sizeof(payload));
    uint16_t nread = polling_for_response();
    if (nread == 13)
        return finger_buffer[10];
    if (nread == 12)
        return finger_buffer[9];
    return 1;
}

uint8_t finger::loadChar(uint16_t page_id, uint8_t buffer_id)
{
    uint8_t payload[] = {
        // Head
        0xEF, 0x01,
        // Device Address
        0xFF, 0xFF, 0xFF, 0xFF,
        // Command Package
        0x01,
        // Package Length
        0x00, 0x06,
        // Command
        0x07,
        // Buffer ID
        buffer_id,
        // Page ID
        static_cast<uint8_t>(page_id >> 8),
        static_cast<uint8_t>(page_id),
        // Check sum
        0x00, 0x00,
    };
    CALC_CHECKSUM(payload);
    TransmitData(payload, sizeof(payload));
    uint16_t nread = polling_for_response();
    if (nread == 13)
        return finger_buffer[10];
    if (nread == 12)
        return finger_buffer[9];
    return 1;
}

uint8_t finger::deleteChar(uint16_t page_id, uint16_t number)
{
    uint8_t payload[] = {
        // Head
        0xEF, 0x01,
        // Device Address
        0xFF, 0xFF, 0xFF, 0xFF,
        // Command Package
        0x01,
        // Package Length
        0x00, 0x07,
        // Command
        0x0C,
        // Page ID
        static_cast<uint8_t>(page_id >> 8),
        static_cast<uint8_t>(page_id),
        // Number
        static_cast<uint8_t>(number >> 8),
        static_cast<uint8_t>(number),
        // Check sum
        0x00, 0x00,
    };
    CALC_CHECKSUM(payload);
    TransmitData(payload, sizeof(payload));
    uint16_t nread = polling_for_response();
    if (nread == 13)
        return finger_buffer[10];
    if (nread == 12)
        return finger_buffer[9];
    return 1;
}

uint8_t finger::clearChar()
{
    uint8_t payload[] = {
        // Head
        0xEF, 0x01,
        // Device Address
        0xFF, 0xFF, 0xFF, 0xFF,
        // Command Package
        0x01,
        // Package Length
        0x00, 0x03,
        // Command
        0x0D,
        // Check sum
        0x00, 0x11,
    };
    TransmitData(payload, sizeof(payload));
    uint16_t nread = polling_for_response();
    if (nread == 13)
        return finger_buffer[10];
    if (nread == 12)
        return finger_buffer[9];
    return 1;
}

uint8_t finger::getImageEnroll()
{
    uint8_t payload[] = {
        // Head
        0xEF, 0x01,
        // Device Address
        0xFF, 0xFF, 0xFF, 0xFF,
        // Command Package
        0x01,
        // Package Length
        0x00, 0x03,
        // Command
        0x29,
        // Check sum
        0x00, 0x2D,
    };
    TransmitData(payload, sizeof(payload));
    uint16_t nread = polling_for_response();
    if (nread == 13)
        return finger_buffer[10];
    if (nread == 12)
        return finger_buffer[9];
    return 1;
}

uint8_t finger::cancelOperation()
{
    uint8_t payload[] = {
        // Head
        0xEF, 0x01,
        // Device Address
        0xFF, 0xFF, 0xFF, 0xFF,
        // Command Package
        0x01,
        // Package Length
        0x00, 0x03,
        // Command
        0x30,
        // Check sum
        0x00, 0x34,
    };
    TransmitData(payload, sizeof(payload));
    uint16_t nread = polling_for_response();
    if (nread == 13)
        return finger_buffer[10];
    if (nread == 12)
        return finger_buffer[9];
    return 1;
}





