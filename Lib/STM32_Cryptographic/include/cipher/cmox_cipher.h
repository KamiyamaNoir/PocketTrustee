/**
  ******************************************************************************
  * @file    cmox_cipher.h
  * @author  MCD Application Team
  * @brief   Header file for the Cipher module
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
#ifndef CMOX_CIPHER_H
#define CMOX_CIPHER_H

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* Include files -------------------------------------------------------------*/
#include <stdint.h>
#include <stddef.h>
#include "cmox_cipher_retvals.h"

/** @addtogroup CMOX_CIPHER
  * @{
  */

/* Macros --------------------------------------------------------------------*/

/** @defgroup CMOX_CIPHER_KEYSIZE Key sizes for the cipher module
  * @{
  */

/**
  * @brief Size in bytes of AES128 and SM4 keys
  */
#define CMOX_CIPHER_128_BIT_KEY ((cmox_cipher_keyLen_t)16U)

/**
  * @brief Size in bytes of AES192 keys
  */
#define CMOX_CIPHER_192_BIT_KEY ((cmox_cipher_keyLen_t)24U)

/**
  * @brief Size in bytes of AES256 keys
  */
#define CMOX_CIPHER_256_BIT_KEY ((cmox_cipher_keyLen_t)32U)

/**
  * @}
  */

/* Public types --------------------------------------------------------------*/

/** @defgroup CMOX_CIPHER_PUBLIC_TYPES Cipher module public types
  * @{
  */

/**
  * @brief Type for specifying the key size for the cipher
  */
typedef size_t cmox_cipher_keyLen_t;

/**
  * @brief Cipher Virtual Table
  *
  * This type specifies a pointer to the virtual table containing the methods
  * for a particular algorithm (e.g. CBC or CHACHA20-POLY1305)
  */
typedef const struct cmox_cipher_vtableStruct_st *cmox_cipher_vtable_t;

/**
  * @brief Cipher algorithm type
  *
  * This type specifies the algorithm to use with the cipher module (e.g. CBC).
  * The type is defined as a pointer to a structure, that
  * contains the functions for the specific algorithm, defined in the library
  * internally
  */
typedef const struct cmox_cipher_algoStruct_st *cmox_cipher_algo_t;

/**
  * @brief AEAD algorithm type
  *
  * This type specifies the AEAD algorithm to use with the cipher module
  * (e.g. CCM or CHACHA20-POLY1305).
  * The type is defined as a pointer to a structure, that
  * contains the functions for the specific algorithm, defined in the library
  * internally
  */
typedef const struct cmox_aead_algoStruct_st *cmox_aead_algo_t;

/**
  * @brief Cipher handle structure definition
  */
typedef struct
{
  cmox_cipher_vtable_t table; /*!< Cipher virtual table */
  uint32_t internalState;     /*!< internal state of the cipher handle */
} cmox_cipher_handle_t;

/**
  * @}
  */

/* Public methods prototypes -------------------------------------------------*/

/** @defgroup CMOX_CIPHER_PUBLIC_METHODS Cipher public method prototypes
  * @{
  */

/**
  * @brief Cleanup the cipher handle
  *
  * @param P_pThis Cipher handle to cleanup
  * @return cmox_cipher_retval_t Cipher return value
  */
cmox_cipher_retval_t cmox_cipher_cleanup(cmox_cipher_handle_t *P_pThis);

/**
  * @brief Initialize the cipher handle for performing the specified algorithm
  *
  * @param P_pThis Cipher handle to initialize
  * @return cmox_cipher_retval_t Cipher return value
  * @note The cipher handle must be derived from an algorithm-specific handle
  *       using the correct constructor
  */
cmox_cipher_retval_t cmox_cipher_init(cmox_cipher_handle_t *P_pThis);

/**
  * @brief Se the key to be used with the cipher algorithm
  *
  * @param P_pThis Cipher handle to set
  * @param P_pKey Buffer of bytes containing the key
  * @param P_keyLen Len in bytes of the key
  * @return cmox_cipher_retval_t Cipher return value
  * @note The XTS mode of operation requires a key with size double than the
  *       normal one (i.e. AES128-XTS will require a 32 bytes key, AES256-XTS
  *       will require a 64 bytes key and so on)
  */
cmox_cipher_retval_t cmox_cipher_setKey(cmox_cipher_handle_t *P_pThis,
                                        const uint8_t *P_pKey,
                                        cmox_cipher_keyLen_t P_keyLen);

/**
  * @brief Set the initialization vector/nonce to be used with the cipher
  *        algorithm
  *
  * @param P_pThis Cipher handle to set
  * @param P_pIv Buffer of bytes containing the IV/nonce
  * @param P_ivLen  Size in bytes of the key
  * @return cmox_cipher_retval_t Cipher return value
  */
cmox_cipher_retval_t cmox_cipher_setIV(cmox_cipher_handle_t *P_pThis,
                                       const uint8_t *P_pIv,
                                       size_t P_ivLen);

/**
  * @brief Set the size of the tag for AEAD cipher
  *
  * @param P_pThis Cipher handle to set
  * @param P_tagLen Size in bytes of the tag
  * @return cmox_cipher_retval_t Cipher return value
  * @note This function must be used only for AEAD ciphers. If used with other
  *       cipher, it won't have any effects
  * @note For ChaCha20-Poly1305 the tag size is fixed to 16 bytes, and this
  *       function has no effects
  */
cmox_cipher_retval_t cmox_cipher_setTagLen(cmox_cipher_handle_t *P_pThis,
                                           size_t P_tagLen);

/**
  * @brief Set the total payload size (only for CCM AEAD cipher)
  *
  * @param P_pThis Cipher handle to set
  * @param P_totalPayloadLen Size in bytes of the total payload
  * @return cmox_cipher_retval_t Cipher return value
  * @note This function must be used for CCM ciphers. If used with other
  *       cipher, it won't have any effects
  */
cmox_cipher_retval_t cmox_cipher_setPayloadLen(cmox_cipher_handle_t *P_pThis,
                                               size_t P_totalPayloadLen);

/**
  * @brief Set the total authenticated data size (only for CCM AEAD cipher)
  *
  * @param P_pThis Cipher handle to set
  * @param P_totalADLen Size in bytes of the total authenticated data
  * @return cmox_cipher_retval_t Cipher return value
  * @note This function must be used for CCM ciphers. If used with other
  *       cipher, it won't have any effects
  */
cmox_cipher_retval_t cmox_cipher_setADLen(cmox_cipher_handle_t *P_pThis,
                                          size_t P_totalADLen);

/**
  * @brief Append additional authenticated data to the cipher handle
  *
  * @param P_pThis Cipher handle where the authenticated data will be appended
  * @param P_pInput Buffer of bytes containing the data to append
  * @param P_inputLen Size in bytes of the data to append
  * @return cmox_cipher_retval_t Cipher return value
  * @note This function must be used only for AEAD ciphers. If used with other
  *       cipher, it won't have any effects
  */
cmox_cipher_retval_t cmox_cipher_appendAD(cmox_cipher_handle_t *P_pThis,
                                          const uint8_t *P_pInput,
                                          size_t P_inputLen);

/**
  * @brief Append part or the totality of the plaintext/ciphertext and return the
  *        corresponding ciphertext/plaintext
  *
  * @param P_pThis Cipher handle to use for ciphering the data
  * @param P_pInput Buffer of bytes containing the data to append
  * @param P_inputLen Size in bytes of the data to append
  * @param P_pOutput Buffer of bytes where there will be stored the encrypted or
  *                  decrypted data
  * @param P_pOutputLen Number of bytes that have been generated by the function.
  *        It is an optional parameter and can be set to NULL if not needed.
  * @return cmox_cipher_retval_t Cipher return value
  * @note ECB and CFB modes of operation for AES and SM4 require that the input
  *       length for each append is multiple of the block size (i.e. 16 bytes)
  * @note CBC mode of operation will apply CS2 ciphertext stealing
  *       <a href="SP 800-38 A - Addendum"> NIST SP 800-38A Addendum </a>
  *       in case the input length is greater than 16 bytes and not multiple of
  *       16 bytes. After CS2 is applied, no more appends are allowed.
  * @note CTR, OFB, GCM and CCM modes of operation permits to have input not
  *       multiple of the block size (i.e. 16 bytes for AES and SM4), but after
  *       this no more further appends are allowed.
  * @note For ChaCha20-Poly1305, this function can be called multiple times with
  *       P_inputLen multiple of 64 bytes. A single, last, call can be made
  *       with any value for P_inputLen.
  * @note With the exception of KEYWRAP algorithm, the minimum size of P_pOutput
  *       buffer must be P_inputLen.
  * @note For KEYWRAP mode, this function must be used to append the key to wrap,
  *       and must be done in a single call (no partial append of the key is
  *       supported). The P_pOutput buffer will contain the encrypted key AND the
  *       8 bytes authentication tag. So the minimum size of this buffer must be
  *       P_inputLen + 8.
  */
cmox_cipher_retval_t cmox_cipher_append(cmox_cipher_handle_t *P_pThis,
                                        const uint8_t *P_pInput,
                                        size_t P_inputLen,
                                        uint8_t *P_pOutput,
                                        size_t *P_pOutputLen);

/**
  * @brief Generate the authenticated tag in case the AEAD operation is an
  *        encryption
  *
  * @param P_pThis Cipher handle used for encrypting the data
  * @param P_pTag  Buffer of bytes where there will be stored the generated tag
  * @param P_pTagLen Number of bytes that have been processed by the function.
  *        It is an optional parameter and can be set to NULL if not needed.
  * @return cmox_cipher_retval_t Cipher return value
  * @note This function must be used only for AEAD ciphers in encryption mode. If
  *       used with other cipher, it won't have any effects. If used with AEAD
  *       cipher in decryption mode, it will raise an error.
  */
cmox_cipher_retval_t cmox_cipher_generateTag(cmox_cipher_handle_t *P_pThis,
                                             uint8_t *P_pTag,
                                             size_t *P_pTagLen);

/**
  * @brief Authenticate the processed data with the given tag
  *
  * @param P_pThis Cipher used for decrypting the data
  * @param P_pTag Buffer of bytes containing the tag to use for the verification
  * @param P_pFaultCheck Optional value to check, together with the retval,
  *                      to verify if some fault happened
  * @return cmox_cipher_retval_t Cipher return value
  * @note This function must be used only for AEAD ciphers in decryption mode. If
  *       used with other cipher, it won't have any effects. If used with AEAD
  *       cipher in encryption mode, it will raise an error.
  * @note P_pFaultCheck value, if the parameter is provided, MUST be checked to
  *       be equal to the retval, and both MUST be equal to the successful value.
  *       P_pFaultCheck MUST be checked only if the main result is successful,
  *       and has no relevance if the main result is not successful.
  *       Every comparison (both for the return value and for P_pFaultCheck) must
  *       be done against the success value, and not comparing the value with the
  *       failure value. Indeed, in presence of faults, especially P_pFaultCheck,
  *       could be a dirty value.
  */
cmox_cipher_retval_t cmox_cipher_verifyTag(cmox_cipher_handle_t *P_pThis,
                                           const uint8_t *P_pTag,
                                           uint32_t *P_pFaultCheck);

/**
  * @brief Encrypt or decrypt a message using a symmetric cipher
  *
  * @param P_algo Identifier of the cipher algorithm to use for the computation.
  *               This parameter can be one of the following:
  *               @arg CMOX_AESFAST_ECB_ENC_ALGO
  *               @arg CMOX_AESFAST_CBC_ENC_ALGO
  *               @arg CMOX_AESFAST_CTR_ENC_ALGO
  *               @arg CMOX_AESFAST_CFB_ENC_ALGO
  *               @arg CMOX_AESFAST_OFB_ENC_ALGO
  *               @arg CMOX_AESFAST_XTS_ENC_ALGO
  *               @arg CMOX_AESSMALL_ECB_ENC_ALGO
  *               @arg CMOX_AESSMALL_CBC_ENC_ALGO
  *               @arg CMOX_AESSMALL_CTR_ENC_ALGO
  *               @arg CMOX_AESSMALL_CFB_ENC_ALGO
  *               @arg CMOX_AESSMALL_OFB_ENC_ALGO
  *               @arg CMOX_AESSMALL_XTS_ENC_ALGO
  *               @arg CMOX_AESSMALL_KEYWRAP_ENC_ALGO
  *               @arg CMOX_SM4_ECB_ENC_ALGO
  *               @arg CMOX_SM4_CBC_ENC_ALGO
  *               @arg CMOX_SM4_CTR_ENC_ALGO
  *               @arg CMOX_SM4_CFB_ENC_ALGO
  *               @arg CMOX_SM4_OFB_ENC_ALGO
  * @param P_pInput Buffer of bytes containing the data to encrypt or decrypt
  * @param P_inputLen Length in bytes of the data to encrypt or decrypt
  * @param P_pKey Buffer of bytes containing the key
  * @param P_keyLen Length in bytes of the key
  * @param P_pIv Buffer of bytes containing the IV/nonce
  * @param P_ivLen  Length in bytes of the key
  * @param P_pOutput Buffer of bytes where there will be stored the encrypted or
  *                  decrypted data
  * @param P_pOutputLen Number of bytes that have been processed by the function.
  *        It is an optional parameter and can be set to NULL if not needed.
  * @return cmox_cipher_retval_t Cipher return value
  * @note This single call function cannot be used for AEAD ciphers
  */
cmox_cipher_retval_t cmox_cipher_encrypt(cmox_cipher_algo_t P_algo,
                                         const uint8_t *P_pInput,
                                         size_t P_inputLen,
                                         const uint8_t *P_pKey,
                                         cmox_cipher_keyLen_t P_keyLen,
                                         const uint8_t *P_pIv,
                                         size_t P_ivLen,
                                         uint8_t *P_pOutput,
                                         size_t *P_pOutputLen);

/**
  * @brief Decrypt a message using a symmetric cipher
  *
  * @param P_algo Identifier of the cipher algorithm to use for the computation.
  *               This parameter can be one of the following:
  *               @arg CMOX_AESFAST_ECB_DEC_ALGO
  *               @arg CMOX_AESFAST_CBC_DEC_ALGO
  *               @arg CMOX_AESFAST_CTR_DEC_ALGO
  *               @arg CMOX_AESFAST_CFB_DEC_ALGO
  *               @arg CMOX_AESFAST_OFB_DEC_ALGO
  *               @arg CMOX_AESFAST_XTS_DEC_ALGO
  *               @arg CMOX_AESFAST_KEYWRAP_DEC_ALGO
  *               @arg CMOX_AESSMALL_ECB_DEC_ALGO
  *               @arg CMOX_AESSMALL_CBC_DEC_ALGO
  *               @arg CMOX_AESSMALL_CTR_DEC_ALGO
  *               @arg CMOX_AESSMALL_CFB_DEC_ALGO
  *               @arg CMOX_AESSMALL_OFB_DEC_ALGO
  *               @arg CMOX_AESSMALL_XTS_DEC_ALGO
  *               @arg CMOX_AESSMALL_KEYWRAP_DEC_ALGO
  *               @arg CMOX_SM4_ECB_DEC_ALGO
  *               @arg CMOX_SM4_CBC_DEC_ALGO
  *               @arg CMOX_SM4_CTR_DEC_ALGO
  *               @arg CMOX_SM4_CFB_DEC_ALGO
  *               @arg CMOX_SM4_OFB_DEC_ALGO
  * @param P_pInput Buffer of bytes containing the data to encrypt or decrypt
  * @param P_inputLen Length in bytes of the data to encrypt or decrypt
  * @param P_pKey Buffer of bytes containing the key
  * @param P_keyLen Length in bytes of the key
  * @param P_pIv Buffer of bytes containing the IV/nonce
  * @param P_ivLen  Length in bytes of the key
  * @param P_pOutput Buffer of bytes where there will be stored the decrypted
  *                  data.
  * @param P_pOutputLen Number of bytes that have been processed by the function.
  *        It is an optional parameter and can be set to NULL if not needed.
  * @return cmox_cipher_retval_t Cipher return value
  * @note This single call function cannot be used for AEAD ciphers
  */
cmox_cipher_retval_t cmox_cipher_decrypt(cmox_cipher_algo_t P_algo,
                                         const uint8_t *P_pInput,
                                         size_t P_inputLen,
                                         const uint8_t *P_pKey,
                                         cmox_cipher_keyLen_t P_keyLen,
                                         const uint8_t *P_pIv,
                                         size_t P_ivLen,
                                         uint8_t *P_pOutput,
                                         size_t *P_pOutputLen);

/**
  * @brief Encrypt  a message using AEAD cipher
  *
  * @param P_algo Identifier of the AEAD cipher algorithm to use for the
  *               computation. This parameter can be one of the following:
  *               @arg CMOX_AESFAST_GCMFAST_ENC_ALGO
  *               @arg CMOX_AESFAST_GCMSMALL_ENC_ALGO
  *               @arg CMOX_AESSMALL_GCMFAST_ENC_ALGO
  *               @arg CMOX_AESSMALL_GCMSMALL_ENC_ALGO
  *               @arg CMOX_AESFAST_CCM_ENC_ALGO
  *               @arg CMOX_AESSMALL_CCM_ENC_ALGO
  *               @arg CMOX_CHACHAPOLY_ENC_ALGO
  * @param P_pInput Buffer of bytes containing the data to encrypt
  * @param P_inputLen Length in bytes of the data to encrypt
  * @param P_tagLen Length in bytes of the authentication tag to append to the
  *                 ciphertext
  * @param P_pKey Buffer of bytes containing the key
  * @param P_keyLen Length in bytes of the key
  * @param P_pIv Buffer of bytes containing the IV/nonce
  * @param P_ivLen  Length in bytes of the key
  * @param P_pAddData Buffer of bytes containing the additional data to be used
  *                   for authentication
  * @param P_addDataLen Length in bytes of the Additional data
  * @param P_pOutput Buffer of bytes where there will be stored the encrypted and
  *                  authenticated data (i.e. ciphertext + tag)
  * @param P_pOutputLen Number of bytes that have been generated by the function.
  *        If correct, it will be the size of the encrypted and authenticated
  *        data (i.e. ciphertext + tag).
  *        It is an optional parameter and can be set to NULL if not needed.
  * @return cmox_cipher_retval_t
  * @note This single call function can be used for AEAD ciphers only
  */
cmox_cipher_retval_t cmox_aead_encrypt(cmox_aead_algo_t P_algo,
                                       const uint8_t *P_pInput,
                                       size_t P_inputLen,
                                       size_t P_tagLen,
                                       const uint8_t *P_pKey,
                                       cmox_cipher_keyLen_t P_keyLen,
                                       const uint8_t *P_pIv,
                                       size_t P_ivLen,
                                       const uint8_t *P_pAddData,
                                       size_t P_addDataLen,
                                       uint8_t *P_pOutput,
                                       size_t *P_pOutputLen);

/**
  * @brief Decrypt a message using AEAD cipher
  *
  * @param P_algo Identifier of the AEAD cipher algorithm to use for the
  *               computation. This parameter can be one of the following:
  *               @arg CMOX_AESFAST_GCMFAST_DEC_ALGO
  *               @arg CMOX_AESFAST_GCMSMALL_DEC_ALGO
  *               @arg CMOX_AESSMALL_GCMFAST_DEC_ALGO
  *               @arg CMOX_AESSMALL_GCMSMALL_DEC_ALGO
  *               @arg CMOX_AESFAST_CCM_DEC_ALGO
  *               @arg CMOX_AESSMALL_CCM_DEC_ALGO
  *               @arg CMOX_CHACHAPOLY_DEC_ALGO
  * @param P_pInput Buffer of bytes containing the authenticated and encrypted
  *                 data (i.e. ciphertext + tag)
  * @param P_inputLen Length in bytes of the the authenticated and encrypted
  *                 data (i.e. ciphertext + tag)
  * @param P_tagLen Length in bytes of the tag
  * @param P_pKey Buffer of bytes containing the key
  * @param P_keyLen Length in bytes of the key
  * @param P_pIv Buffer of bytes containing the IV/nonce
  * @param P_ivLen  Length in bytes of the key
  * @param P_pAddData Buffer of bytes containing the additional data to be used
  *                   for authentication
  * @param P_addDataLen Length in bytes of the Additional data
  * @param P_pOutput Buffer of bytes where there will be stored the decrypted
  *                  data
  * @param P_pOutputLen Number of bytes that have been generated by the function.
  *        If correct, it will be the plaintext size.
  *        It is an optional parameter and can be set to NULL if not needed.
  * @return cmox_cipher_retval_t
  * @note This single call function can be used for AEAD ciphers only
  */
cmox_cipher_retval_t cmox_aead_decrypt(cmox_aead_algo_t P_algo,
                                       const uint8_t *P_pInput,
                                       size_t P_inputLen,
                                       size_t P_tagLen,
                                       const uint8_t *P_pKey,
                                       cmox_cipher_keyLen_t P_keyLen,
                                       const uint8_t *P_pIv,
                                       size_t P_ivLen,
                                       const uint8_t *P_pAddData,
                                       size_t P_addDataLen,
                                       uint8_t *P_pOutput,
                                       size_t *P_pOutputLen);

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CMOX_CIPHER_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
