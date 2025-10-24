/**
  ******************************************************************************
  * @file    cmox_ecb.h
  * @author  MCD Application Team
  * @brief   Header file for the ECB cipher definitions and functions
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
#ifndef CMOX_ECB_H
#define CMOX_ECB_H

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

/** @defgroup CMOX_ECB ECB cipher
  * @{
  */

/* Public macros -------------------------------------------------------------*/
/* Public types --------------------------------------------------------------*/

/** @defgroup CMOX_ECB_PUBLIC_TYPES ECB public types
  * @{
  */

/**
  * @brief ECB mode implementation
  *
  * This type specifies the used block cipher for the ECB construct and if the
  * algorithm will be used for encryption or decryption.
  * This type is defined as a pointer to a structure, that contains the
  * functions needed for the specific implementation, defined in the library
  * internally
  */
typedef const struct cmox_ecb_implStruct_st *cmox_ecb_impl_t;

/**
  * @brief ECB handle structure definition
  */
typedef struct
{
  cmox_cipher_handle_t super;             /*!< General cipher handle */
  cmox_blockcipher_handle_t blockCipher;  /*!< Block cipher handle */
} cmox_ecb_handle_t;

/**
  * @}
  */

/* Public constants ----------------------------------------------------------*/

/** @defgroup CMOX_ECB_PUBLIC_CONSTANTS ECB public constants
  * @{
  */

/** @defgroup CMOX_ECB_IMPL ECB implementations
  * @{
  */

/**
  * @brief Implementation of ECB encryption using AES (fast implementation)
  *        (Defined internally)
  */
extern const cmox_ecb_impl_t CMOX_AESFAST_ECB_ENC;

/**
  * @brief Implementation of ECB decryption using AES (fast implementation)
  *        (Defined internally)
  */
extern const cmox_ecb_impl_t CMOX_AESFAST_ECB_DEC;

/**
  * @brief Implementation of ECB encryption using AES (small implementation)
  *        (Defined internally)
  */
extern const cmox_ecb_impl_t CMOX_AESSMALL_ECB_ENC;

/**
  * @brief Implementation of ECB decryption using AES (small implementation)
  *        (Defined internally)
  */
extern const cmox_ecb_impl_t CMOX_AESSMALL_ECB_DEC;

/**
  * @brief Implementation of ECB encryption using SM4 (Defined internally)
  */
extern const cmox_ecb_impl_t CMOX_SM4_ECB_ENC;

/**
  * @brief Implementation of ECB decryption using SM4 (Defined internally)
  */
extern const cmox_ecb_impl_t CMOX_SM4_ECB_DEC;

/**
  * @}
  */

/** @defgroup CMOX_ECB_ALGO ECB single-call algorithms
  * @{
  */

/**
  * @brief Identifier of the ECB encryption using AES (small implementation)
     for single-call function (Defined internally)
  */
extern const cmox_cipher_algo_t CMOX_AESSMALL_ECB_ENC_ALGO;

/**
  * @brief Identifier of the ECB decryption using AES (small implementation)
          for single-call function (Defined internally)
  */
extern const cmox_cipher_algo_t CMOX_AESSMALL_ECB_DEC_ALGO;

/**
  * @brief Identifier of the ECB encryption using AES (fast implementation)
          for single-call function (Defined internally)
  */
extern const cmox_cipher_algo_t CMOX_AESFAST_ECB_ENC_ALGO;

/**
  * @brief Identifier of the ECB decryption using AES (fast implementation)
          for single-call function (Defined internally)
  */
extern const cmox_cipher_algo_t CMOX_AESFAST_ECB_DEC_ALGO;

/**
  * @brief Identifier of the ECB encryption using SM4
          for single-call function (Defined internally)
  */
extern const cmox_cipher_algo_t CMOX_SM4_ECB_ENC_ALGO;

/**
  * @brief Identifier of the ECB decryption using SM4
          for single-call function (Defined internally)
  */
extern const cmox_cipher_algo_t CMOX_SM4_ECB_DEC_ALGO;

/**
  * @}
  */

/**
  * @}
  */

/* Public methods prototypes -------------------------------------------------*/

/** @defgroup CMOX_ECB_PUBLIC_METHODS ECB public method prototypes
  * @{
  */

/**
  * @brief ECB constructor
  *
  * The function is used for specifying which block cipher algorithm to use in
  * order to implement the ECB algorithm and if the algorithm will be used for
  * encryption or decryption.
  *
  * @param P_pThis Pointer to the ECB handle to initialize
  * @param P_impl  Constant that specifies the implementation to use.
  *                This parameter can be one of the following values:
  *                @arg CMOX_AESFAST_ECB_ENC
  *                @arg CMOX_AESFAST_ECB_DEC
  *                @arg CMOX_AESSMALL_ECB_ENC
  *                @arg CMOX_AESSMALL_ECB_DEC
  *                @arg CMOX_SM4_ECB_ENC
  *                @arg CMOX_SM4_ECB_DEC
  * @return cmox_cipher_handle_t* Pointer to a general cipher handle. This will
  *                be used by the general purpose cipher functions in order to
  *                perform the algorithm
  */
cmox_cipher_handle_t *cmox_ecb_construct(cmox_ecb_handle_t *P_pThis,
                                         cmox_ecb_impl_t P_impl);

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

#endif /* CMOX_ECB_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
