/**
  ******************************************************************************
  * @file    cmox_ctr_drbg.h
  * @author  MCD Application Team
  * @brief   Header file for the CTR-DRBG module
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
#ifndef CMOX_CTR_DRBG_H
#define CMOX_CTR_DRBG_H

/* Include files -------------------------------------------------------------*/
#include "cmox_drbg.h"
#include "cipher/cmox_blockcipher.h"

#include "cipher/cmox_check_default_aes.h"
#include "cmox_default_defs.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/** @addtogroup CMOX_DRBG
  * @{
  */

/** @defgroup CMOX_CTR_DRBG CTR-DRBG
  *  @{
  */

/* Public macros -------------------------------------------------------------*/
/* Public types --------------------------------------------------------------*/

/** @defgroup CMOX_CTR_DRBG_PUBLIC_TYPES CTR-DRBG public types
  * @{
  */

/**
  * @brief CTR-DRBG mode implementation
  *
  * This type specifies the used block cipher for the CTR-DRBG construct.
  * This type is defined as a pointer to a structure, that contains the
  * functions needed for the specific implementation, defined in the library
  * internally
  */
typedef const struct cmox_ctr_drbg_implStruct_st *cmox_ctr_drbg_impl_t;

/**
  * @brief  Structure to store the state/context of the CTR_DRBG
  */
typedef struct
{
  uint32_t value[4];  /*!<  V value, a 128 bit value */
  uint32_t key[8];    /*!<  K value, contains the block cipher key */
  uint64_t reseed_counter;  /*!< Reseed counter 32-bit of data */
} cmox_ctr_drbg_state_t;

/**
  * @brief CTR-DRBG handle structure definition
  */
typedef struct
{
  cmox_drbg_handle_t super;                 /*!< General DRBG handle */
  cmox_blockcipher_handle_t blockCipher;    /*!< Block cipher handle */
  cmox_blockcipher_handle_t blockCipher_df; /*!< Block cipher handle for derivation function */
  cmox_ctr_drbg_state_t state;              /*!< DRBG state */
  uint32_t flag;                            /*!< DRBG flag */
  cmox_cipher_keyLen_t keyLen;              /*!< Length in bytes of the block cipher key */
  size_t expKeyLen;                         /*!< Length in bytes of the expanded key */
  size_t minEntropyLen;                     /*!< Minimum entropy length */
  size_t maxBytesPerRequest;                /*!< Maximum number of random bytes per request */
  const uint32_t *exp_aes_0_key;            /*!< Pointer to the expanded key */
  const uint32_t *exp_bcc_aes_key;          /*!< Pointer to the expanded key for BCC */
} cmox_ctr_drbg_handle_t;

/**
  * @}
  */

/* Public constants ----------------------------------------------------------*/

/** @defgroup CMOX_CTR_DRBG_PUBLIC_CONSTANTS CTR-DRBG public constants
  * @{
  */

/** @defgroup CMOX_CTR_DRBG_IMPL CTR-DRBG implementations
  * @{
  */

/**
  * @brief Implementation of CTR-DRBG using AES-128 (small implementation)
  *        (Defined internally)
  */
extern const cmox_ctr_drbg_impl_t CMOX_CTR_DRBG_AES128_SMALL;

/**
  * @brief Implementation of CTR-DRBG using AES-128 (fast implementation)
  *        (Defined internally)
  */
extern const cmox_ctr_drbg_impl_t CMOX_CTR_DRBG_AES128_FAST;

/**
  * @brief Implementation of CTR-DRBG using AES-256 (small implementation)
  *        (Defined internally)
  */
extern const cmox_ctr_drbg_impl_t CMOX_CTR_DRBG_AES256_SMALL;

/**
  * @brief Implementation of CTR-DRBG using AES-256 (fast implementation)
  *        (Defined internally)
  */
extern const cmox_ctr_drbg_impl_t CMOX_CTR_DRBG_AES256_FAST;

/**
  * @}
  */

/**
  * @}
  */

/* Public method prototypes --------------------------------------------------*/

/** @defgroup CMOX_CTR_DRBG_PUBLIC_METHODS CTR-DRBG public method prototypes
  * @{
  */

/**
  * @brief CTR-DRBG constructor
  *
  * The function is used for specifying which block cipher algorithm to use in
  * order to implement the CTR-DRBG algorithm
  * @param P_pThis Pointer to the CTR-DRBG handle to initialize
  * @param P_impl  Constant that specifies the implementation to use.
  *                This parameter can be one of the following values:
  *                @arg CMOX_CTR_DRBG_AES128_FAST
  *                @arg CMOX_CTR_DRBG_AES192_FAST
  *                @arg CMOX_CTR_DRBG_AES256_FAST
  *                @arg CMOX_CTR_DRBG_AES128_SMALL
  *                @arg CMOX_CTR_DRBG_AES192_SMALL
  *                @arg CMOX_CTR_DRBG_AES256_SMALL
  * @return cmox_drbg_handle_t* Pointer to a general DRBG handle. This will
  *                be used by the general purpose cipher functions in order to
  *                perform the algorithm
  */
cmox_drbg_handle_t *cmox_ctr_drbg_construct(cmox_ctr_drbg_handle_t *P_pThis,
                                            cmox_ctr_drbg_impl_t P_impl);

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

#endif /* CMOX_CTR_DRBG_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
