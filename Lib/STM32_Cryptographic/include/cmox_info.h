/**
  ******************************************************************************
  * @file    cmox_info.h
  * @author  MCD Application Team
  * @brief   This file exports symbols needed to use information module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#ifndef CMOX_INFO_H
#define CMOX_INFO_H

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */


/** @addtogroup CMOX_CRYPTO
  * @{
  */

/** @addtogroup CMOX_INFO Information module
  * @{
  */

/**
  *  @brief Structure to store information
  */
typedef struct
{
  uint32_t version;      /*!< Library version */
  uint32_t build[7];     /*!< Build info */
} cmox_info_st;

/**
  * @brief          Get library information
  * @param          pInfo  Library information
  * @retval         None
  */
void cmox_getInfos(cmox_info_st *pInfo);

/**
  * @}
  */
/**
  * @}
  */

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* CMOX_INFO_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
