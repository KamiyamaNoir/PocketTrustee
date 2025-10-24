/**
  ******************************************************************************
  * @file    cmox_ecdsa.h
  * @author  MCD Application Team
  * @brief   Header file for ECDSA signature creation and verification
             functions
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

#ifndef CMOX_ECDSA_H
#define CMOX_ECDSA_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ecc/cmox_ecc.h"

/** @addtogroup CMOX_ECC
  * @{
  */

/**
  * @defgroup CMOX_ECC_ECDSA ECDSA algorithm
  * @{
  */

/** @defgroup CMOX_ECC_ECDSA_PUBLIC_METHODS ECDSA public method prototypes
  * @{
  */

/**
  * @brief      Generate private and public keys to use with ECDSA
  * @param[in]  P_pEccCtx       Context for ECC operations
  * @param[in]  P_CurveParams   Curve Parameters and point functions
  *                             This parameter can be one of the following:
  *                                @arg CMOX_ECC_SECP224R1_LOWMEM
  *                                @arg CMOX_ECC_SECP224R1_HIGHMEM
  *                                @arg CMOX_ECC_SECP256R1_LOWMEM
  *                                @arg CMOX_ECC_SECP256R1_HIGHMEM
  *                                @arg CMOX_ECC_SECP384R1_LOWMEM
  *                                @arg CMOX_ECC_SECP384R1_HIGHMEM
  *                                @arg CMOX_ECC_SECP521R1_LOWMEM
  *                                @arg CMOX_ECC_SECP521R1_HIGHMEM
  *                                @arg CMOX_ECC_SECP256K1_LOWMEM
  *                                @arg CMOX_ECC_SECP256K1_HIGHMEM
  *                                @arg CMOX_ECC_BPP160R1_LOWMEM
  *                                @arg CMOX_ECC_BPP160R1_HIGHMEM
  *                                @arg CMOX_ECC_BPP192R1_LOWMEM
  *                                @arg CMOX_ECC_BPP192R1_HIGHMEM
  *                                @arg CMOX_ECC_BPP224R1_LOWMEM
  *                                @arg CMOX_ECC_BPP224R1_HIGHMEM
  *                                @arg CMOX_ECC_BPP256R1_LOWMEM
  *                                @arg CMOX_ECC_BPP256R1_HIGHMEM
  *                                @arg CMOX_ECC_BPP320R1_LOWMEM
  *                                @arg CMOX_ECC_BPP320R1_HIGHMEM
  *                                @arg CMOX_ECC_BPP384R1_LOWMEM
  *                                @arg CMOX_ECC_BPP384R1_HIGHMEM
  *                                @arg CMOX_ECC_BPP512R1_LOWMEM
  *                                @arg CMOX_ECC_BPP512R1_HIGHMEM
  *                                @arg CMOX_ECC_BPP160T1_LOWMEM
  *                                @arg CMOX_ECC_BPP160T1_HIGHMEM
  *                                @arg CMOX_ECC_BPP192T1_LOWMEM
  *                                @arg CMOX_ECC_BPP192T1_HIGHMEM
  *                                @arg CMOX_ECC_BPP224T1_LOWMEM
  *                                @arg CMOX_ECC_BPP224T1_HIGHMEM
  *                                @arg CMOX_ECC_BPP256T1_LOWMEM
  *                                @arg CMOX_ECC_BPP256T1_HIGHMEM
  *                                @arg CMOX_ECC_BPP320T1_LOWMEM
  *                                @arg CMOX_ECC_BPP320T1_HIGHMEM
  *                                @arg CMOX_ECC_BPP384T1_LOWMEM
  *                                @arg CMOX_ECC_BPP384T1_HIGHMEM
  *                                @arg CMOX_ECC_BPP512T1_LOWMEM
  *                                @arg CMOX_ECC_BPP512T1_HIGHMEM
  *                                @arg CMOX_ECC_FRP256V1_LOWMEM
  *                                @arg CMOX_ECC_FRP256V1_HIGHMEM
  * @param[in]  P_pRandom       Buffer of random bytes
  * @param[in]  P_RandomLen     Byte length of the random buffer
  * @param[out] P_pPrivKey      Buffer with the private key (in range [1; N - 1])
  * @param[out] P_pPrivKeyLen   Byte length of the private key
  * @param[out] P_pPubKey       Buffer with the public key
  * @param[out] P_pPubKeyLen    Byte length of the public key
  * @note the private key is derived from the random in the
  *       following way: \verbatim privkey = (random >> ((WORD_NBIT - N_MSW_NBIT) & 7)) + 1 \endverbatim,
  *       where: random is the input random buffer (interpreted as a big-endian integer),
  *              WORD_NBIT is the bitsize of a cpu word,
  *              N_MSW_NBIT is the bitlength of the most significant word of the parameter N of the curve.
  * @note If P_RandomLen is not enough, an error is returned and P_pPrivKeyLen
  *       contains the minimum number of bytes of random to provide (and is also
  *       the minimum length of P_pPrivKey), while P_pPubKeyLen contains the
  *       minimum length of P_pPubKey.
  * @retval     CMOX_ECC_SUCCESS                Everything OK
  * @retval     CMOX_ECC_ERR_MATHCURVE_MISMATCH Mathematical function set is not compatible with current ECC curve
  * @retval     CMOX_ECC_ERR_ALGOCURVE_MISMATCH Curve is not compatible with current functionality
  * @retval     CMOX_ECC_ERR_BAD_PARAMETERS     Some NULL/wrong/empty parameter or Construct API not called
  * @retval     CMOX_ECC_ERR_WRONG_RANDOM       Random material too short or not valid for the functionality
  * @retval     CMOX_ECC_ERR_MEMORY_FAIL        Not enough memory
  */
cmox_ecc_retval_t cmox_ecdsa_keyGen(cmox_ecc_handle_t     *P_pEccCtx,
                                    const cmox_ecc_impl_t P_CurveParams,
                                    const uint8_t         *P_pRandom,
                                    size_t                P_RandomLen,
                                    uint8_t               *P_pPrivKey,
                                    size_t                *P_pPrivKeyLen,
                                    uint8_t               *P_pPubKey,
                                    size_t                *P_pPubKeyLen);

/**
  * @brief      Generate a signature, using ECDSA
  * @param[in]  P_pEccCtx       Context for ECC operations
  * @param[in]  P_CurveParams   Curve Parameters and point functions
  *                             This parameter can be one of the following:
  *                                @arg CMOX_ECC_SECP224R1_LOWMEM
  *                                @arg CMOX_ECC_SECP224R1_HIGHMEM
  *                                @arg CMOX_ECC_SECP256R1_LOWMEM
  *                                @arg CMOX_ECC_SECP256R1_HIGHMEM
  *                                @arg CMOX_ECC_SECP384R1_LOWMEM
  *                                @arg CMOX_ECC_SECP384R1_HIGHMEM
  *                                @arg CMOX_ECC_SECP521R1_LOWMEM
  *                                @arg CMOX_ECC_SECP521R1_HIGHMEM
  *                                @arg CMOX_ECC_SECP256K1_LOWMEM
  *                                @arg CMOX_ECC_SECP256K1_HIGHMEM
  *                                @arg CMOX_ECC_BPP160R1_LOWMEM
  *                                @arg CMOX_ECC_BPP160R1_HIGHMEM
  *                                @arg CMOX_ECC_BPP192R1_LOWMEM
  *                                @arg CMOX_ECC_BPP192R1_HIGHMEM
  *                                @arg CMOX_ECC_BPP224R1_LOWMEM
  *                                @arg CMOX_ECC_BPP224R1_HIGHMEM
  *                                @arg CMOX_ECC_BPP256R1_LOWMEM
  *                                @arg CMOX_ECC_BPP256R1_HIGHMEM
  *                                @arg CMOX_ECC_BPP320R1_LOWMEM
  *                                @arg CMOX_ECC_BPP320R1_HIGHMEM
  *                                @arg CMOX_ECC_BPP384R1_LOWMEM
  *                                @arg CMOX_ECC_BPP384R1_HIGHMEM
  *                                @arg CMOX_ECC_BPP512R1_LOWMEM
  *                                @arg CMOX_ECC_BPP512R1_HIGHMEM
  *                                @arg CMOX_ECC_BPP160T1_LOWMEM
  *                                @arg CMOX_ECC_BPP160T1_HIGHMEM
  *                                @arg CMOX_ECC_BPP192T1_LOWMEM
  *                                @arg CMOX_ECC_BPP192T1_HIGHMEM
  *                                @arg CMOX_ECC_BPP224T1_LOWMEM
  *                                @arg CMOX_ECC_BPP224T1_HIGHMEM
  *                                @arg CMOX_ECC_BPP256T1_LOWMEM
  *                                @arg CMOX_ECC_BPP256T1_HIGHMEM
  *                                @arg CMOX_ECC_BPP320T1_LOWMEM
  *                                @arg CMOX_ECC_BPP320T1_HIGHMEM
  *                                @arg CMOX_ECC_BPP384T1_LOWMEM
  *                                @arg CMOX_ECC_BPP384T1_HIGHMEM
  *                                @arg CMOX_ECC_BPP512T1_LOWMEM
  *                                @arg CMOX_ECC_BPP512T1_HIGHMEM
  *                                @arg CMOX_ECC_FRP256V1_LOWMEM
  *                                @arg CMOX_ECC_FRP256V1_HIGHMEM
  * @param[in]  P_pRandom       Buffer of random bytes
  * @param[in]  P_RandomLen     Byte length of the random buffer
  * @param[in]  P_pPrivKey      Buffer with the private key
  * @param[in]  P_PrivKeyLen    Byte length of the private key
  * @param[in]  P_pDigest       Buffer with the digest to sign
  * @param[in]  P_DigestLen     Byte length of the digest
  * @param[out] P_pSignature    Buffer with the signature (concatenation of R and S)
  * @param[out] P_pSignatureLen Byte length of the signature
  * @note the internal value k, used as scalar and multiplied by the base point, is derived from the random in the
  *       following way: \verbatim k = (random >> ((WORD_NBIT - N_MSW_NBIT) & 7)) + 1 \endverbatim,
  *       where: random is the input random buffer (interpreted as a big-endian integer),
  *              WORD_NBIT is the bitsize of a cpu word,
  *              N_MSW_NBIT is the bitlength of the most significant word of the parameter N of the curve.
  * @note If P_RandomLen is not enough, an error is returned and
  *       P_pSignatureLen contains the minimum number of bytes of random
  *       to provide (and is also the minimum length of P_pPrivKey, and
  *       half the length of P_pSignature).
  * @retval     CMOX_ECC_SUCCESS                Everything OK
  * @retval     CMOX_ECC_ERR_MATHCURVE_MISMATCH Mathematical function set is not compatible with current ECC curve
  * @retval     CMOX_ECC_ERR_ALGOCURVE_MISMATCH Curve is not compatible with current functionality
  * @retval     CMOX_ECC_ERR_BAD_PARAMETERS     Some NULL/wrong/empty parameter or Construct API not called
  * @retval     CMOX_ECC_ERR_WRONG_RANDOM       Random material too short or not valid for the functionality
  * @retval     CMOX_ECC_ERR_MEMORY_FAIL        Not enough memory
  */
cmox_ecc_retval_t cmox_ecdsa_sign(cmox_ecc_handle_t     *P_pEccCtx,
                                  const cmox_ecc_impl_t P_CurveParams,
                                  const uint8_t         *P_pRandom,
                                  size_t                P_RandomLen,
                                  const uint8_t         *P_pPrivKey,
                                  size_t                P_PrivKeyLen,
                                  const uint8_t         *P_pDigest,
                                  size_t                P_DigestLen,
                                  uint8_t               *P_pSignature,
                                  size_t                *P_pSignatureLen);

/**
  * @brief      Verify a signature, using ECDSA
  * @param[in]  P_pEccCtx       Context for ECC operations
  * @param[in]  P_CurveParams   Curve Parameters and point functions
  *                             This parameter can be one of the following:
  *                                @arg CMOX_ECC_SECP224R1_LOWMEM
  *                                @arg CMOX_ECC_SECP224R1_HIGHMEM
  *                                @arg CMOX_ECC_SECP256R1_LOWMEM
  *                                @arg CMOX_ECC_SECP256R1_HIGHMEM
  *                                @arg CMOX_ECC_SECP384R1_LOWMEM
  *                                @arg CMOX_ECC_SECP384R1_HIGHMEM
  *                                @arg CMOX_ECC_SECP521R1_LOWMEM
  *                                @arg CMOX_ECC_SECP521R1_HIGHMEM
  *                                @arg CMOX_ECC_SECP256K1_LOWMEM
  *                                @arg CMOX_ECC_SECP256K1_HIGHMEM
  *                                @arg CMOX_ECC_BPP160R1_LOWMEM
  *                                @arg CMOX_ECC_BPP160R1_HIGHMEM
  *                                @arg CMOX_ECC_BPP192R1_LOWMEM
  *                                @arg CMOX_ECC_BPP192R1_HIGHMEM
  *                                @arg CMOX_ECC_BPP224R1_LOWMEM
  *                                @arg CMOX_ECC_BPP224R1_HIGHMEM
  *                                @arg CMOX_ECC_BPP256R1_LOWMEM
  *                                @arg CMOX_ECC_BPP256R1_HIGHMEM
  *                                @arg CMOX_ECC_BPP320R1_LOWMEM
  *                                @arg CMOX_ECC_BPP320R1_HIGHMEM
  *                                @arg CMOX_ECC_BPP384R1_LOWMEM
  *                                @arg CMOX_ECC_BPP384R1_HIGHMEM
  *                                @arg CMOX_ECC_BPP512R1_LOWMEM
  *                                @arg CMOX_ECC_BPP512R1_HIGHMEM
  *                                @arg CMOX_ECC_BPP160T1_LOWMEM
  *                                @arg CMOX_ECC_BPP160T1_HIGHMEM
  *                                @arg CMOX_ECC_BPP192T1_LOWMEM
  *                                @arg CMOX_ECC_BPP192T1_HIGHMEM
  *                                @arg CMOX_ECC_BPP224T1_LOWMEM
  *                                @arg CMOX_ECC_BPP224T1_HIGHMEM
  *                                @arg CMOX_ECC_BPP256T1_LOWMEM
  *                                @arg CMOX_ECC_BPP256T1_HIGHMEM
  *                                @arg CMOX_ECC_BPP320T1_LOWMEM
  *                                @arg CMOX_ECC_BPP320T1_HIGHMEM
  *                                @arg CMOX_ECC_BPP384T1_LOWMEM
  *                                @arg CMOX_ECC_BPP384T1_HIGHMEM
  *                                @arg CMOX_ECC_BPP512T1_LOWMEM
  *                                @arg CMOX_ECC_BPP512T1_HIGHMEM
  *                                @arg CMOX_ECC_FRP256V1_LOWMEM
  *                                @arg CMOX_ECC_FRP256V1_HIGHMEM
  * @param[in]  P_pPubKey       Buffer with the public key
  * @param[in]  P_PubKeyLen     Byte length of the public key
  * @param[in]  P_pDigest       Buffer with the digest to sign
  * @param[in]  P_DigestLen     Byte length of the digest
  * @param[in]  P_pSignature    Buffer with the signature (concatenation of R and S)
  * @param[in]  P_SignatureLen  Byte length of the signature
  * @param[out] P_pFaultCheck   Optional value to check, together with the retval, to verify if some fault happened
  * @retval     CMOX_ECC_AUTH_SUCCESS           Signature verified
  * @retval     CMOX_ECC_AUTH_FAIL              Signature NOT verified
  * @retval     CMOX_ECC_ERR_MATHCURVE_MISMATCH Mathematical function set is not compatible with current ECC curve
  * @retval     CMOX_ECC_ERR_ALGOCURVE_MISMATCH Curve is not compatible with current functionality
  * @retval     CMOX_ECC_ERR_BAD_PARAMETERS     Some NULL/wrong/empty parameter or Construct API not called
  * @retval     CMOX_ECC_ERR_MEMORY_FAIL        Not enough memory
  * @retval     CMOX_ECC_ERR_INVALID_SIGNATURE  Input signature corrupted or not in the expected format
  * @retval     CMOX_ECC_ERR_INVALID_PUBKEY     Public key not in a valid format
  * @note       P_pFaultCheck value, if the parameter is provided, MUST be checked to
  *             be equal to the retval, and both MUST be equal to the successful value.
  *             P_pFaultCheck MUST be checked only if the main result is successful,
  *             and has no relevance if the main result is not successful.
  *             Every comparison (both for the return value and for P_pFaultCheck) must be done against
  *             the success value, and not comparing the value with the failure value. Indeed, in presence of faults,
  *             especially P_pFaultCheck, could be a dirty value.
  */
cmox_ecc_retval_t cmox_ecdsa_verify(cmox_ecc_handle_t     *P_pEccCtx,
                                    const cmox_ecc_impl_t P_CurveParams,
                                    const uint8_t         *P_pPubKey,
                                    size_t                P_PubKeyLen,
                                    const uint8_t         *P_pDigest,
                                    size_t                P_DigestLen,
                                    const uint8_t         *P_pSignature,
                                    size_t                P_SignatureLen,
                                    uint32_t              *P_pFaultCheck);

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
#endif

#endif /* CMOX_ECDSA_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
