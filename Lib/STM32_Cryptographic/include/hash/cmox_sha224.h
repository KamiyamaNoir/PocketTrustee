/**
  ******************************************************************************
  * @file    cmox_sha224.h
  * @author  MCD Application Team
  * @brief   Header file for the SHA224 hash definition and function
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
#ifndef CMOX_SHA224_H
#define CMOX_SHA224_H

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* Include files -------------------------------------------------------------*/
#include "cmox_hash.h"
#include "cmox_md.h"

/**
  * @addtogroup CMOX_MD
  * @{
  */

/**
  * @defgroup CMOX_SHA224 SHA224 hash module
  * @{
  */

/* Macros --------------------------------------------------------------------*/

/** @defgroup CMOX_SHA224_MACROS SHA224 macros
  * @{
  */

#define CMOX_SHA224_SIZE 28U  /*!< Maximum size in bytes of a SHA224 digest */

/**
  * @}
  */

/* Public types --------------------------------------------------------------*/

/** @defgroup CMOX_SHA224_PUBLIC_TYPES SHA224 module public types
  * @{
  */

/**
  * @brief SHA224 handle type definition
  */
typedef cmox_mdSmall_handle_t cmox_sha224_handle_t;

/**
  * @}
  */

/* Public methods prototypes -------------------------------------------------*/

/** @defgroup CMOX_SHA224_PUBLIC_METHODS SHA224 module public method prototypes
  * @{
  */

/**
  * @brief SHA224 constructor
  *
  * @param P_pThis Pointer to the SHA224 handle to initialize
  * @return cmox_hash_handle_t* Pointer to a general hash handle. This will be
  *              used by the general purpose hash functions in order to perform
  *              the algorithm
  */
cmox_hash_handle_t *cmox_sha224_construct(cmox_sha224_handle_t *P_pThis);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CMOX_SHA224_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
