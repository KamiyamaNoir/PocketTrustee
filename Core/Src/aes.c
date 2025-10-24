/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    aes.c
  * @brief   This file provides code for the configuration
  *          of the AES instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "aes.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

CRYP_HandleTypeDef hcryp;
__ALIGN_BEGIN static const uint8_t pKeyAES[16] __ALIGN_END = {
                            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                            0x00,0x00,0x00,0x00,0x00,0x00};

/* AES init function */
void MX_AES_Init(void)
{

  /* USER CODE BEGIN AES_Init 0 */

  /* USER CODE END AES_Init 0 */

  /* USER CODE BEGIN AES_Init 1 */

  /* USER CODE END AES_Init 1 */
  hcryp.Instance = AES;
  hcryp.Init.DataType = CRYP_DATATYPE_8B;
  hcryp.Init.KeySize = CRYP_KEYSIZE_128B;
  hcryp.Init.OperatingMode = CRYP_ALGOMODE_ENCRYPT;
  hcryp.Init.ChainingMode = CRYP_CHAINMODE_AES_ECB;
  hcryp.Init.KeyWriteFlag = CRYP_KEY_WRITE_ENABLE;
  hcryp.Init.pKey = (uint8_t *)pKeyAES;
  if (HAL_CRYP_Init(&hcryp) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN AES_Init 2 */

  /* USER CODE END AES_Init 2 */

}

void HAL_CRYP_MspInit(CRYP_HandleTypeDef* crypHandle)
{

  if(crypHandle->Instance==AES)
  {
  /* USER CODE BEGIN AES_MspInit 0 */

  /* USER CODE END AES_MspInit 0 */
    /* AES clock enable */
    __HAL_RCC_AES_CLK_ENABLE();
  /* USER CODE BEGIN AES_MspInit 1 */

  /* USER CODE END AES_MspInit 1 */
  }
}

void HAL_CRYP_MspDeInit(CRYP_HandleTypeDef* crypHandle)
{

  if(crypHandle->Instance==AES)
  {
  /* USER CODE BEGIN AES_MspDeInit 0 */

  /* USER CODE END AES_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_AES_CLK_DISABLE();
  /* USER CODE BEGIN AES_MspDeInit 1 */

  /* USER CODE END AES_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
