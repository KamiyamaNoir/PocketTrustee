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
    HAL_Delay(10);
    DRST_GPIO_Port->BSRR = DRST_Pin;
    HAL_Delay(10);
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
        if (mode == epaper::EP_FAST) DAT(0x64);
        else DAT(0x5A);
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

void epaper::update(const uint8_t* data, const EpaperMode mode)
{
    ep_hardware_init(mode);
    CMD(0x24);
    TransmitData(data, EPD_ARRAY);
    CMD(0x22);
    if (mode == EP_FAST) DAT(0xC7);
    else DAT(0xF7);
    CMD(0x20);
    BUSY;
    CMD(0x10);
    DAT(0x01);
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
    TransmitData(data, EPD_ARRAY);
    CMD(0x26);
    TransmitData(data, EPD_ARRAY);
    // Same as FULL_Update
    CMD(0x22);
    DAT(0xF7);
    CMD(0x20);
    BUSY;
    CMD(0x10);
    DAT(0x01);
}

/**
 * Reset and config necessary register.
 */
void epaper::partial_preconfig()
{
    // HWRST
    DRST_GPIO_Port->BRR = DRST_Pin;
    HAL_Delay(10);
    DRST_GPIO_Port->BSRR = DRST_Pin;
    HAL_Delay(10);
    BUSY;

    // Border Waveform
    CMD(0x3C);
    DAT(0x80);
}


void epaper::partial_write_ram(const uint8_t* data, uint16_t rx, uint8_t cy, uint16_t rw, uint8_t ch)
{
    // EPD的原点在左下角，然而UI的原点在左上角，且XY刚好相反，此处需要注意
    // 以EPD的原点计算，对齐EPD的x轴，y轴无需对齐
    // 镜像翻转
    uint8_t x_end = EPD_WIDTH/8 - cy - 1;
    uint8_t x_start = EPD_WIDTH/8 - (cy+ch);

    uint16_t y_start = rx;
    uint16_t y_end = y_start + rw - 1;

    // Data entry sequence -> X decrement, Y increment
    CMD(0x11);
    DAT(0x02);
    // X Start End
    CMD(0x44);
    DAT(x_end);
    DAT(x_start);
    // Y Start End
    CMD(0x45);
    DAT(y_start%256);
    DAT(y_start/256);
    DAT(y_end%256);
    DAT(y_end/256);
    // Reset X address
    CMD(0x4E);
    DAT(x_end);
    // Reset Y address
    CMD(0x4F);
    DAT(y_start%256);
    DAT(y_start/256);

    CMD(0x24);
    TransmitData(data, rw*ch);
}

void epaper::partial_update()
{
    CMD(0x22);
    DAT(0xFF);
    CMD(0x20);
    BUSY;
    CMD(0x10);
    DAT(0x01);
}

void epaper::update_full(const uint8_t* data)
{
    ep_hardware_init(EP_FULL);
    CMD(0x24);
    TransmitData(data, EPD_ARRAY);
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
    TransmitData(data, EPD_ARRAY);
    CMD(0x26);
    TransmitData(data, EPD_ARRAY);
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
    HAL_Delay(10);
    DRST_GPIO_Port->BSRR = DRST_Pin;
    HAL_Delay(10);
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
    TransmitData(data, EPD_ARRAY);

    CMD(0x22);
    DAT(0xFF);
    CMD(0x20);
    BUSY;
    CMD(0x10);
    DAT(0x01);
}




