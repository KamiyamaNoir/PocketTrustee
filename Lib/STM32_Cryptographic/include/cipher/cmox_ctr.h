/**
  ******************************************************************************
  * @file    cmox_ctr.h
  * @author  MCD Application Team
  * @brief   Header file for the CTR cipher definitions and functions
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
#ifndef CMOX_CTR_H
#define CMOX_CTR_H

/* Include files -------------------------------------------------------------*/
#include "cmox_cipher.h"
#include "cmox_blockcipher.h"

#include "cmox_check_default_aes.h"
#include "cmox_default_defs.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/** @addtogroup CMOX_CIPHER
  * @{
  */

/** @defgroup CMOX_CTR CTR cipher
  * @{
  */

/* Public macros -------------------------------------------------------------*/
/* Public types --------------------------------------------------------------*/

/** @defgroup CMOX_CTR_PUBLIC_TYPES CTR public types
  * @{
  */

/**
  * @brief CTR mode implementation
  *
  * This type specifies the used block cipher for the CTR construct and if the
  * algorithm will be used for encryption or decryption.
  * This type is defined as a pointer to a structure, that contains the
  * functions needed for the specific implementation, defined in the library
  * internally
  */
typedef const struct cmox_ctr_implStruct_st *cmox_ctr_impl_t;

/**
  * @brief CTR handle structure definition
  */
typedef struct
{
  cmox_cipher_handle_t super;             /*!< General cipher handle */
  cmox_blockcipher_handle_t blockCipher;  /*!< Block cipher handle */
  uint32_t iv[CMOX_CIPHER_BLOCK_SIZE];    /*!< Buffer containing the Initialization Vector */
} cmox_ctr_handle_t;

/**
  * @}
  */

/* Public constants ----------------------------------------------------------*/

/** @defgroup CMOX_CTR_PUBLIC_CONSTANTS CTR public constants
  * @{
  */

/** @defgroup CMOX_CTR_IMPL CTR implementations
  * @{
  */

/**
  * @brief Implementation of CTR encryption using AES (fast implementation)
  *        (Defined internally)
  */
extern const cmox_ctr_impl_t CMOX_AESFAST_CTR_ENC;

/**
  * @brief Implementation of CTR decryption using AES (fast implementation)
  *        (Defined internally)
  */
extern const cmox_ctr_impl_t CMOX_AESFAST_CTR_DEC;

/**
  * @brief Implementation of CTR encryption using AES (small implementation)
  *        (Defined internally)
  */
extern const cmox_ctr_impl_t CMOX_AESSMALL_CTR_ENC;

/**
  * @brief Implementation of CTR decryption using AES (small implementation)
  *        (Defined internally)
  */
extern const cmox_ctr_impl_t CMOX_AESSMALL_CTR_DEC;

/**
  * @brief Implementation of CTR encryption using SM4 (Defined internally)
  */
extern const cmox_ctr_impl_t CMOX_SM4_CTR_ENC;

/**
  * @brief Implementation of CTR decryption using SM4 (Defined internally)
  */
extern const cmox_ctr_impl_t CMOX_SM4_CTR_DEC;

/**
  * @}
  */

/** @defgroup CMOX_CTR_ALGO CTR single-call algorithms
  * @{
  */

/**
  * @brief Identifier of the CTR encryption using AES (small implementation)
     for single-call function (Defined internally)
  */
extern const cmox_cipher_algo_t CMOX_AESSMALL_CTR_ENC_ALGO;

/**
  * @brief Identifier of the CTR decryption using AES (small implementation)
     for single-call function (Defined internally)
  */
extern const cmox_cipher_algo_t CMOX_AESSMALL_CTR_DEC_ALGO;

/**
  * @brief Identifier of the CTR encryption using AES (fast implementation)
     for single-call function (Defined internally)
  */
extern const cmox_cipher_algo_t CMOX_AESFAST_CTR_ENC_ALGO;

/**
  * @brief Identifier of the CTR decryption using AES (fast implementation)
     for single-call function (Defined internally)
  */
extern const cmox_cipher_algo_t CMOX_AESFAST_CTR_DEC_ALGO;

/**
  * @brief Identifier of the CTR encryption using SM4
     for single-call function (Defined internally)
  */
extern const cmox_cipher_algo_t CMOX_SM4_CTR_ENC_ALGO;

/**
  * @brief Identifier of the CTR decryption using SM4
     for single-call function (Defined internally)
  */
extern const cmox_cipher_algo_t CMOX_SM4_CTR_DEC_ALGO;

/**
  * @}
  */

/**
  * @}
  */

/* Public methods prototypes -------------------------------------------------*/

/** @defgroup CMOX_CTR_PUBLIC_METHODS CTR public method prototypes
  * @{
  */

/**
  * @brief CTR constructor
  *
  * The function is used for specifying which block cipher algorithm to use in
  * order to implement the CTR algorithm and if the algorithm will be used for
  * encryption or decryption.
  *
  * @param P_pThis Pointer to the CTR handle to initialize
  * @param P_impl  Constant that specifies the implementation to use.
  *                This parameter can be one of the following values:
  *                @arg CMOX_AESFAST_CTR_ENC
  *                @arg CMOX_AESFAST_CTR_DEC
  *                @arg CMOX_AESSMALL_CTR_ENC
  *                @arg CMOX_AESSMALL_CTR_DEC
  *                @arg CMOX_SM4_CTR_ENC
  *                @arg CMOX_SM4_CTR_DEC
  * @return cmox_cipher_handle_t* Pointer to a general cipher handle. This will
  *                be used by the general purpose cipher functions in order to
  *                perform the algorithm
  */
cmox_cipher_handle_t *cmox_ctr_construct(cmox_ctr_handle_t *P_pThis,
                                         cmox_ctr_impl_t P_impl);

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

#endif /* CMOX_CTR_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
