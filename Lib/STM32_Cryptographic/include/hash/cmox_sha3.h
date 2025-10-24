/**
  ******************************************************************************
  * @file    cmox_sha3.h
  * @author  MCD Application Team
  * @brief   Header file for SHA3 and SHAKE hash functions
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
#ifndef CMOX_SHA3_H
#define CMOX_SHA3_H

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* Include files -------------------------------------------------------------*/
#include "cmox_hash.h"
#include "cmox_sponge.h"

/**
  * @addtogroup CMOX_HASH
  * @{
  */

/**
  * @defgroup CMOX_SHA3 SHA3 hash module
  * @{
  */

/* Macros --------------------------------------------------------------------*/

/** @defgroup CMOX_SHA3_MACROS SHA3 macros
  * @{
  */

#define CMOX_SHA3_224_SIZE 28U /*!<  Number of bytes (uint8_t) to store a SHA3-224 digest. */
#define CMOX_SHA3_256_SIZE 32U /*!<  Number of bytes (uint8_t) to store a SHA3-256 digest. */
#define CMOX_SHA3_384_SIZE 48U /*!<  Number of bytes (uint8_t) to store a SHA3-384 digest. */
#define CMOX_SHA3_512_SIZE 64U /*!<  Number of bytes (uint8_t) to store a SHA3-512 digest. */

/**
  * @}
  */

/* Public types --------------------------------------------------------------*/

/** @defgroup CMOX_SHA3_PUBLIC_TYPES SHA3 module public types
  * @{
  */

/**
  * @brief SHA3 handle type definition
  */
typedef struct
{
  cmox_hash_handle_t super;     /*!< General hash module */
  uint32_t flags;               /*!< Internal flag */
  cmox_sponge_handle_t keccak;  /*!< Keccak P-1600 handler */
} cmox_sha3_handle_t;

/**
  * @}
  */

/* Public methods prototypes -------------------------------------------------*/

/** @defgroup CMOX_SHA3_PUBLIC_METHODS SHA3 module public method prototypes
  * @{
  */

/**
  * @brief SHA3-224 constructor
  *
  * @param P_pThis Pointer to the SHA3 handle to initialize
  * @return cmox_hash_handle_t* Pointer to a general hash handle. This will be
  *              used by the general purpose hash functions in order to perform
  *              the algorithm
  */
cmox_hash_handle_t *cmox_sha3_224_construct(cmox_sha3_handle_t *P_pThis);

/**
  * @brief SHA3-256 constructor
  *
  * @param P_pThis Pointer to the SHA3 handle to initialize
  * @return cmox_hash_handle_t* Pointer to a general hash handle. This will be
  *              used by the general purpose hash functions in order to perform
  *              the algorithm
  */
cmox_hash_handle_t *cmox_sha3_256_construct(cmox_sha3_handle_t *P_pThis);

/**
  * @brief SHA3-384 constructor
  *
  * @param P_pThis Pointer to the SHA3 handle to initialize
  * @return cmox_hash_handle_t* Pointer to a general hash handle. This will be
  *              used by the general purpose hash functions in order to perform
  *              the algorithm
  */
cmox_hash_handle_t *cmox_sha3_384_construct(cmox_sha3_handle_t *P_pThis);

/**
  * @brief SHA3-512 constructor
  *
  * @param P_pThis Pointer to the SHA3 handle to initialize
  * @return cmox_hash_handle_t* Pointer to a general hash handle. This will be
  *              used by the general purpose hash functions in order to perform
  *              the algorithm
  */
cmox_hash_handle_t *cmox_sha3_512_construct(cmox_sha3_handle_t *P_pThis);

/**
  * @brief SHAKE128 constructor
  *
  * @param P_pThis Pointer to the SHA3 handle to initialize
  * @return cmox_hash_handle_t* Pointer to a general hash handle. This will be
  *              used by the general purpose hash functions in order to perform
  *              the algorithm
  */
cmox_hash_handle_t *cmox_shake128_construct(cmox_sha3_handle_t *P_pThis);

/**
  * @brief SHAKE256 constructor
  *
  * @param P_pThis Pointer to the SHA3 handle to initialize
  * @return cmox_hash_handle_t* Pointer to a general hash handle. This will be
  *              used by the general purpose hash functions in order to perform
  *              the algorithm
  */
cmox_hash_handle_t *cmox_shake256_construct(cmox_sha3_handle_t *P_pThis);

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

#endif /* CMOX_SHA3_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
