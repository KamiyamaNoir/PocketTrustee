/**
  ******************************************************************************
  * @file    cmox_keywrap.h
  * @author  MCD Application Team
  * @brief   Header file for the KEYWRAP cipher definitions and functions
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
#ifndef CMOX_KEYWRAP_H
#define CMOX_KEYWRAP_H

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* Include files -------------------------------------------------------------*/
#include "cmox_cipher.h"
#include "cmox_blockcipher.h"

#include "cmox_check_default_aes.h"
#include "cmox_default_defs.h"

/** @addtogroup CMOX_CIPHER
  * @{
  */

/** @defgroup CMOX_KEYWRAP KEYWRAP cipher
  * @{
  */

/* Macros --------------------------------------------------------------------*/

/** @defgroup CMOX_KEYWRAP_PUBLIC_MACROS KEYWRAP public macros
  * @{
  */

/**
  * @brief Size in bytes of the keywrap tag
  */
#define CMOX_KEYWRAP_TAG_LEN 8U

/**
  * @}
  */

/* Public types --------------------------------------------------------------*/

/** @defgroup CMOX_KEYWRAP_PUBLIC_TYPES KEYWRAP public types
  * @{
  */

/**
  * @brief KEYWRAP mode implementation
  *
  * This type specifies the used block cipher for the KEYWRAP construct and if
  * the algorithm will be used for encryption or decryption.
  * This type is defined as a pointer to a structure, that contains the
  * functions needed for the specific implementation, defined in the library
  * internally
  */
typedef const struct cmox_keywrap_implStruct_st *cmox_keywrap_impl_t;

/**
  * @brief KEYWRAP handle structure definition
  */
typedef struct
{
  cmox_cipher_handle_t super;             /*!< General cipher handle */
  cmox_blockcipher_handle_t blockCipher;  /*!< Block cipher handle */
  uint32_t iv[CMOX_CIPHER_BLOCK_SIZE];    /*!< Buffer containing the Initialization Vector */
} cmox_keywrap_handle_t;

/**
  * @}
  */

/* Public constants ----------------------------------------------------------*/

/** @defgroup CMOX_KEYWRAP_PUBLIC_CONSTANTS KEYWRAP public constants
  * @{
  */

/** @defgroup CMOX_KEYWRAP_IMPL KEYWRAP implementations
  * @{
  */

/**
  * @brief Implementation of KEYWRAP encryption using AES (fast implementation)
  *        (Defined internally)
  */
extern const cmox_keywrap_impl_t CMOX_AESFAST_KEYWRAP_ENC;

/**
  * @brief Implementation of KEYWRAP decryption using AES (fast implementation)
  *        (Defined internally)
  */
extern const cmox_keywrap_impl_t CMOX_AESFAST_KEYWRAP_DEC;

/**
  * @brief Implementation of KEYWRAP encryption using AES (small implementation)
  *        (Defined internally)
  */
extern const cmox_keywrap_impl_t CMOX_AESSMALL_KEYWRAP_ENC;

/**
  * @brief Implementation of KEYWRAP decryption using AES (small implementation)
  *        (Defined internally)
  */
extern const cmox_keywrap_impl_t CMOX_AESSMALL_KEYWRAP_DEC;

/**
  * @}
  */

/** @defgroup CMOX_KEYWRAP_ALGO KEYWRAP single-call algorithms
  * @{
  */

/**
  * @brief Identifier of the KEYWRAP encryption using AES (small implementation)
     for single-call function (Defined internally)
  */
extern const cmox_cipher_algo_t CMOX_AESSMALL_KEYWRAP_ENC_ALGO;

/**
  * @brief Identifier of the KEYWRAP decryption using AES (small implementation)
     for single-call function (Defined internally)
  */
extern const cmox_cipher_algo_t CMOX_AESSMALL_KEYWRAP_DEC_ALGO;

/**
  * @brief Identifier of the KEYWRAP encryption using AES (fast implementation)
     for single-call function (Defined internally)
  */
extern const cmox_cipher_algo_t CMOX_AESFAST_KEYWRAP_ENC_ALGO;

/**
  * @brief Identifier of the KEYWRAP decryption using AES (fast implementation)
     for single-call function (Defined internally)
  */
extern const cmox_cipher_algo_t CMOX_AESFAST_KEYWRAP_DEC_ALGO;

/**
  * @}
  */

/**
  * @}
  */

/* Public methods prototypes -------------------------------------------------*/

/** @defgroup CMOX_KEYWRAP_PUBLIC_METHODS KEYWRAP public method prototypes
  * @{
  */

/**
  * @brief KEYWRAP constructor
  *
  * The function is used for specifying which block cipher algorithm to use in
  * order to implement the KEYWRAP algorithm and if the algorithm will be used for
  * encryption or decryption.
  *
  * @param P_pThis Pointer to the KEYWRAP handle to initialize
  * @param P_impl  Constant that specifies the implementation to use.
  *                This parameter can be one of the following values:
  *                @arg CMOX_AESFAST_KEYWRAP_ENC
  *                @arg CMOX_AESFAST_KEYWRAP_DEC
  *                @arg CMOX_AESSMALL_KEYWRAP_ENC
  *                @arg CMOX_AESSMALL_KEYWRAP_DEC
  * @return cmox_cipher_handle_t* Pointer to a general cipher handle. This will
  *                be used by the general purpose cipher functions in order to
  *                perform the algorithm
  */
cmox_cipher_handle_t *cmox_keywrap_construct(cmox_keywrap_handle_t *P_pThis,
                                             cmox_keywrap_impl_t P_impl);

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

#endif /* CMOX_KEYWRAP_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
