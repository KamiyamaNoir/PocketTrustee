/**
  ******************************************************************************
  * @file    cmox_chachapoly.h
  * @author  MCD Application Team
  * @brief   Header file for the ChaCha20-Poly1305 AEAD cipher functions
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
#ifndef CMOX_CHACHA20_POLY1305_H
#define CMOX_CHACHA20_POLY1305_H

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* Include files -------------------------------------------------------------*/
#include "cmox_cipher.h"

/** @addtogroup CMOX_CIPHER
  * @{
  */

/** @defgroup CMOX_CHACHAPOLY ChaCha20-Poly1305 cipher
  * @{
  */

/* Public types --------------------------------------------------------------*/

/** @defgroup CMOX_CHACHAPOLY_PUBLIC_TYPES ChaCha20-Poly1305 public types
  * @{
  */

/**
  * @brief ChaCha20-Poly1305 mode implementation
  *
  * This type specifies if the algorithm will be used for encryption or
  * decryption.
  * This type is defined as a pointer to a structure, that contains the
  * functions needed for the specific implementation, defined in the library
  * internally
  */
typedef const struct cmox_chachapoly_implStruct_st *cmox_chachapoly_impl_t;

/**
  * @brief  ChaCha20-Poly1305 handle structure definition
  */
typedef struct
{
  cmox_cipher_handle_t super; /*!< General cipher handle */
  size_t mAadLen;             /*!< Size of the processed AAD */
  size_t mCipherLen;          /*!< Size of the processed CipherText */
  uint32_t rValue[5];         /*!< Internal: value of r */
  uint32_t hValue[5];         /*!< Internal: value of h */
  uint32_t pad[4];            /*!< Internal: value of Poly nonce */
  uint32_t internalState[16]; /*!< Internal: ChaCha Internal State */
} cmox_chachapoly_handle_t;

/**
  * @}
  */

/* Public constants ----------------------------------------------------------*/

/** @defgroup CMOX_CHACHAPOLY_PUBLIC_CONSTANTS ChaCha20-Poly1305 public
  *                                             constants
  * @{
  */

/**
  * @brief Implementation of ChaCha20-Poly1305 encryption (Defined internally)
  */
extern const cmox_chachapoly_impl_t CMOX_CHACHAPOLY_ENC;

/**
  * @brief Implementation of ChaCha20-Poly1305 decryption (Defined internally)
  */
extern const cmox_chachapoly_impl_t CMOX_CHACHAPOLY_DEC;

/**
  * @}
  */

/**
  * @brief Identifier of the ChaCha20-Poly1305 encryption for single-call function
  *        (Defined internally)
  */
extern const cmox_aead_algo_t CMOX_CHACHAPOLY_ENC_ALGO;

/**
  * @brief Identifier of the ChaCha20-Poly1305 decryption for single-call function
  *        (Defined internally)
  */
extern const cmox_aead_algo_t CMOX_CHACHAPOLY_DEC_ALGO;

/* Public methods prototypes -------------------------------------------------*/

/** @defgroup CMOX_CHACHAPOLY_PUBLIC_METHODS ChaCha20-Poly1305 public method
  *                                           prototypes
  * @{
  */

/**
  * @brief ChaCha20-Poly1305 constructor
  *
  * The function is used for specifying if the algorithm will be used for
  * encryption or decryption.
  *
  * @param P_pThis Pointer to the ChaCha20-Poly1305 handle to initialize
  * @param P_impl  Constant that specifies the implementation to use.
  *                This parameter can be one of the following values:
  *                @arg CMOX_CHACHAPOLY_ENC
  *                @arg CMOX_CHACHAPOLY_DEC
  * @return cmox_cipher_handle_t* Pointer to a general cipher handle. This will
  *                be used by the general purpose cipher functions in order to
  *                perform the algorithm
  */
cmox_cipher_handle_t *cmox_chachapoly_construct(cmox_chachapoly_handle_t *P_pThis,
                                                cmox_chachapoly_impl_t P_impl);

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

#endif /* CMOX_CHACHA20_POLY1305_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
