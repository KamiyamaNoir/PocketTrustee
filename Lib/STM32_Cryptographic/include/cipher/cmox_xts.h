/**
  ******************************************************************************
  * @file    cmox_xts.h
  * @author  MCD Application Team
  * @brief   Header file for the XTS cipher
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
#ifndef CMOX_XTS_H
#define CMOX_XTS_H

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

/** @defgroup CMOX_XTS XTS cipher
  * @{
  */

/* Public macros -------------------------------------------------------------*/
/* Public types --------------------------------------------------------------*/

/** @defgroup CMOX_XTS_PUBLIC_TYPES XTS public types
  * @{
  */

/**
  * @brief XTS mode implementation
  *
  * This type specifies the used block cipher for the XTS construct and if the
  * algorithm will be used for encryption or decryption.
  * This type is defined as a pointer to a structure, that contains the
  * functions needed for the specific implementation, defined in the library
  * internally
  */
typedef const struct cmox_xts_implStruct_st *cmox_xts_impl_t;

/**
  * @brief XTS handle structure definition
  */
typedef struct
{
  cmox_cipher_handle_t super;             /*!< General cipher handle */
  cmox_blockcipher_handle_t blockCipher1; /*!< Block cipher handle */
  cmox_blockcipher_handle_t blockCipher2; /*!< Block cipher handle */
  uint32_t tweak[CMOX_CIPHER_BLOCK_SIZE]; /*!< Temporary result/tweak */
} cmox_xts_handle_t;

/**
  * @}
  */

/* Public constants ----------------------------------------------------------*/

/** @defgroup CMOX_XTS_PUBLIC_CONSTANTS XTS public constants
  * @{
  */

/** @defgroup CMOX_XTS_IMPL XTS implementations
  * @{
  */

/**
  * @brief Implementation of XTS encryption using AES (fast implementation)
  *        (Defined internally)
  */
extern const cmox_xts_impl_t CMOX_AESFAST_XTS_ENC;

/**
  * @brief Implementation of XTS decryption using AES (fast implementation)
  *        (Defined internally)
  */
extern const cmox_xts_impl_t CMOX_AESFAST_XTS_DEC;

/**
  * @brief Implementation of XTS encryption using AES (small implementation)
  *        (Defined internally)
  */
extern const cmox_xts_impl_t CMOX_AESSMALL_XTS_ENC;

/**
  * @brief Implementation of XTS decryption using AES (small implementation)
  *        (Defined internally)
  */
extern const cmox_xts_impl_t CMOX_AESSMALL_XTS_DEC;

/**
  * @}
  */

/** @defgroup CMOX_XTS_ALGO XTS single-call algorithms
  * @{
  */

/**
  * @brief Identifier of the XTS encryption using AES (small implementation)
     for single-call function (Defined internally)
  */
extern const cmox_cipher_algo_t CMOX_AESSMALL_XTS_ENC_ALGO;

/**
  * @brief Identifier of the XTS decryption using AES (small implementation)
     for single-call function (Defined internally)
  */
extern const cmox_cipher_algo_t CMOX_AESSMALL_XTS_DEC_ALGO;

/**
  * @brief Identifier of the XTS encryption using AES (fast implementation)
     for single-call function (Defined internally)
  */
extern const cmox_cipher_algo_t CMOX_AESFAST_XTS_ENC_ALGO;

/**
  * @brief Identifier of the XTS decryption using AES (fast implementation)
     for single-call function (Defined internally)
  */
extern const cmox_cipher_algo_t CMOX_AESFAST_XTS_DEC_ALGO;

/**
  * @}
  */

/**
  * @}
  */

/* Public method prototypes --------------------------------------------------*/

/** @defgroup CMOX_XTS_PUBLIC_METHODS XTS public method prototypes
  * @{
  */

/**
  * @brief XTS constructor
  *
  * The function is used for specifying which block cipher algorithm to use in
  * order to implement the XTS algorithm and if the algorithm will be used for
  * encryption or decryption.
  *
  * @param P_pThis Pointer to the XTS handle to initialize
  * @param P_impl  Constant that specifies the implementation to use.
  *                This parameter can be one of the following values:
  *                @arg CMOX_AESFAST_XTS_ENC
  *                @arg CMOX_AESFAST_XTS_DEC
  *                @arg CMOX_AESSMALL_XTS_ENC
  *                @arg CMOX_AESSMALL_XTS_DEC
  * @return cmox_cipher_handle_t* Pointer to a general cipher handle. This will
  *                be used by the general purpose cipher functions in order to
  *                perform the algorithm
  */
cmox_cipher_handle_t *cmox_xts_construct(cmox_xts_handle_t *P_pThis,
                                         cmox_xts_impl_t P_impl);

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

#endif /* CMOX_XTS_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
