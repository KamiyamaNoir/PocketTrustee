#include "bsp_rfid.h"
#include "comp.h"
#include "dac.h"
#include "lptim.h"
#include "tim.h"


// based on 4us tick
#define HAFCYC_MIN 57
#define HAFCYC_MAX 90
#define FULCYC_MIN 120
#define FULCYC_MAX 136


static uint32_t emulation_circulate_buffer[4] = {};
static uint8_t circulate_buffer_index = 0;

static volatile bool inSample = false;
static uint8_t sampleIndex = 0;
static uint32_t sampleClock = 0;
static uint32_t sample_buffer[4] = {};

void HAL_COMP_TriggerCallback(COMP_HandleTypeDef *hcomp)
{
    if (hcomp == &hcomp1)
    {
        rfid::lf_oa_callback();
    }
}

BSP_Status rfid::parseSample(IDCard* dest)
{
    // Find out 1STOP bit(0) and 9HEAD bit(1)
    uint8_t pointer = 0;
    uint8_t index = 0;
    for (;;)
    {
        // Find bit 0
        if (!(sample_buffer[pointer / 32] & (1 << (pointer % 32))))
        {
            index = pointer;
            // count how many bit1 following bit0
            for (;;)
            {
                pointer++;
                if (!(sample_buffer[pointer / 32] & (1 << (pointer % 32))))
                {
                    // Oops! Another bit0
                    // Makesure: the next circle starts at this position
                    pointer--;
                    break;
                }
            }
            if (pointer >= 64) return BSP_EXCEPTION;
            if (pointer - index >= 9)
            {
                // Perfect HEAD
                break;
            }
        }
        pointer++;
    }
    // Copy raw data
    index += 1;
    dest->raw_mcode[0] = 0;dest->raw_mcode[1] = 0;
    for (uint8_t i = 0; i < 64; i++)
    {
        if (sample_buffer[(i + index) / 32] & (1 << ((i + index) % 32)))
            dest->raw_mcode[i / 32] |= 1 << (i % 32);
    }
    // Data starts at "index"
    index += 9;
    uint8_t data_matrix[11] = {};
    for (uint8_t i = 0; i < 11; i++)
    {
        for (uint8_t j = 0; j < 5; j++)
        {
            uint8_t bit = sample_buffer[(index + i*5 + j) / 32] & (1 << ((index + i*5 + j) % 32)) ? 1 : 0;
            data_matrix[i] |= bit << j;
        }
    }
    // Even parity check
    for (uint8_t i = 0; i < 10; i++)
    {
        bool even = false;
        for (uint8_t j = 0; j < 4; j++)
        {
            if (data_matrix[i] & (1 << j)) even = !even;
        }
        if (even ^ static_cast<bool>(data_matrix[i] & 0x10)) return BSP_EXCEPTION;
    }
    for (uint8_t i = 0; i < 4; i++)
    {
        bool even = false;
        for (uint8_t j = 0; j < 10; j++)
        {
            if (data_matrix[j] & (1 << i)) even = !even;
        }
        if (even ^ static_cast<bool>(data_matrix[10] & (1 << i))) return BSP_EXCEPTION;
    }
    // Extract code
    for (uint8_t i = 0; i < 32; i++)
    {
        uint8_t bit = data_matrix[i / 4 + 2] & (1 << (i % 4)) ? 1 : 0;
        dest->idcode <<= 1;
        dest->idcode |= bit;
    }
    return BSP_OK;
}


void rfid::loadEmulator(const IDCard* card)
{
    emulation_circulate_buffer[0] = 0;
    emulation_circulate_buffer[1] = 0;
    emulation_circulate_buffer[2] = 0;
    emulation_circulate_buffer[3] = 0;
    for (uint8_t i = 0; i < 64; i++)
    {
        // bit1 -> (mcode)10 -> (drive)01
        // bit0 -> (mcode)01 -> ...
        uint8_t bit = card->raw_mcode[i / 32] & (1 << (i % 32)) ? 1 : 2;
        emulation_circulate_buffer[i / 16] |= bit << (i*2) % 32;
    }
}

void rfid::clearEmulator()
{
    emulation_circulate_buffer[0] = 0;
    emulation_circulate_buffer[1] = 0;
    emulation_circulate_buffer[2] = 0;
    emulation_circulate_buffer[3] = 0;
}

void rfid::newSample()
{
    sample_buffer[0] = 0;
    sample_buffer[1] = 0;
    sample_buffer[2] = 0;
    sample_buffer[3] = 0;
    sampleIndex = 0;
    sampleClock = 0;
    inSample = true;
    set_drive_mode(READ);
}

uint8_t rfid::availablePoint()
{
    return sampleIndex;
}

void rfid::lf_oa_callback()
{
    if (!inSample) return;

    uint32_t delay = 0;
    uint32_t now = hlptim1.Instance->CNT;
    if (now < sampleClock) delay = now + (0xFFFF - sampleClock);
    else delay = now - sampleClock;
    if (FULCYC_MIN <= delay && delay <= FULCYC_MAX)
    {
        sampleClock = hlptim1.Instance->CNT;
        // const uint32_t status = (LF_OA_GPIO_Port->IDR & LF_OA_Pin) ? 0 : 1;
        const uint32_t status = HAL_COMP_GetOutputLevel(&hcomp1) == COMP_OUTPUT_LEVEL_HIGH ? 0 : 1;
        sample_buffer[sampleIndex / 32] |= status << (sampleIndex % 32);
        if (sampleIndex >= 127)
        {
            inSample = false;
            set_drive_mode(STOP);
            RFID_BufferFullCallback();
        }
        else sampleIndex++;
    }
    else if (HAFCYC_MIN <= delay && delay <= HAFCYC_MAX)
    {
        // dispose
        return;
    }
    else
    {
        // too short or long, probably an abnormal signal
        // return pointer
        sampleClock = hlptim1.Instance->CNT;
        sampleIndex = 0;
        sample_buffer[0] = 0;sample_buffer[1] = 0;sample_buffer[2] = 0;
        sample_buffer[3] = 0;
    }
}

void rfid::emulate_callback()
{
    if (emulation_circulate_buffer[circulate_buffer_index / 32] & (1 << (circulate_buffer_index % 32)))
        LF_MOD_GPIO_Port->BSRR = LF_MOD_Pin;
    else
        LF_MOD_GPIO_Port->BRR = LF_MOD_Pin;
    circulate_buffer_index++;
    if (circulate_buffer_index >= 128) circulate_buffer_index = 0;
}

__STATIC_INLINE void switch_stop_mode()
{
    HAL_TIM_OC_Stop(&htim1, TIM_CHANNEL_1);
    HAL_DAC_Stop(&hdac1, DAC_CHANNEL_1);
    HAL_COMP_Stop(&hcomp1);
    HAL_LPTIM_Counter_Stop(&hlptim1);

    HAL_TIM_OC_DeInit(&htim1);
    HAL_TIM_Base_DeInit(&htim1);
    HAL_LPTIM_DeInit(&hlptim1);
    HAL_COMP_DeInit(&hcomp1);
    HAL_DAC_DeInit(&hdac1);

    // Connect DRV to GND
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
}

__STATIC_INLINE void switch_read_mode()
{
    MX_TIM1_Init();
    MX_DAC1_Init();
    MX_COMP1_Init();
    MX_LPTIM1_Init();

    HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_8B_R, 2);
    HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);
    HAL_LPTIM_Counter_Start(&hlptim1, 0xFFFF);
    HAL_TIM_Base_Start(&htim1);
    HAL_TIM_OC_Start(&htim1, TIM_CHANNEL_1);
    HAL_COMP_Start(&hcomp1);
}

void rfid::set_drive_mode(const DriveMode mode)
{
    LF_MOD_GPIO_Port->BRR = LF_MOD_Pin;
    if (mode == STOP || mode == EMULATE)
    {
        // Disable unnecessart periphers
        HAL_TIM_Base_Stop_IT(&htim16);
        switch_stop_mode();
        // TIM16 256us basis
        if (mode == EMULATE)
        {
            HAL_TIM_Base_Start_IT(&htim16);
        }
    }
    else
    {
        switch_read_mode();
    }
}

__weak void rfid::RFID_BufferFullCallback()
{

}




