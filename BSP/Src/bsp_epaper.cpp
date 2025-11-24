#include "bsp_epaper.h"
#include "cmsis_os.h"
#include "spi.h"

#define EPD_WIDTH   128
#define EPD_HEIGHT  296
#define EPD_ARRAY  4736

#define DAT(x) ByteData(x)
#define CMD(x) TransmitCommand(x)
// #define BUSY while (HAL_GPIO_ReadPin(DBUSY_GPIO_Port, DBUSY_Pin) != GPIO_PIN_RESET) {}
#define BUSY CheckBUSY_RTOS()

__STATIC_INLINE void TransmitData(const uint8_t* data, const uint16_t len)
{
    DDC_GPIO_Port->BSRR = DDC_Pin;
    HAL_SPI_Transmit(&hspi2, data, len, HAL_MAX_DELAY);
}

__STATIC_INLINE void TransmitDataDMA(const uint8_t* data, const uint16_t len)
{
    DDC_GPIO_Port->BSRR = DDC_Pin;
    HAL_SPI_Transmit_DMA(&hspi2, data, len);
}

__STATIC_INLINE void PollForSPI()
{
    uint16_t timeout = 1000;
    while (HAL_SPI_GetState(&hspi2) != HAL_SPI_STATE_READY && timeout)
    {
        HAL_Delay(1);
        --timeout;
    }
}

__STATIC_INLINE void ByteData(const uint8_t data)
{
    TransmitData(&data, 1);
}

__STATIC_INLINE void TransmitCommand(const uint8_t command)
{
    DDC_GPIO_Port->BRR = DDC_Pin;
    HAL_SPI_Transmit(&hspi2, &command, 1, HAL_MAX_DELAY);
}

static void CheckBUSY_RTOS()
{
    uint16_t timeout = 400;
    while (HAL_GPIO_ReadPin(DBUSY_GPIO_Port, DBUSY_Pin) != GPIO_PIN_RESET)
    {
        timeout--;
        osDelay(10);
        if (timeout == 0)
        {
            // DRST_GPIO_Port->BRR = DRST_Pin;
            // osDelay(10);
            // DRST_GPIO_Port->BSRR = DRST_Pin;
            // osDelay(10);
            return;
        }
    }
}

static void ep_hardware_init(const epaper::EpaperMode mode)
{
    // HWRST
    DRST_GPIO_Port->BRR = DRST_Pin;
    HAL_Delay(5);
    DRST_GPIO_Port->BSRR = DRST_Pin;
    HAL_Delay(1);
    BUSY;
    // Select internal temperture sensor
    CMD(0x18);
    DAT(0x80);
    // Data entry sequence -> X decrement, Y increment
    CMD(0x11);
    DAT(0x02);
    // X Start End
    CMD(0x44);
    DAT(EPD_WIDTH/8-1);
    DAT(0x00);
    // Y Start End
    CMD(0x45);
    DAT(0x00);
    DAT(0x00);
    DAT((EPD_HEIGHT-1)%256);
    DAT((EPD_HEIGHT-1)/256);
    // Reset X address
    CMD(0x4E);
    DAT(EPD_WIDTH/8-1);
    // Reset Y address
    CMD(0x4F);
    DAT(0x00);
    DAT(0x00);
    if (mode != epaper::EP_FULL)
    {
        // Display Update Control 2 -> B1
        CMD(0x22);
        DAT(0xB1);
        CMD(0x20);
        BUSY;
        // Temperature Sensor Control
        CMD(0x1A);
        DAT(0x64);
        DAT(0x00);
        // Display Update Control 2 -> 91
        CMD(0x22);
        DAT(0x91);
        CMD(0x20);
        BUSY;
    }
    else
    {
        // Driver Output control
        CMD(0x01);
        DAT((EPD_HEIGHT-1)%256);
        DAT((EPD_HEIGHT-1)/256);
        DAT(0x00);
        // Border Waveform
        CMD(0x3C);
        DAT(0x05);
        // Display Update Control
        CMD(0x21);
        DAT(0x00);
        DAT(0x80);
        BUSY;
    }
}

/**
 * This function is used to pre-update the EPD fully, which is aimed to prepare for partial update
 * @param data Compelete sequence of data
 */
void epaper::pre_update(const uint8_t* data)
{
    // Initialize EPD as same as FULL update
    ep_hardware_init(EP_FULL);
    // However, both register 0x24 and 0x26 is expected to be written
    CMD(0x24);
    TransmitDataDMA(data, EPD_ARRAY);
    PollForSPI();
    CMD(0x26);
    TransmitDataDMA(data, EPD_ARRAY);
    PollForSPI();
    // Same as FULL_Update
    CMD(0x22);
    DAT(0xF7);
    CMD(0x20);
    BUSY;
    CMD(0x10);
    DAT(0x01);
}

void epaper::update_fast(const uint8_t* data)
{
    ep_hardware_init(EP_FAST);
    CMD(0x24);
    TransmitDataDMA(data, EPD_ARRAY);
    PollForSPI();
    CMD(0x26);
    TransmitDataDMA(data, EPD_ARRAY);
    PollForSPI();
    CMD(0x22);
    DAT(0xC7);
    CMD(0x20);
    BUSY;
    CMD(0x10);
    DAT(0x01);
}

void epaper::update_part_full(const uint8_t* data)
{
    // HWRST
    DRST_GPIO_Port->BRR = DRST_Pin;
    HAL_Delay(5);
    DRST_GPIO_Port->BSRR = DRST_Pin;
    HAL_Delay(1);
    BUSY;

    // Border Waveform
    CMD(0x3C);
    DAT(0x80);

    // Data entry sequence -> X decrement, Y increment
    CMD(0x11);
    DAT(0x02);
    // X Start End
    CMD(0x44);
    DAT(EPD_WIDTH/8-1);
    DAT(0x00);
    // Y Start End
    CMD(0x45);
    DAT(0x00);
    DAT(0x00);
    DAT((EPD_HEIGHT-1)%256);
    DAT((EPD_HEIGHT-1)/256);
    // Reset X address
    CMD(0x4E);
    DAT(EPD_WIDTH/8-1);
    // Reset Y address
    CMD(0x4F);
    DAT(0x00);
    DAT(0x00);

    CMD(0x24);
    TransmitDataDMA(data, EPD_ARRAY);
    PollForSPI();

    CMD(0x22);
    DAT(0xFF);
    CMD(0x20);
    BUSY;
    CMD(0x10);
    DAT(0x01);
}




