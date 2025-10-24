/**
  ******************************************************************************
  * @file    cmox_ecc.h
  * @author  MCD Application Team
  * @brief   This file provides common function for ECC module
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

#ifndef CMOX_ECC_H
#define CMOX_ECC_H

#include <stdint.h>
#include <stdlib.h>

#include "ecc/cmox_ecc_retvals.h"
#include "ecc/cmox_ecc_types.h"
#if !defined(CMOX_DEFAULT_FILE)
#include "cmox_default_config.h"
#else
#include CMOX_DEFAULT_FILE
#endif /* CMOX_DEFAULT_FILE */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/** @addtogroup CMOX_ECC
  * @{
  */

/**
  * @defgroup CMOX_ECC_CURVES Supported curves implementations
  * @{
  */
extern const cmox_ecc_impl_t CMOX_ECC_CURVE25519;           /*!< Suggested Curve25519 implementation (for X25519) for the target device */
extern const cmox_ecc_impl_t CMOX_ECC_CURVE448;             /*!< Suggested Curve448 implementation (for X448) for the target device */

extern const cmox_ecc_impl_t CMOX_ECC_ED25519_HIGHMEM;      /*!< EDWARDS Ed25519 with general Edwards functions, high RAM usage */
extern const cmox_ecc_impl_t CMOX_ECC_ED25519_OPT_LOWMEM;   /*!< EDWARDS Ed25519 with Edwards functions optimized for a = -1, low RAM usage */
extern const cmox_ecc_impl_t CMOX_ECC_ED25519_OPT_HIGHMEM;  /*!< EDWARDS Ed25519 with Edwards functions optimized for a = -1, high RAM usage */
extern const cmox_ecc_impl_t CMOX_ECC_ED448_LOWMEM;         /*!< EDWARDS Ed448 with general Edwards functions, low RAM usage */
extern const cmox_ecc_impl_t CMOX_ECC_ED448_HIGHMEM;        /*!< EDWARDS Ed448 with general Edwards functions, high RAM usage */

extern const cmox_ecc_impl_t CMOX_ECC_SECP224R1_LOWMEM;     /*!< NIST-R P-224, low RAM usage */
extern const cmox_ecc_impl_t CMOX_ECC_SECP224R1_HIGHMEM;    /*!< NIST-R P-224, high RAM usage */
extern const cmox_ecc_impl_t CMOX_ECC_SECP256R1_LOWMEM;     /*!< NIST-R P-256, low RAM usage */
extern const cmox_ecc_impl_t CMOX_ECC_SECP256R1_HIGHMEM;    /*!< NIST-R P-256, high RAM usage */
extern const cmox_ecc_impl_t CMOX_ECC_SECP384R1_LOWMEM;     /*!< NIST-R P-384, low RAM usage */
extern const cmox_ecc_impl_t CMOX_ECC_SECP384R1_HIGHMEM;    /*!< NIST-R P-384, high RAM usage */
extern const cmox_ecc_impl_t CMOX_ECC_SECP521R1_LOWMEM;     /*!< NIST-R P-521, low RAM usage */
extern const cmox_ecc_impl_t CMOX_ECC_SECP521R1_HIGHMEM;    /*!< NIST-R P-521, high RAM usage */

extern const cmox_ecc_impl_t CMOX_ECC_SECP256K1_LOWMEM;     /*!< NIST-K P-256, low RAM usage */
extern const cmox_ecc_impl_t CMOX_ECC_SECP256K1_HIGHMEM;    /*!< NIST-K P-256, high RAM usage */

extern const cmox_ecc_impl_t CMOX_ECC_BPP160R1_LOWMEM;      /*!< BRAINPOOL-R P-160, low RAM usage */
extern const cmox_ecc_impl_t CMOX_ECC_BPP160R1_HIGHMEM;     /*!< BRAINPOOL-R P-160, high RAM usage */
extern const cmox_ecc_impl_t CMOX_ECC_BPP192R1_LOWMEM;      /*!< BRAINPOOL-R P-192, low RAM usage */
extern const cmox_ecc_impl_t CMOX_ECC_BPP192R1_HIGHMEM;     /*!< BRAINPOOL-R P-192, high RAM usage */
extern const cmox_ecc_impl_t CMOX_ECC_BPP224R1_LOWMEM;      /*!< BRAINPOOL-R P-224, low RAM usage */
extern const cmox_ecc_impl_t CMOX_ECC_BPP224R1_HIGHMEM;     /*!< BRAINPOOL-R P-224, high RAM usage */
extern const cmox_ecc_impl_t CMOX_ECC_BPP256R1_LOWMEM;      /*!< BRAINPOOL-R P-256, low RAM usage */
extern const cmox_ecc_impl_t CMOX_ECC_BPP256R1_HIGHMEM;     /*!< BRAINPOOL-R P-256, high RAM usage */
extern const cmox_ecc_impl_t CMOX_ECC_BPP320R1_LOWMEM;      /*!< BRAINPOOL-R P-320, low RAM usage */
extern const cmox_ecc_impl_t CMOX_ECC_BPP320R1_HIGHMEM;     /*!< BRAINPOOL-R P-320, high RAM usage */
extern const cmox_ecc_impl_t CMOX_ECC_BPP384R1_LOWMEM;      /*!< BRAINPOOL-R P-384, low RAM usage */
extern const cmox_ecc_impl_t CMOX_ECC_BPP384R1_HIGHMEM;     /*!< BRAINPOOL-R P-384, high RAM usage */
extern const cmox_ecc_impl_t CMOX_ECC_BPP512R1_LOWMEM;      /*!< BRAINPOOL-R P-512, low RAM usage */
extern const cmox_ecc_impl_t CMOX_ECC_BPP512R1_HIGHMEM;     /*!< BRAINPOOL-R P-512, high RAM usage */

extern const cmox_ecc_impl_t CMOX_ECC_BPP160T1_LOWMEM;      /*!< BRAINPOOL-T P-160, low RAM usage */
extern const cmox_ecc_impl_t CMOX_ECC_BPP160T1_HIGHMEM;     /*!< BRAINPOOL-T P-160, high RAM usage */
extern const cmox_ecc_impl_t CMOX_ECC_BPP192T1_LOWMEM;      /*!< BRAINPOOL-T P-192, low RAM usage */
extern const cmox_ecc_impl_t CMOX_ECC_BPP192T1_HIGHMEM;     /*!< BRAINPOOL-T P-192, high RAM usage */
extern const cmox_ecc_impl_t CMOX_ECC_BPP224T1_LOWMEM;      /*!< BRAINPOOL-T P-224, low RAM usage */
extern const cmox_ecc_impl_t CMOX_ECC_BPP224T1_HIGHMEM;     /*!< BRAINPOOL-T P-224, high RAM usage */
extern const cmox_ecc_impl_t CMOX_ECC_BPP256T1_LOWMEM;      /*!< BRAINPOOL-T P-256, low RAM usage */
extern const cmox_ecc_impl_t CMOX_ECC_BPP256T1_HIGHMEM;     /*!< BRAINPOOL-T P-256, high RAM usage */
extern const cmox_ecc_impl_t CMOX_ECC_BPP320T1_LOWMEM;      /*!< BRAINPOOL-T P-320, low RAM usage */
extern const cmox_ecc_impl_t CMOX_ECC_BPP320T1_HIGHMEM;     /*!< BRAINPOOL-T P-320, high RAM usage */
extern const cmox_ecc_impl_t CMOX_ECC_BPP384T1_LOWMEM;      /*!< BRAINPOOL-T P-384, low RAM usage */
extern const cmox_ecc_impl_t CMOX_ECC_BPP384T1_HIGHMEM;     /*!< BRAINPOOL-T P-384, high RAM usage */
extern const cmox_ecc_impl_t CMOX_ECC_BPP512T1_LOWMEM;      /*!< BRAINPOOL-T P-512, low RAM usage */
extern const cmox_ecc_impl_t CMOX_ECC_BPP512T1_HIGHMEM;     /*!< BRAINPOOL-T P-512, high RAM usage */

extern const cmox_ecc_impl_t CMOX_ECC_FRP256V1_LOWMEM;      /*!< ANSSI P-256, low RAM usage */
extern const cmox_ecc_impl_t CMOX_ECC_FRP256V1_HIGHMEM;     /*!< ANSSI P-256, high RAM usage */

extern const cmox_ecc_impl_t CMOX_ECC_SM2_LOWMEM;           /*!< OSCCA 256 bit curve, low RAM usage */
extern const cmox_ecc_impl_t CMOX_ECC_SM2_HIGHMEM;          /*!< OSCCA 256 bit curve, high RAM usage */
extern const cmox_ecc_impl_t CMOX_ECC_SM2TEST_LOWMEM;       /*!< OSCCA 256 bit test curve, low RAM usage */
extern const cmox_ecc_impl_t CMOX_ECC_SM2TEST_HIGHMEM;      /*!< OSCCA 256 bit test curve, high RAM usage */
/**
  * @}
  */

/**
  * @defgroup CMOX_ECC_OUTPUT_LENGTHS Macros for output buffers definitions
  * @{
  */
#define CMOX_ECC_CURVE25519_PRIVKEY_LEN   32u /*!< Byte length for a byte buffer containing a CURVE25519 private key */
#define CMOX_ECC_CURVE25519_PUBKEY_LEN    32u /*!< Byte length for a byte buffer containing a CURVE25519 public key */
#define CMOX_ECC_CURVE25519_SECRET_LEN    32u /*!< Byte length for a byte buffer containing a CURVE25519 ECDH secret */
#define CMOX_ECC_CURVE448_PRIVKEY_LEN     56u /*!< Byte length for a byte buffer containing a CURVE448 private key */
#define CMOX_ECC_CURVE448_PUBKEY_LEN      56u /*!< Byte length for a byte buffer containing a CURVE448 public key */
#define CMOX_ECC_CURVE448_SECRET_LEN      56u /*!< Byte length for a byte buffer containing a CURVE448 ECDH secret */
#define CMOX_ECC_ED25519_SIG_LEN          64u /*!< Byte length for a byte buffer containing a ED25519 signature */
#define CMOX_ECC_ED25519_PRIVKEY_LEN      64u /*!< Byte length for a byte buffer containing a ED25519 private key */
#define CMOX_ECC_ED25519_PUBKEY_LEN       32u /*!< Byte length for a byte buffer containing a ED25519 public key */
#define CMOX_ECC_ED448_SIG_LEN           114u /*!< Byte length for a byte buffer containing a ED448 signature */
#define CMOX_ECC_ED448_PRIVKEY_LEN       114u /*!< Byte length for a byte buffer containing a ED448 private key */
#define CMOX_ECC_ED448_PUBKEY_LEN         57u /*!< Byte length for a byte buffer containing a ED448 public key */
#define CMOX_ECC_SECP224R1_SIG_LEN        56u /*!< Byte length for a byte buffer containing a SECP224R1 signature */
#define CMOX_ECC_SECP224R1_PRIVKEY_LEN    28u /*!< Byte length for a byte buffer containing a SECP224R1 private key */
#define CMOX_ECC_SECP224R1_PUBKEY_LEN     56u /*!< Byte length for a byte buffer containing a SECP224R1 public key */
#define CMOX_ECC_SECP224R1_SECRET_LEN     56u /*!< Byte length for a byte buffer containing a SECP224R1 ECDH secret */
#define CMOX_ECC_SECP256R1_SIG_LEN        64u /*!< Byte length for a byte buffer containing a SECP256R1 signature */
#define CMOX_ECC_SECP256R1_PRIVKEY_LEN    32u /*!< Byte length for a byte buffer containing a SECP256R1 private key */
#define CMOX_ECC_SECP256R1_PUBKEY_LEN     64u /*!< Byte length for a byte buffer containing a SECP256R1 public key */
#define CMOX_ECC_SECP256R1_SECRET_LEN     64u /*!< Byte length for a byte buffer containing a SECP256R1 ECDH secret */
#define CMOX_ECC_SECP384R1_SIG_LEN        96u /*!< Byte length for a byte buffer containing a SECP384R1 signature */
#define CMOX_ECC_SECP384R1_PRIVKEY_LEN    48u /*!< Byte length for a byte buffer containing a SECP384R1 private key */
#define CMOX_ECC_SECP384R1_PUBKEY_LEN     96u /*!< Byte length for a byte buffer containing a SECP384R1 public key */
#define CMOX_ECC_SECP384R1_SECRET_LEN     96u /*!< Byte length for a byte buffer containing a SECP384R1 ECDH secret */
#define CMOX_ECC_SECP521R1_SIG_LEN       132u /*!< Byte length for a byte buffer containing a SECP521R1 signature */
#define CMOX_ECC_SECP521R1_PRIVKEY_LEN    66u /*!< Byte length for a byte buffer containing a SECP521R1 private key */
#define CMOX_ECC_SECP521R1_PUBKEY_LEN    132u /*!< Byte length for a byte buffer containing a SECP521R1 public key */
#define CMOX_ECC_SECP521R1_SECRET_LEN    132u /*!< Byte length for a byte buffer containing a SECP521R1 ECDH secret */
#define CMOX_ECC_SECP256K1_SIG_LEN        64u /*!< Byte length for a byte buffer containing a SECP256K1 signature */
#define CMOX_ECC_SECP256K1_PRIVKEY_LEN    32u /*!< Byte length for a byte buffer containing a SECP256K1 private key */
#define CMOX_ECC_SECP256K1_PUBKEY_LEN     64u /*!< Byte length for a byte buffer containing a SECP256K1 public key */
#define CMOX_ECC_SECP256K1_SECRET_LEN     64u /*!< Byte length for a byte buffer containing a SECP256K1 ECDH secret */
#define CMOX_ECC_BPP160R1_SIG_LEN         40u /*!< Byte length for a byte buffer containing a BPP160R1 signature */
#define CMOX_ECC_BPP160R1_PRIVKEY_LEN     20u /*!< Byte length for a byte buffer containing a BPP160R1 private key */
#define CMOX_ECC_BPP160R1_PUBKEY_LEN      40u /*!< Byte length for a byte buffer containing a BPP160R1 public key */
#define CMOX_ECC_BPP160R1_SECRET_LEN      40u /*!< Byte length for a byte buffer containing a BPP160R1 ECDH secret */
#define CMOX_ECC_BPP192R1_SIG_LEN         48u /*!< Byte length for a byte buffer containing a BPP192R1 signature */
#define CMOX_ECC_BPP192R1_PRIVKEY_LEN     24u /*!< Byte length for a byte buffer containing a BPP192R1 private key */
#define CMOX_ECC_BPP192R1_PUBKEY_LEN      48u /*!< Byte length for a byte buffer containing a BPP192R1 public key */
#define CMOX_ECC_BPP192R1_SECRET_LEN      48u /*!< Byte length for a byte buffer containing a BPP192R1 ECDH secret */
#define CMOX_ECC_BPP224R1_SIG_LEN         56u /*!< Byte length for a byte buffer containing a BPP224R1 signature */
#define CMOX_ECC_BPP224R1_PRIVKEY_LEN     28u /*!< Byte length for a byte buffer containing a BPP224R1 private key */
#define CMOX_ECC_BPP224R1_PUBKEY_LEN      56u /*!< Byte length for a byte buffer containing a BPP224R1 public key */
#define CMOX_ECC_BPP224R1_SECRET_LEN      56u /*!< Byte length for a byte buffer containing a BPP224R1 ECDH secret */
#define CMOX_ECC_BPP256R1_SIG_LEN         64u /*!< Byte length for a byte buffer containing a BPP256R1 signature */
#define CMOX_ECC_BPP256R1_PRIVKEY_LEN     32u /*!< Byte length for a byte buffer containing a BPP256R1 private key */
#define CMOX_ECC_BPP256R1_PUBKEY_LEN      64u /*!< Byte length for a byte buffer containing a BPP256R1 public key */
#define CMOX_ECC_BPP256R1_SECRET_LEN      64u /*!< Byte length for a byte buffer containing a BPP256R1 ECDH secret */
#define CMOX_ECC_BPP320R1_SIG_LEN         80u /*!< Byte length for a byte buffer containing a BPP320R1 signature */
#define CMOX_ECC_BPP320R1_PRIVKEY_LEN     40u /*!< Byte length for a byte buffer containing a BPP320R1 private key */
#define CMOX_ECC_BPP320R1_PUBKEY_LEN      80u /*!< Byte length for a byte buffer containing a BPP320R1 public key */
#define CMOX_ECC_BPP320R1_SECRET_LEN      80u /*!< Byte length for a byte buffer containing a BPP320R1 ECDH secret */
#define CMOX_ECC_BPP384R1_SIG_LEN         96u /*!< Byte length for a byte buffer containing a BPP384R1 signature */
#define CMOX_ECC_BPP384R1_PRIVKEY_LEN     48u /*!< Byte length for a byte buffer containing a BPP384R1 private key */
#define CMOX_ECC_BPP384R1_PUBKEY_LEN      96u /*!< Byte length for a byte buffer containing a BPP384R1 public key */
#define CMOX_ECC_BPP384R1_SECRET_LEN      96u /*!< Byte length for a byte buffer containing a BPP384R1 ECDH secret */
#define CMOX_ECC_BPP512R1_SIG_LEN        128u /*!< Byte length for a byte buffer containing a BPP512R1 signature */
#define CMOX_ECC_BPP512R1_PRIVKEY_LEN     64u /*!< Byte length for a byte buffer containing a BPP512R1 private key */
#define CMOX_ECC_BPP512R1_PUBKEY_LEN     128u /*!< Byte length for a byte buffer containing a BPP512R1 public key */
#define CMOX_ECC_BPP512R1_SECRET_LEN     128u /*!< Byte length for a byte buffer containing a BPP512R1 ECDH secret */
#define CMOX_ECC_BPP160T1_SIG_LEN         40u /*!< Byte length for a byte buffer containing a BPP160T1 signature */
#define CMOX_ECC_BPP160T1_PRIVKEY_LEN     20u /*!< Byte length for a byte buffer containing a BPP160T1 private key */
#define CMOX_ECC_BPP160T1_PUBKEY_LEN      40u /*!< Byte length for a byte buffer containing a BPP160T1 public key */
#define CMOX_ECC_BPP160T1_SECRET_LEN      40u /*!< Byte length for a byte buffer containing a BPP160T1 ECDH secret */
#define CMOX_ECC_BPP192T1_SIG_LEN         48u /*!< Byte length for a byte buffer containing a BPP192T1 signature */
#define CMOX_ECC_BPP192T1_PRIVKEY_LEN     24u /*!< Byte length for a byte buffer containing a BPP192T1 private key */
#define CMOX_ECC_BPP192T1_PUBKEY_LEN      48u /*!< Byte length for a byte buffer containing a BPP192T1 public key */
#define CMOX_ECC_BPP192T1_SECRET_LEN      48u /*!< Byte length for a byte buffer containing a BPP192T1 ECDH secret */
#define CMOX_ECC_BPP224T1_SIG_LEN         56u /*!< Byte length for a byte buffer containing a BPP224T1 signature */
#define CMOX_ECC_BPP224T1_PRIVKEY_LEN     28u /*!< Byte length for a byte buffer containing a BPP224T1 private key */
#define CMOX_ECC_BPP224T1_PUBKEY_LEN      56u /*!< Byte length for a byte buffer containing a BPP224T1 public key */
#define CMOX_ECC_BPP224T1_SECRET_LEN      56u /*!< Byte length for a byte buffer containing a BPP224T1 ECDH secret */
#define CMOX_ECC_BPP256T1_SIG_LEN         64u /*!< Byte length for a byte buffer containing a BPP256T1 signature */
#define CMOX_ECC_BPP256T1_PRIVKEY_LEN     32u /*!< Byte length for a byte buffer containing a BPP256T1 private key */
#define CMOX_ECC_BPP256T1_PUBKEY_LEN      64u /*!< Byte length for a byte buffer containing a BPP256T1 public key */
#define CMOX_ECC_BPP256T1_SECRET_LEN      64u /*!< Byte length for a byte buffer containing a BPP256T1 ECDH secret */
#define CMOX_ECC_BPP320T1_SIG_LEN         80u /*!< Byte length for a byte buffer containing a BPP320T1 signature */
#define CMOX_ECC_BPP320T1_PRIVKEY_LEN     40u /*!< Byte length for a byte buffer containing a BPP320T1 private key */
#define CMOX_ECC_BPP320T1_PUBKEY_LEN      80u /*!< Byte length for a byte buffer containing a BPP320T1 public key */
#define CMOX_ECC_BPP320T1_SECRET_LEN      80u /*!< Byte length for a byte buffer containing a BPP320T1 ECDH secret */
#define CMOX_ECC_BPP384T1_SIG_LEN         96u /*!< Byte length for a byte buffer containing a BPP384T1 signature */
#define CMOX_ECC_BPP384T1_PRIVKEY_LEN     48u /*!< Byte length for a byte buffer containing a BPP384T1 private key */
#define CMOX_ECC_BPP384T1_PUBKEY_LEN      96u /*!< Byte length for a byte buffer containing a BPP384T1 public key */
#define CMOX_ECC_BPP384T1_SECRET_LEN      96u /*!< Byte length for a byte buffer containing a BPP384T1 ECDH secret */
#define CMOX_ECC_BPP512T1_SIG_LEN        128u /*!< Byte length for a byte buffer containing a BPP512T1 signature */
#define CMOX_ECC_BPP512T1_PRIVKEY_LEN     64u /*!< Byte length for a byte buffer containing a BPP512T1 private key */
#define CMOX_ECC_BPP512T1_PUBKEY_LEN     128u /*!< Byte length for a byte buffer containing a BPP512T1 public key */
#define CMOX_ECC_BPP512T1_SECRET_LEN     128u /*!< Byte length for a byte buffer containing a BPP512T1 ECDH secret */
#define CMOX_ECC_FRP256V1_SIG_LEN         64u /*!< Byte length for a byte buffer containing a FRP256V1 signature */
#define CMOX_ECC_FRP256V1_PRIVKEY_LEN     32u /*!< Byte length for a byte buffer containing a FRP256V1 private key */
#define CMOX_ECC_FRP256V1_PUBKEY_LEN      64u /*!< Byte length for a byte buffer containing a FRP256V1 public key */
#define CMOX_ECC_FRP256V1_SECRET_LEN      64u /*!< Byte length for a byte buffer containing a FRP256V1 ECDH secret */
#define CMOX_ECC_SM2_SIG_LEN              64u /*!< Byte length for a byte buffer containing a SM2 signature */
#define CMOX_ECC_SM2_PRIVKEY_LEN          32u /*!< Byte length for a byte buffer containing a SM2 private key */
#define CMOX_ECC_SM2_PUBKEY_LEN           64u /*!< Byte length for a byte buffer containing a SM2 public key */
#define CMOX_ECC_SM2_SECRET_LEN           64u /*!< Byte length for a byte buffer containing a SM2 ECDH secret */
#define CMOX_ECC_SM2TEST_SIG_LEN          64u /*!< Byte length for a byte buffer containing a SM2TEST signature */
#define CMOX_ECC_SM2TEST_PRIVKEY_LEN      32u /*!< Byte length for a byte buffer containing a SM2TEST private key */
#define CMOX_ECC_SM2TEST_PUBKEY_LEN       64u /*!< Byte length for a byte buffer containing a SM2TEST public key */
#define CMOX_ECC_SM2TEST_SECRET_LEN       64u /*!< Byte length for a byte buffer containing a SM2TEST ECDH secret */
/**
  * @}
  */

/**
  * @defgroup CMOX_ECC_PUBLIC_METHODS ECC public method prototypes
  * @{
  */

/**
  * @brief         Mandatory function to set Low Level Mathematical Functions, and working memory buffer, for ECC
  * @param[in,out] P_pEccCtx  Context for ECC operations
  * @param[in]     P_Math     Structure pointer with the Low-level Mathematical functions
  *                           This parameter can be one of the following:
  *                              @arg CMOX_MATH_FUNCS_SMALL
  *                              @arg CMOX_MATH_FUNCS_FAST
  *                              @arg CMOX_MATH_FUNCS_SUPERFAST256
  * @param[in]     P_pBuf     The preallocated static buffer that will be used
  * @param[in]     P_BufLen   The size in bytes of the P_pBuf buffer
  */
void cmox_ecc_construct(cmox_ecc_handle_t        *P_pEccCtx,
                        const cmox_math_funcs_t  P_Math,
                        uint8_t                  *P_pBuf,
                        size_t                   P_BufLen);

/**
  * @brief         Clean the ECC context and the internal temporary buffer
  * @param[in,out] P_pEccCtx  Context for ECC operations
  */
void cmox_ecc_cleanup(cmox_ecc_handle_t *P_pEccCtx);

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* CMOX_ECC_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
