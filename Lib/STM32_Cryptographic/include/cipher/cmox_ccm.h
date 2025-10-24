/**
  ******************************************************************************
  * @file    cmox_ccm.h
  * @author  MCD Application Team
  * @brief   Header file for the CCM AEAD cipher definitions and functions
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
#ifndef CMOX_CCM_H
#define CMOX_CCM_H

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

/** @defgroup CMOX_CCM CCM cipher
  * @{
  */

/* Public macros -------------------------------------------------------------*/


/* Public types --------------------------------------------------------------*/

/** @defgroup CMOX_CCM_PUBLIC_TYPES CCM public types
  * @{
  */

/**
  * @brief CCM mode implementation
  *
  * This type specifies the used block cipher for the CCM construct and if the
  * algorithm will be used for encryption or decryption.
  * This type is defined as a pointer to a structure, that contains the
  * functions needed for the specific implementation, defined in the library
  * internally
  */
typedef const struct cmox_ccm_implStruct_st *cmox_ccm_impl_t;

/**
  * @brief CCM handle structure
  */
typedef struct
{
  cmox_cipher_handle_t super;  /*!< General cipher handle */
  cmox_blockcipher_handle_t blockCipher;  /*!< Block cipher handle */
  uint32_t ivCtr[4];  /*!< Current IV value for encryption.*/
  uint32_t ivCbc[4];  /*!< Current IV value for authentication.*/
  size_t tagLen;      /*!< Size of the tag to return. Possible values are values are {4,6,8,10,12,14,16} */
  size_t AdLen;       /*!< Len of the associated data to be processed */
  size_t payloadLen;  /*!< Size of the total payload data to be processed */
  size_t nonceLen;    /*!< Size of the Nonce in bytes. Possible values are {7,8,9,10,11,12,13}  */
  uint32_t tmpBuf[4]; /*!< Temporary buffer used for internal computations */
  uint32_t tmpBufUse; /*!< Number of bytes in use for internal computations */
} cmox_ccm_handle_t;

/**
  * @}
  */

/* Public constants ----------------------------------------------------------*/

/** @defgroup CMOX_CCM_PUBLIC_CONSTANTS CCM public constants
  * @{
  */

/** @defgroup CMOX_CCM_IMPL CCM implementations
  * @{
  */

/**
  * @brief Implementation of CCM encryption using AES (fast implementation)
  *        (Defined internally)
  */
extern const cmox_ccm_impl_t CMOX_AESFAST_CCM_ENC;

/**
  * @brief Implementation of CCM decryption using AES (fast implementation)
  *        (Defined internally)
  */
extern const cmox_ccm_impl_t CMOX_AESFAST_CCM_DEC;

/**
  * @brief Implementation of CCM encryption using AES (small implementation)
  *        (Defined internally)
  */
extern const cmox_ccm_impl_t CMOX_AESSMALL_CCM_ENC;

/**
  * @brief Implementation of CCM decryption using AES (small implementation)
  *        (Defined internally)
  */
extern const cmox_ccm_impl_t CMOX_AESSMALL_CCM_DEC;

/**
  * @}
  */

/** @defgroup CMOX_CCM_ALGO CCM single-call algorithms
  * @{
  */

/**
  * @brief Identifier of the CCM encryption using AES (small implementation)
     for single-call function (Defined internally)
  */
extern const cmox_aead_algo_t CMOX_AESSMALL_CCM_ENC_ALGO;

/**
  * @brief Identifier of the CCM decryption using AES (small implementation)
     for single-call function (Defined internally)
  */
extern const cmox_aead_algo_t CMOX_AESSMALL_CCM_DEC_ALGO;

/**
  * @brief Identifier of the CCM encryption using AES (fast implementation)
     for single-call function (Defined internally)
  */
extern const cmox_aead_algo_t CMOX_AESFAST_CCM_ENC_ALGO;

/**
  * @brief Identifier of the CCM decryption using AES (fast implementation)
     for single-call function (Defined internally)
  */
extern const cmox_aead_algo_t CMOX_AESFAST_CCM_DEC_ALGO;

/**
  * @}
  */

/**
  * @}
  */

/* Public methods prototypes -------------------------------------------------*/

/** @defgroup CMOX_CCM_PUBLIC_METHODS CCM public method prototypes
  * @{
  */

/**
  * @brief CCM constructor
  *
  * The function is used for specifying which block cipher algorithm to use in
  * order to implement the CCM algorithm and if the algorithm will be used for
  * encryption or decryption.
  *
  * @param P_pThis Pointer to the CCM handle to initialize
  * @param P_impl  Constant that specifies the implementation to use.
  *                This parameter can be one of the following values:
  *                @arg CMOX_AESFAST_CCM_ENC
  *                @arg CMOX_AESFAST_CCM_DEC
  *                @arg CMOX_AESSMALL_CCM_ENC
  *                @arg CMOX_AESSMALL_CCM_DEC
  * @return cmox_cipher_handle_t* Pointer to a general cipher handle. This will
  *                be used by the general purpose cipher functions in order to
  *                perform the algorithm
  */
cmox_cipher_handle_t *cmox_ccm_construct(cmox_ccm_handle_t *P_pThis,
                                         cmox_ccm_impl_t P_impl);

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

#endif /* CMOX_CCM_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
