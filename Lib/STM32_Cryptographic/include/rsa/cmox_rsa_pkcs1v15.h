/**
  ******************************************************************************
  * @file    cmox_rsa_pkcs1v15.h
  * @author  MCD Application Team
  * @brief   Header file for RSA PKCS#1 v1.5 definitions and functions
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

#ifndef CMOX_RSA_PKCS1V15_H
#define CMOX_RSA_PKCS1V15_H

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "rsa/cmox_rsa.h"
#include "utils/cmox_utils_retvals.h"

/** @addtogroup CMOX_RSA
  * @{
  */

/**
  * @defgroup CMOX_RSA_PKCS1V15 RSA PKCS#1 v1.5 standard
  * @{
  */

/**
  * @brief Hash algorithms to use with RSA PKCS#1 API
  * @defgroup CMOX_RSA_PKCS1V15_HASH Hash functions to use in RSA PKCS#1 v1.5
  * @{
  */

/**
  * @brief SHA1 Hash structure for the RSA PKCS#1 v1.5 functions
  */
extern const cmox_rsa_pkcs1v15_hash_t CMOX_RSA_PKCS1V15_HASH_SHA1;
/**
  * @brief SHA224 Hash structure for the RSA PKCS#1 v1.5 functions
  */
extern const cmox_rsa_pkcs1v15_hash_t CMOX_RSA_PKCS1V15_HASH_SHA224;
/**
  * @brief SHA256 Hash structure for the RSA PKCS#1 v1.5 functions
  */
extern const cmox_rsa_pkcs1v15_hash_t CMOX_RSA_PKCS1V15_HASH_SHA256;
/**
  * @brief SHA384 Hash structure for the RSA PKCS#1 v1.5 functions
  */
extern const cmox_rsa_pkcs1v15_hash_t CMOX_RSA_PKCS1V15_HASH_SHA384;
/**
  * @brief SHA512 Hash structure for the RSA PKCS#1 v1.5 functions
  */
extern const cmox_rsa_pkcs1v15_hash_t CMOX_RSA_PKCS1V15_HASH_SHA512;
/**
  * @brief SHA512/224 Hash structure for the RSA PKCS#1 v1.5 functions
  */
extern const cmox_rsa_pkcs1v15_hash_t CMOX_RSA_PKCS1V15_HASH_SHA512_224;
/**
  * @brief SHA512/256 Hash structure for the RSA PKCS#1 v1.5 functions
  */
extern const cmox_rsa_pkcs1v15_hash_t CMOX_RSA_PKCS1V15_HASH_SHA512_256;

/**
  * @}
  */

/**
  * @defgroup CMOX_RSA_PKCS1V15_PUBLIC_METHODS RSA PKCS#1 v1.5 public method prototypes
  * @{
  */

/**
  * @brief       Sign a message using PKCS#1 v1.5
  * @param[in]   P_pRsaCtx       Context for RSA operations
  * @param[in]   P_pPrivKey      Private Key (standard or CRT)
  * @param[in]   P_pDigest       Message to sign
  * @param[in]   P_HashId        Hash to use
  * @param[out]  P_pSignature    Output signature
  * @param[out]  P_pSignatureLen Output signature Length (in Bytes)
  * @retval      CMOX_RSA_SUCCESS                   Everything OK
  * @retval      CMOX_RSA_ERR_MATH_ALGO_MISMATCH    Mathematical function set is incompatible with current functionality
  * @retval      CMOX_RSA_ERR_MEXP_ALGO_MISMATCH    Modexp function set is not compatible with current functionality
  * @retval      CMOX_RSA_ERR_BAD_PARAMETER         Some NULL/wrong/empty parameter
  * @retval      CMOX_RSA_ERR_MEMORY_FAIL           Not enough memory
  * @retval      CMOX_RSA_ERR_MODULUS_TOO_SHORT     Modulus too short for the message to sign
  */
cmox_rsa_retval_t cmox_rsa_pkcs1v15_sign(cmox_rsa_handle_t              *P_pRsaCtx,
                                         const cmox_rsa_key_t           *P_pPrivKey,
                                         const uint8_t                  *P_pDigest,
                                         const cmox_rsa_pkcs1v15_hash_t P_HashId,
                                         uint8_t                        *P_pSignature,
                                         size_t                         *P_pSignatureLen);

/**
  * @brief       Verify a message signed with PKCS#1 v1.5
  * @param[in]   P_pRsaCtx       Context for RSA operations
  * @param[in]   P_pPubKey       Public Key
  * @param[in]   P_pDigest       Message to verify
  * @param[in]   P_HashId        Hash to use
  * @param[in]   P_pSignature    Signature
  * @param[in]   P_SignatureLen  Signature Length (in Bytes)
  * @param[out]  P_pFaultCheck   Optional value to check, together with the retval, to verify if some fault happened
  * @retval      CMOX_RSA_AUTH_SUCCESS              Signature verified
  * @retval      CMOX_RSA_AUTH_FAIL                 Signature NOT verified
  * @retval      CMOX_RSA_ERR_MATH_ALGO_MISMATCH    Mathematical function set is incompatible with current functionality
  * @retval      CMOX_RSA_ERR_MEXP_ALGO_MISMATCH    Modexp function set is not compatible with current functionality
  * @retval      CMOX_RSA_ERR_BAD_PARAMETER         Some NULL/wrong/empty parameter
  * @retval      CMOX_RSA_ERR_INVALID_SIGNATURE     Input signature corrupted or not in the expected format
  * @retval      CMOX_RSA_ERR_MEMORY_FAIL           Not enough memory
  * @retval      CMOX_RSA_ERR_MODULUS_TOO_SHORT     Modulus too short for the message to verify
  * @note        P_pFaultCheck value, if the parameter is provided, MUST be checked to
  *              be equal to the retval, and both MUST be equal to the successful value.
  *              P_pFaultCheck MUST be checked only if the main result is successful,
  *              and has no relevance if the main result is not successful.
  *              Every comparison (both for the return value and for P_pFaultCheck) must be done against
  *              the success value, and not comparing the value with the failure value. Indeed, in presence of faults,
  *              especially P_pFaultCheck, could be a dirty value.
  */
cmox_rsa_retval_t cmox_rsa_pkcs1v15_verify(cmox_rsa_handle_t              *P_pRsaCtx,
                                           const cmox_rsa_key_t           *P_pPubKey,
                                           const uint8_t                  *P_pDigest,
                                           const cmox_rsa_pkcs1v15_hash_t P_HashId,
                                           const uint8_t                  *P_pSignature,
                                           size_t                         P_SignatureLen,
                                           uint32_t                       *P_pFaultCheck);

/**
  * @brief       Encrypt a message using PKCS#1 v1.5
  * @param[in]   P_pRsaCtx       Context for RSA operations
  * @param[in]   P_pPubKey       Public Key
  * @param[in]   P_pInput        Message to encrypt
  * @param[in]   P_InputLen      Message Length (in Bytes)
  * @param[in]   P_pRandom       Random buffer
  * @param[in]   P_RandomLen     Random Length (in Bytes)
  * @param[out]  P_pOutput       Output encrypted buffer
  * @param[out]  P_pOutputLen    Output Length (in Bytes)
  * @retval      CMOX_RSA_SUCCESS                   Everything OK
  * @retval      CMOX_RSA_ERR_MATH_ALGO_MISMATCH    Mathematical function set is incompatible with current functionality
  * @retval      CMOX_RSA_ERR_MEXP_ALGO_MISMATCH    Modexp function set is not compatible with current functionality
  * @retval      CMOX_RSA_ERR_BAD_PARAMETER         Some NULL/wrong/empty parameter
  * @retval      CMOX_RSA_ERR_MODULUS_TOO_SHORT     Modulus too short for the message to encrypt
  * @retval      CMOX_RSA_ERR_MEMORY_FAIL           Not enough memory
  * @retval      CMOX_RSA_ERR_WRONG_RANDOM          Random material too short or not valid for the functionality
  */
cmox_rsa_retval_t cmox_rsa_pkcs1v15_encrypt(cmox_rsa_handle_t     *P_pRsaCtx,
                                            const cmox_rsa_key_t  *P_pPubKey,
                                            const uint8_t         *P_pInput,
                                            size_t                P_InputLen,
                                            const uint8_t         *P_pRandom,
                                            size_t                P_RandomLen,
                                            uint8_t               *P_pOutput,
                                            size_t                *P_pOutputLen);

/**
  * @brief       Decrypt a message using PKCS#1 v1.5
  * @param[in]   P_pRsaCtx       Context for RSA operations
  * @param[in]   P_pPrivKey      Private Key (standard or CRT)
  * @param[in]   P_pInput        Message to decrypt
  * @param[in]   P_InputLen      Message Length (in Bytes)
  * @param[out]  P_pOutput       Output decrypted buffer
  * @param[out]  P_pOutputLen    Output Length (in Bytes)
  * @retval      CMOX_RSA_SUCCESS                   Everything OK
  * @retval      CMOX_RSA_ERR_MATH_ALGO_MISMATCH    Mathematical function set is incompatible with current functionality
  * @retval      CMOX_RSA_ERR_MEXP_ALGO_MISMATCH    Modexp function set is not compatible with current functionality
  * @retval      CMOX_RSA_ERR_BAD_PARAMETER         Some NULL/wrong/empty parameter
  * @retval      CMOX_RSA_ERR_MEMORY_FAIL           Not enough memory
  * @retval      CMOX_RSA_ERR_WRONG_DECRYPTION      Decryption failed, probably due to a wrong private key
  */
cmox_rsa_retval_t cmox_rsa_pkcs1v15_decrypt(cmox_rsa_handle_t     *P_pRsaCtx,
                                            const cmox_rsa_key_t  *P_pPrivKey,
                                            const uint8_t         *P_pInput,
                                            size_t                P_InputLen,
                                            uint8_t               *P_pOutput,
                                            size_t                *P_pOutputLen);

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

#endif /* CMOX_RSA_PKCS1V15_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
