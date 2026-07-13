#include "bsp_nfc.h"
#include "bsp_pn532.h"
#include "simple_buffer.hpp"
#include "usart.h"

// TODO: Remove bsp_nfc.cpp
#include "driver_multiplexer.hpp"

extern StaticRingBuffer cdc_receive_ring_buffer;

extern void cdc_acm_data_send(const uint8_t* data, uint8_t len, uint16_t timeout);
uint8_t PN532_RX_BUFFER[128];
static uint8_t PN532_TX_BUFFER[128];
static volatile bool in_transparent_mode = false;

void nfc::enable_transparent_mode()
{
    core::RegisterACMDevice();
    MX_USART3_UART_Init();
    CoreMultiplexer::Set(CoreMultiplexer::MUX_BYPASS);
    pn532::reset();
    cdc_receive_ring_buffer.clear();
    HAL_UARTEx_ReceiveToIdle_IT(&huart3, PN532_RX_BUFFER, 128);
    in_transparent_mode = true;
}

void nfc::disable_transparent_mode()
{
    core::DeinitUSB();
    HAL_UART_DeInit(&huart3);
    in_transparent_mode = false;
    PN_PD_GPIO_Port->BRR = PN_PD_Pin;
    CoreMultiplexer::Set(CoreMultiplexer::MUX_NONE);
    cdc_receive_ring_buffer.clear();
}

void nfc::transparent_send_cb()
{
    if (!in_transparent_mode) return;
    uint16_t nread = cdc_receive_ring_buffer.read(PN532_TX_BUFFER, cdc_receive_ring_buffer._size);
    HAL_UART_Transmit(&huart3, PN532_TX_BUFFER, nread, 100);
}

void nfc::transparent_recv_cb(uint16_t size)
{
    if (!in_transparent_mode) return;
    cdc_acm_data_send(PN532_RX_BUFFER, size, 100);
    HAL_UARTEx_ReceiveToIdle_IT(&huart3, PN532_RX_BUFFER, 128);
}

