#include "bsp_nfc.h"
#include "bsp_pn532.h"
#include "simple_buffer.hpp"
#include "usart.h"

static nfc::NFC_Route nfc_current_route = nfc::ROUTE_NONE;
extern StaticRingBuffer cdc_receive_ring_buffer;

nfc::NFC_Route nfc::get_route()
{
    return nfc_current_route;
}

void nfc::set_route(NFC_Route route)
{
    nfc_current_route = route;
    HF_EN1_GPIO_Port->BSRR = HF_EN1_Pin;
    if (route == ROUTE_NONE) return;
    if (route == ROUTE_PN532)
    {
        HF_S3_GPIO_Port->BSRR = HF_S3_Pin;
    }
    else
    {
        HF_EN1_GPIO_Port->BRR = HF_EN1_Pin;
        switch (route)
        {
        case ROUTE_C1:
            // Card1 -> L L
            HF_S1_GPIO_Port->BRR = HF_S1_Pin;
            HF_S2_GPIO_Port->BRR = HF_S2_Pin;
            break;
        case ROUTE_C2:
            // Card2 -> H L
            HF_S1_GPIO_Port->BSRR = HF_S1_Pin;
            HF_S2_GPIO_Port->BRR = HF_S2_Pin;
            break;
        case ROUTE_C3:
            // Card3 -> L H
            HF_S1_GPIO_Port->BRR = HF_S1_Pin;
            HF_S2_GPIO_Port->BSRR = HF_S2_Pin;
            break;
        case ROUTE_C4:
            // Card4 -> H H
            HF_S1_GPIO_Port->BSRR = HF_S1_Pin;
            HF_S2_GPIO_Port->BSRR = HF_S2_Pin;
        default:
            break;
        }
    }
}

extern void cdc_acm_data_send(const uint8_t* data, uint8_t len, uint16_t timeout);
uint8_t PN532_RX_BUFFER[128];
static uint8_t PN532_TX_BUFFER[128];
static volatile bool in_transparent_mode = false;

void nfc::enable_transparent_mode()
{
    core::RegisterACMDevice();
    MX_USART3_UART_Init();
    set_route(ROUTE_PN532);
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
    set_route(ROUTE_NONE);
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

