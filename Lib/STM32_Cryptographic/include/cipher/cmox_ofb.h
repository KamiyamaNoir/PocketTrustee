/**
  ******************************************************************************
  * @file    cmox_ofb.h
  * @author  MCD Application Team
  * @brief   Header file for the OFB cipher definitions and functions
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
#ifndef CMOX_OFB_H
#define CMOX_OFB_H

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

/** @defgroup CMOX_OFB OFB cipher
  * @{
  */

/* Public macros -------------------------------------------------------------*/
/* Public types --------------------------------------------------------------*/

/** @defgroup CMOX_OFB_PUBLIC_TYPES OFB public types
  * @{
  */

/**
  * @brief OFB mode implementation
  *
  * This type specifies the used block cipher for the OFB construct and if the
  * algorithm will be used for encryption or decryption.
  * This type is defined as a pointer to a structure, that contains the
  * functions needed for the specific implementation, defined in the library
  * internally
  */
typedef const struct cmox_ofb_implStruct_st *cmox_ofb_impl_t;

/**
  * @brief OFB handle structure definition
  */
typedef struct
{
  cmox_cipher_handle_t super;             /*!< General cipher handle */
  cmox_blockcipher_handle_t blockCipher;  /*!< Block cipher handle */
  uint32_t iv[CMOX_CIPHER_BLOCK_SIZE];    /*!< Buffer containing the Initialization Vector */
} cmox_ofb_handle_t;

/**
  * @}
  */

/* Public constants ----------------------------------------------------------*/

/** @defgroup CMOX_OFB_PUBLIC_CONSTANTS OFB public constants
  * @{
  */

/** @defgroup CMOX_OFB_IMPL OFB implementations
  * @{
  */

/**
  * @brief Implementation of OFB encryption using AES (fast implementation)
  *        (Defined internally)
  */
extern const cmox_ofb_impl_t CMOX_AESFAST_OFB_ENC;

/**
  * @brief Implementation of OFB decryption using AES (fast implementation)
  *        (Defined internally)
  */
extern const cmox_ofb_impl_t CMOX_AESFAST_OFB_DEC;

/**
  * @brief Implementation of OFB encryption using AES (small implementation)
  *        (Defined internally)
  */
extern const cmox_ofb_impl_t CMOX_AESSMALL_OFB_ENC;

/**
  * @brief Implementation of OFB decryption using AES (small implementation)
  *        (Defined internally)
  */
extern const cmox_ofb_impl_t CMOX_AESSMALL_OFB_DEC;

/**
  * @brief Implementation of OFB encryption using SM4 (Defined internally)
  */
extern const cmox_ofb_impl_t CMOX_SM4_OFB_ENC;

/**
  * @brief Implementation of OFB decryption using SM4 (Defined internally)
  */
extern const cmox_ofb_impl_t CMOX_SM4_OFB_DEC;

/**
  * @}
  */

/** @defgroup CMOX_OFB_ALGO OFB single-call algorithms
  * @{
  */

/**
  * @brief Identifier of the OFB encryption using AES (small implementation)
     for single-call function (Defined internally)
  */
extern const cmox_cipher_algo_t CMOX_AESSMALL_OFB_ENC_ALGO;

/**
  * @brief Identifier of the OFB decryption using AES (small implementation)
     for single-call function (Defined internally)
  */
extern const cmox_cipher_algo_t CMOX_AESSMALL_OFB_DEC_ALGO;

/**
  * @brief Identifier of the OFB encryption using AES (fast implementation)
     for single-call function (Defined internally)
  */
extern const cmox_cipher_algo_t CMOX_AESFAST_OFB_ENC_ALGO;

/**
  * @brief Identifier of the OFB decryption using AES (fast implementation)
     for single-call function (Defined internally)
  */
extern const cmox_cipher_algo_t CMOX_AESFAST_OFB_DEC_ALGO;

/**
  * @brief Identifier of the OFB encryption using SM4
     for single-call function (Defined internally)
  */
extern const cmox_cipher_algo_t CMOX_SM4_OFB_ENC_ALGO;

/**
  * @brief Identifier of the OFB decryption using SM4
     for single-call function (Defined internally)
  */
extern const cmox_cipher_algo_t CMOX_SM4_OFB_DEC_ALGO;

/**
  * @}
  */

/**
  * @}
  */

/* Public methods prototypes -------------------------------------------------*/

/** @defgroup CMOX_OFB_PUBLIC_METHODS OFB public method prototypes
  * @{
  */

/**
  * @brief OFB constructor
  *
  * The function is used for specifying which block cipher algorithm to use in
  * order to implement the OFB algorithm and if the algorithm will be used for
  * encryption or decryption.
  *
  * @param P_pThis Pointer to the OFB handle to initialize
  * @param P_impl  Constant that specifies the implementation to use.
  *                This parameter can be one of the following values:
  *                @arg CMOX_AESFAST_OFB_ENC
  *                @arg CMOX_AESFAST_OFB_DEC
  *                @arg CMOX_AESSMALL_OFB_ENC
  *                @arg CMOX_AESSMALL_OFB_DEC
  *                @arg CMOX_SM4_OFB_ENC
  *                @arg CMOX_SM4_OFB_DEC
  * @return cmox_cipher_handle_t* Pointer to a general cipher handle. This will
  *                be used by the general purpose cipher functions in order to
  *                perform the algorithm
  */
cmox_cipher_handle_t *cmox_ofb_construct(cmox_ofb_handle_t *P_pThis,
                                         cmox_ofb_impl_t P_impl);

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

#endif /* CMOX_OFB_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
