/**
  ******************************************************************************
  * @file    cmox_cfb.h
  * @author  MCD Application Team
  * @brief   Header file for the CFB cipher definitions and functions
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
#ifndef CMOX_CFB_H
#define CMOX_CFB_H

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* Include files -------------------------------------------------------------*/
#include "cmox_cipher.h"
#include "cmox_blockcipher.h"

#include "cmox_default_defs.h"
#include "cmox_check_default_aes.h"

/** @addtogroup CMOX_CIPHER
  * @{
  */

/** @defgroup CMOX_CFB CFB cipher
  * @{
  */

/* Public macros -------------------------------------------------------------*/
/* Public types --------------------------------------------------------------*/

/** @defgroup CMOX_CFB_PUBLIC_TYPES CFB public types
  * @{
  */

/**
  * @brief CFB mode implementation
  *
  * This type specifies the used block cipher for the CFB construct and if the
  * algorithm will be used for encryption or decryption.
  * This type is defined as a pointer to a structure, that contains the
  * functions needed for the specific implementation, defined in the library
  * internally
  */
typedef const struct cmox_cfb_implStruct_st *cmox_cfb_impl_t;

/**
  * @brief CFB handle structure definition
  */
typedef struct
{
  cmox_cipher_handle_t super;  /*!< General cipher handle */

  cmox_blockcipher_handle_t blockCipher;  /*!< Block cipher handle */
  uint32_t iv[CMOX_CIPHER_BLOCK_SIZE];    /*!< Buffer containing the Initialization Vector */
  size_t cfb_blockLen;                    /*!< Size of the CFB block. Currently only 16 bytes block supported */
} cmox_cfb_handle_t;

/**
  * @}
  */

/* Public constants ----------------------------------------------------------*/

/** @defgroup CMOX_CFB_PUBLIC_CONSTANTS CFB public constants
  * @{
  */

/** @defgroup CMOX_CFB_IMPL CFB implementations
  * @{
  */

/**
  * @brief Implementation of CFB encryption using AES (fast implementation)
  *        (Defined internally)
  */
extern const cmox_cfb_impl_t CMOX_AESFAST_CFB_ENC;

/**
  * @brief Implementation of CFB decryption using AES (fast implementation)
  *        (Defined internally)
  */
extern const cmox_cfb_impl_t CMOX_AESFAST_CFB_DEC;

/**
  * @brief Implementation of CFB encryption using AES (small implementation)
  *        (Defined internally)
  */
extern const cmox_cfb_impl_t CMOX_AESSMALL_CFB_ENC;

/**
  * @brief Implementation of CFB decryption using AES (small implementation)
  *        (Defined internally)
  */
extern const cmox_cfb_impl_t CMOX_AESSMALL_CFB_DEC;

/**
  * @brief Implementation of CFB encryption using SM4 (Defined internally)
  */
extern const cmox_cfb_impl_t CMOX_SM4_CFB_ENC;

/**
  * @brief Implementation of CFB decryption using SM4 (Defined internally)
  */
extern const cmox_cfb_impl_t CMOX_SM4_CFB_DEC;

/**
  * @}
  */

/** @defgroup CMOX_CFB_ALGO CFB single-call algorithms
  * @{
  */

/**
  * @brief Identifier of the CFB encryption using AES (small implementation)
     for single-call function (Defined internally)
  */
extern const cmox_cipher_algo_t CMOX_AESSMALL_CFB_ENC_ALGO;

/**
  * @brief Identifier of the CFB decryption using AES (small implementation)
     for single-call function (Defined internally)
  */
extern const cmox_cipher_algo_t CMOX_AESSMALL_CFB_DEC_ALGO;

/**
  * @brief Identifier of the CFB encryption using AES (fast implementation)
     for single-call function (Defined internally)
  */
extern const cmox_cipher_algo_t CMOX_AESFAST_CFB_ENC_ALGO;

/**
  * @brief Identifier of the CFB decryption using AES (fast implementation)
     for single-call function (Defined internally)
  */
extern const cmox_cipher_algo_t CMOX_AESFAST_CFB_DEC_ALG;

/**
  * @brief Identifier of the CFB encryption using SM4
     for single-call function (Defined internally)
  */
extern const cmox_cipher_algo_t CMOX_SM4_CFB_ENC_ALGO;

/**
  * @brief Identifier of the CFB decryption using SM4
     for single-call function (Defined internally)
  */
extern const cmox_cipher_algo_t CMOX_SM4_CFB_DEC_ALGO;

/**
  * @}
  */

/**
  * @}
  */

/* Public methods prototypes -------------------------------------------------*/

/** @defgroup CMOX_CFB_PUBLIC_METHODS CFB public method prototypes
  * @{
  */

/**
  * @brief CFB constructor
  *
  * The function is used for specifying which block cipher algorithm to use in
  * order to implement the CFB algorithm and if the algorithm will be used for
  * encryption or decryption.
  *
  * @param P_pThis Pointer to the CFB handle to initialize
  * @param P_impl  Constant that specifies the implementation to use.
  *                This parameter can be one of the following values:
  *                @arg CMOX_AESFAST_CFB_ENC
  *                @arg CMOX_AESFAST_CFB_DEC
  *                @arg CMOX_AESSMALL_CFB_ENC
  *                @arg CMOX_AESSMALL_CFB_DEC
  *                @arg CMOX_SM4_CFB_ENC
  *                @arg CMOX_SM4_CFB_DEC
  * @return cmox_cipher_handle_t* Pointer to a general cipher handle. This will
  *                be used by the general purpose cipher functions in order to
  *                perform the algorithm
  */
cmox_cipher_handle_t *cmox_cfb_construct(cmox_cfb_handle_t *P_pThis,
                                         cmox_cfb_impl_t P_impl);

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

#endif /* CMOX_CFB_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
