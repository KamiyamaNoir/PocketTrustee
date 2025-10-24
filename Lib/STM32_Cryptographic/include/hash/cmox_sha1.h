/**
  ******************************************************************************
  * @file    cmox_sha1.h
  * @author  MCD Application Team
  * @brief   Header file for the SHA1 hash definition and function
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
#ifndef CMOX_SHA1_H
#define CMOX_SHA1_H

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
  * @defgroup CMOX_SHA1 SHA1 hash module
  * @{
  */

/* Macros --------------------------------------------------------------------*/

/** @defgroup CMOX_SHA1_MACROS SHA1 macros
  * @{
  */

#define CMOX_SHA1_SIZE 20U /*!< Maximum size in bytes of a SHA1 digest */

/**
  * @}
  */

/* Public types --------------------------------------------------------------*/

/** @defgroup CMOX_SHA1_PUBLIC_TYPES SHA1 module public types
  * @{
  */

/**
  * @brief SHA1 handle type definition
  */
typedef cmox_mdSmall_handle_t cmox_sha1_handle_t;

/**
  * @}
  */

/* Public methods prototypes -------------------------------------------------*/

/** @defgroup CMOX_SHA1_PUBLIC_METHODS SHA1 module public method prototypes
  * @{
  */

/**
  * @brief SHA1 constructor
  *
  * @param P_pThis Pointer to the SHA1 handle to initialize
  * @return cmox_hash_handle_t* Pointer to a general hash handle. This will be
  *              used by the general purpose hash functions in order to perform
  *              the algorithm
  */
cmox_hash_handle_t *cmox_sha1_construct(cmox_sha1_handle_t *P_pThis);

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

#endif /* CMOX_SHA1_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
