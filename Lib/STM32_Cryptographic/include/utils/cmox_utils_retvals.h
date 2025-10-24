/**
  ******************************************************************************
  * @file    cmox_utils_retvals.h
  * @author  MCD Application Team
  * @brief   This file provides the error types and code for UTILS modules
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

#ifndef CMOX_UTILS_RETVALS_H
#define CMOX_UTILS_RETVALS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/**
  * @addtogroup CMOX_UTILS
  * @{
  */

/** @defgroup CMOX_UTILS_RETVALS Utils return values
  * @{
  */

/**
  * Return value type for the UTILS module
  */
typedef uint32_t cmox_utils_retval_t;

#define CMOX_UTILS_AUTH_SUCCESS       ((cmox_utils_retval_t)0x0007C726) /*!< UTILS operation successfully performed */
#define CMOX_UTILS_AUTH_FAIL          ((cmox_utils_retval_t)0x00076E93) /*!< Input buffers are different */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* CMOX_UTILS_RETVALS_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
