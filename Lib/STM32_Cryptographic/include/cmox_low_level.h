/**
  ******************************************************************************
  * @file    cmox_low_level.h
  * @brief   This file exports symbols needed to use low level module
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

#ifndef CMOX_LOW_LEVEL_H
#define CMOX_LOW_LEVEL_H

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */


/** @addtogroup CMOX_CRYPTO
  * @{
  */

/** @addtogroup CMOX_INIT Initialization module
  * @{
  */

/**
  * @brief          CMOX library low level initialization
  * @note           Implements low level initialization required by cryptographic
  *                 library to operate properly
  * @param          pArg User defined parameter that is transmitted from Initialize service
  * @retval         Initialization status: @ref CMOX_INIT_SUCCESS / @ref CMOX_INIT_FAIL
  */
cmox_init_retval_t cmox_ll_init(void *pArg);

/**
  * @brief          CMOX library low level de-initialization
  * @param          pArg User defined parameter that is transmitted from Finalize service
  * @retval         De-initialization status: @ref CMOX_INIT_SUCCESS / @ref CMOX_INIT_FAIL
  */
cmox_init_retval_t cmox_ll_deInit(void *pArg);

/**
  * @}
  */
/**
  * @}
  */

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* CMOX_LOW_LEVEL_H */
