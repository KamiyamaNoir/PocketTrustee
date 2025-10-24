#include "bsp_flash.h"
#include "spi.h"

__STATIC_INLINE void TransmitCMD(const uint8_t data)
{
    HAL_SPI_Transmit(&hspi1, &data, 1, HAL_MAX_DELAY);
}

__STATIC_INLINE void TransmitData(const uint8_t* data, const uint16_t len)
{
    HAL_SPI_Transmit(&hspi1, data, len, HAL_MAX_DELAY);
}

__STATIC_INLINE void ReceiveData(uint8_t* rx_data, const uint16_t len)
{
    HAL_SPI_Receive(&hspi1, rx_data, len, HAL_MAX_DELAY);
}

__STATIC_INLINE void WriteEnable()
{
    FCS_GPIO_Port->BRR = FCS_Pin;
    TransmitCMD(0x06);
    FCS_GPIO_Port->BSRR = FCS_Pin;
}

void bsp_flash::read_data(const uint32_t addr, uint8_t* rx_buffer, const uint16_t len)
{
    const uint8_t cmd[4]
    {
        0x03,
        static_cast<uint8_t>(addr >> 16),
        static_cast<uint8_t>(addr >> 8),
        static_cast<uint8_t>(addr),
    };
    FCS_GPIO_Port->BRR = FCS_Pin;
    TransmitData(cmd, 4);
    ReceiveData(rx_buffer, len);
    FCS_GPIO_Port->BSRR = FCS_Pin;
}

void bsp_flash::page_program(const uint32_t addr, const uint8_t* data, const uint16_t len)
{
    WriteEnable();
    const uint8_t cmd[4]
    {
        0x02,
        static_cast<uint8_t>(addr >> 16),
        static_cast<uint8_t>(addr >> 8),
        static_cast<uint8_t>(addr),
    };
    FCS_GPIO_Port->BRR = FCS_Pin;
    TransmitData(cmd, 4);
    TransmitData(data, len);
    FCS_GPIO_Port->BSRR = FCS_Pin;
}

void bsp_flash::sector_erase(const uint32_t addr)
{
    WriteEnable();
    const uint8_t cmd[4]
    {
        0x20,
        static_cast<uint8_t>(addr >> 16),
        static_cast<uint8_t>(addr >> 8),
        static_cast<uint8_t>(addr),
    };
    FCS_GPIO_Port->BRR = FCS_Pin;
    TransmitData(cmd, 4);
    FCS_GPIO_Port->BSRR = FCS_Pin;
}

void bsp_flash::chip_erase()
{
    WriteEnable();
    FCS_GPIO_Port->BRR = FCS_Pin;
    TransmitCMD(0x60);
    FCS_GPIO_Port->BSRR = FCS_Pin;
}

void bsp_flash::enter_powerdown(bool enter)
{
    FCS_GPIO_Port->BRR = FCS_Pin;
    if (enter)
    {
        TransmitCMD(0xB9);
    }
    else
    {
        TransmitCMD(0xAB);
    }
    FCS_GPIO_Port->BSRR = FCS_Pin;
}


void bsp_flash::read_device_id(uint8_t* rx_buffer)
{
    constexpr uint8_t cmd[4]
    {
        0x90,
        0x00,
        0x00,
        0x00
    };
    FCS_GPIO_Port->BRR = FCS_Pin;
    TransmitData(cmd, 4);
    ReceiveData(rx_buffer, 2);
    FCS_GPIO_Port->BSRR = FCS_Pin;
}


void bsp_flash::reset_device()
{
    FCS_GPIO_Port->BRR = FCS_Pin;
    TransmitCMD(0x66);
    FCS_GPIO_Port->BSRR = FCS_Pin;
    FCS_GPIO_Port->BRR = FCS_Pin;
    TransmitCMD(0x99);
    FCS_GPIO_Port->BSRR = FCS_Pin;
}

bool bsp_flash::read_busy()
{
    uint8_t status = 0;
    FCS_GPIO_Port->BRR = FCS_Pin;
    TransmitCMD(0x05);
    ReceiveData(&status, 1);
    FCS_GPIO_Port->BSRR = FCS_Pin;
    if (status & 1)
    {
        return true;
    }
    return false;
}

