/**
  ******************************************************************************
  * @file    cmox_cbc.h
  * @author  MCD Application Team
  * @brief   Header file for the CBC cipher definitions and functions
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
#ifndef CMOX_CBC_H
#define CMOX_CBC_H

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

/** @defgroup CMOX_CBC CBC cipher
  * @{
  */

/* Public macros -------------------------------------------------------------*/
/* Public types --------------------------------------------------------------*/

/** @defgroup CMOX_CBC_PUBLIC_TYPES CBC public types
  * @{
  */

/**
  * @brief CBC mode implementation
  *
  * This type specifies the used block cipher for the CBC construct and if the
  * algorithm will be used for encryption or decryption.
  * This type is defined as a pointer to a structure, that contains the
  * functions needed for the specific implementation, defined in the library
  * internally
  */
typedef const struct cmox_cbc_implStruct_st *cmox_cbc_impl_t;

/**
  * @brief CBC handle structure definition
  */
typedef struct
{
  cmox_cipher_handle_t super;             /*!< General cipher handle */
  cmox_blockcipher_handle_t blockCipher;  /*!< Block cipher handle */
  uint32_t iv[CMOX_CIPHER_BLOCK_SIZE];    /*!< Buffer containing the Initialization Vector */
} cmox_cbc_handle_t;

/**
  * @}
  */

/* Public constants ----------------------------------------------------------*/

/** @defgroup CMOX_CBC_PUBLIC_CONSTANTS CBC public constants
  * @{
  */

/** @defgroup CMOX_CBC_IMPL CBC implementations
  * @{
  */

/**
  * @brief Implementation of CBC encryption using AES (fast implementation)
  *        (Defined internally)
  */
extern const cmox_cbc_impl_t CMOX_AESFAST_CBC_ENC;

/**
  * @brief Implementation of CBC decryption using AES (fast implementation)
  *        (Defined internally)
  */
extern const cmox_cbc_impl_t CMOX_AESFAST_CBC_DEC;

/**
  * @brief Implementation of CBC encryption using AES (small implementation)
  *        (Defined internally)
  */
extern const cmox_cbc_impl_t CMOX_AESSMALL_CBC_ENC;

/**
  * @brief Implementation of CBC decryption using AES (small implementation)
  *        (Defined internally)
  */
extern const cmox_cbc_impl_t CMOX_AESSMALL_CBC_DEC;

/**
  * @brief Implementation of CBC encryption using SM4 (Defined internally)
  */
extern const cmox_cbc_impl_t CMOX_SM4_CBC_ENC;

/**
  * @brief Implementation of CBC decryption using SM4 (Defined internally)
  */
extern const cmox_cbc_impl_t CMOX_SM4_CBC_DEC;

/**
  * @}
  */

/** @defgroup CMOX_CBC_ALGO CBC single-call algorithms
  * @{
  */

/**
  * @brief Identifier of the CBC encryption using AES (small implementation)
     for single-call function (Defined internally)
  */
extern const cmox_cipher_algo_t CMOX_AESSMALL_CBC_ENC_ALGO;

/**
  * @brief Identifier of the CBC decryption using AES (small implementation)
     for single-call function (Defined internally)
  */
extern const cmox_cipher_algo_t CMOX_AESSMALL_CBC_DEC_ALGO;

/**
  * @brief Identifier of the CBC encryption using AES (fast implementation)
     for single-call function (Defined internally)
  */
extern const cmox_cipher_algo_t CMOX_AESFAST_CBC_ENC_ALGO;

/**
  * @brief Identifier of the CBC decryption using AES (fast implementation)
     for single-call function (Defined internally)
  */
extern const cmox_cipher_algo_t CMOX_AESFAST_CBC_DEC_ALGO;

/**
  * @brief Identifier of the CBC encryption using SM4
     for single-call function (Defined internally)
  */
extern const cmox_cipher_algo_t CMOX_SM4_CBC_ENC_ALGO;

/**
  * @brief Identifier of the CBC decryption using SM4
     for single-call function (Defined internally)
  */
extern const cmox_cipher_algo_t CMOX_SM4_CBC_DEC_ALGO;

/**
  * @}
  */

/**
  * @}
  */

/* Public method prototypes --------------------------------------------------*/

/** @defgroup CMOX_CBC_PUBLIC_METHODS CBC public method prototypes
  * @{
  */

/**
  * @brief CBC constructor
  *
  * The function is used for specifying which block cipher algorithm to use in
  * order to implement the CBC algorithm and if the algorithm will be used for
  * encryption or decryption.
  *
  * @param P_pThis Pointer to the CBC handle to initialize
  * @param P_impl  Constant that specifies the implementation to use.
  *                This parameter can be one of the following values:
  *                @arg CMOX_AESFAST_CBC_ENC
  *                @arg CMOX_AESFAST_CBC_DEC
  *                @arg CMOX_AESSMALL_CBC_ENC
  *                @arg CMOX_AESSMALL_CBC_DEC
  *                @arg CMOX_SM4_CBC_ENC
  *                @arg CMOX_SM4_CBC_DEC
  * @return cmox_cipher_handle_t* Pointer to a general cipher handle. This will
  *                be used by the general purpose cipher functions in order to
  *                perform the algorithm
  */
cmox_cipher_handle_t *cmox_cbc_construct(cmox_cbc_handle_t *P_pThis,
                                         cmox_cbc_impl_t P_impl);

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

#endif /* CMOX_CBC_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
