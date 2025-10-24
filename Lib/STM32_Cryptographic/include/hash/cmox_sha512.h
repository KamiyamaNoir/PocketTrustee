/**
  ******************************************************************************
  * @file    cmox_sha512.h
  * @author  MCD Application Team
  * @brief   Header file for the SHA512 hash definition and function
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
#ifndef CMOX_SHA512_H
#define CMOX_SHA512_H

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
  * @defgroup CMOX_SHA512 SHA512 hash module
  * @{
  */

/* Macros --------------------------------------------------------------------*/

/** @defgroup CMOX_SHA512_MACROS SHA512 macros
  * @{
  */

#define CMOX_SHA512_SIZE      64U  /*!< Maximum size in bytes of a SHA512 digest */
#define CMOX_SHA512_224_SIZE  28U  /*!< Maximum size in bytes of a SHA512/224 digest */
#define CMOX_SHA512_256_SIZE  32U  /*!< Maximum size in bytes of a SHA512/256 digest */

/**
  * @}
  */

/* Public types --------------------------------------------------------------*/

/** @defgroup CMOX_SHA512_PUBLIC_TYPES SHA512 module public types
  * @{
  */

/**
  * @brief SHA512 handle type definition
  */
typedef cmox_mdLarge_handle_t cmox_sha512_handle_t;

/**
  * @}
  */

/* Public methods prototypes -------------------------------------------------*/

/** @defgroup CMOX_SHA512_PUBLIC_METHODS SHA512 module public method prototypes
  * @{
  */

/**
  * @brief SHA512 constructor
  *
  * @param P_pThis Pointer to the SHA512 handle to initialize
  * @return cmox_hash_handle_t* Pointer to a general hash handle. This will be
  *              used by the general purpose hash functions in order to perform
  *              the algorithm
  */
cmox_hash_handle_t *cmox_sha512_construct(cmox_sha512_handle_t *P_pThis);

/**
  * @brief SHA512/224 constructor
  *
  * @param P_pThis Pointer to the SHA512 handle to initialize
  * @return cmox_hash_handle_t* Pointer to a general hash handle. This will be
  *              used by the general purpose hash functions in order to perform
  *              the algorithm
  */
cmox_hash_handle_t *cmox_sha512_224_construct(cmox_sha512_handle_t *P_pThis);

/**
  * @brief SHA512/256 constructor
  *
  * @param P_pThis Pointer to the SHA512 handle to initialize
  * @return cmox_hash_handle_t* Pointer to a general hash handle. This will be
  *              used by the general purpose hash functions in order to perform
  *              the algorithm
  */
cmox_hash_handle_t *cmox_sha512_256_construct(cmox_sha512_handle_t *P_pThis);

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

#endif /* CMOX_SHA512_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
