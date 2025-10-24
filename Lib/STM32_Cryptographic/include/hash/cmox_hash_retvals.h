/**
  ******************************************************************************
  * @file    cmox_hash_retvals.h
  * @author  MCD Application Team
  * @brief   Header file containing the return values for the hash module
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef CMOX_HASH_RETVALS_H
#define CMOX_HASH_RETVALS_H

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* Include files -------------------------------------------------------------*/
#include <stdint.h>

/** @addtogroup CMOX_HASH
  * @{
  */

/** @defgroup CMOX_HASH_RETVALS Hash return values
  * @{
  */

/* Macros --------------------------------------------------------------------*/

/**
  * @brief Hash operation successfully performed
  */
#define CMOX_HASH_SUCCESS                      ((cmox_hash_retval_t)0x00020000U)

/**
  * @brief Some error happens internally in the hash module
  */
#define CMOX_HASH_ERR_INTERNAL                 ((cmox_hash_retval_t)0x00020001U)

/**
  * @brief One or more parameter has been wrongly passed to the function
  *        (e.g. pointer to NULL)
  */
#define CMOX_HASH_ERR_BAD_PARAMETER            ((cmox_hash_retval_t)0x00020003U)

/**
  * @brief Error on performing the operation
  *        (e.g. an operation has been called before initializing the handle)
  */
#define CMOX_HASH_ERR_BAD_OPERATION            ((cmox_hash_retval_t)0x00020004U)

/**
  * @brief The desired digest size is not supported by the hash alforithm
  */
#define CMOX_HASH_ERR_BAD_TAG_SIZE             ((cmox_hash_retval_t)0x00020006U)

/* Public types --------------------------------------------------------------*/

/**
  * @brief Hash module return value type
  */
typedef uint32_t cmox_hash_retval_t;

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CMOX_HASH_RETVALS_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
