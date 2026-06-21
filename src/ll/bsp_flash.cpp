#include "bsp_flash.h"
#include "spi.h"

__STATIC_INLINE void PollForSPI()
{
    uint16_t timeout = 1000;
    while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY && timeout)
    {
        HAL_Delay(1);
        --timeout;
    }
}

__STATIC_INLINE void TransmitCMD(const uint8_t* data, uint16_t len)
{
    HAL_SPI_Transmit(&hspi1, data, len, HAL_MAX_DELAY);
}

__STATIC_INLINE void TransmitData(const uint8_t* data, const uint16_t len)
{
    if (len < 16)
        HAL_SPI_Transmit(&hspi1, data, len, HAL_MAX_DELAY);
    else
    {
        HAL_SPI_Transmit_DMA(&hspi1, data, len);
        PollForSPI();
    }
}

__STATIC_INLINE void ReceiveData(uint8_t* rx_data, const uint16_t len)
{
    if (len < 16)
        HAL_SPI_Receive(&hspi1, rx_data, len, HAL_MAX_DELAY);
    else
    {
        HAL_SPI_Receive_DMA(&hspi1, rx_data, len);
        PollForSPI();
    }
}

__STATIC_INLINE void WriteEnable()
{
    constexpr uint8_t cmd = 0x06;
    FCS_GPIO_Port->BRR = FCS_Pin;
    TransmitCMD(&cmd, 1);
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
    TransmitCMD(cmd, 4);
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
    TransmitCMD(cmd, 4);
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
    TransmitCMD(cmd, 4);
    FCS_GPIO_Port->BSRR = FCS_Pin;
}

void bsp_flash::chip_erase()
{
    constexpr uint8_t cmd = 0x60;
    WriteEnable();
    FCS_GPIO_Port->BRR = FCS_Pin;
    TransmitCMD(&cmd, 1);
    FCS_GPIO_Port->BSRR = FCS_Pin;
}

void bsp_flash::enter_powerdown(bool enter)
{
    constexpr uint8_t cmd_pd = 0xB9;
    constexpr uint8_t cmd_rc = 0xAB;
    FCS_GPIO_Port->BRR = FCS_Pin;
    if (enter)
    {
        TransmitCMD(&cmd_pd, 1);
    }
    else
    {
        TransmitCMD(&cmd_rc, 1);
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
    TransmitCMD(cmd, 4);
    ReceiveData(rx_buffer, 2);
    FCS_GPIO_Port->BSRR = FCS_Pin;
}


void bsp_flash::reset_device()
{
    constexpr uint8_t cmd1 = 0x66;
    constexpr uint8_t cmd2 = 0x99;
    FCS_GPIO_Port->BRR = FCS_Pin;
    TransmitCMD(&cmd1, 1);
    FCS_GPIO_Port->BSRR = FCS_Pin;
    FCS_GPIO_Port->BRR = FCS_Pin;
    TransmitCMD(&cmd2, 1);
    FCS_GPIO_Port->BSRR = FCS_Pin;
}

bool bsp_flash::read_busy()
{
    constexpr uint8_t cmd = 0x05;
    uint8_t status = 0;
    FCS_GPIO_Port->BRR = FCS_Pin;
    TransmitCMD(&cmd, 1);
    ReceiveData(&status, 1);
    FCS_GPIO_Port->BSRR = FCS_Pin;
    if (status & 1)
    {
        return true;
    }
    return false;
}

