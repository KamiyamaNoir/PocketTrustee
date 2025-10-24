/**
  ******************************************************************************
  * @file    cmox_rsa.h
  * @author  MCD Application Team
  * @brief   This file provides common function for RSA module
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

#ifndef CMOX_RSA_H
#define CMOX_RSA_H

#include <stdint.h>
#include <stdlib.h>

#include "rsa/cmox_rsa_types.h"
#if !defined(CMOX_DEFAULT_FILE)
#include "cmox_default_config.h"
#else
#include CMOX_DEFAULT_FILE
#endif /* CMOX_DEFAULT_FILE */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/** @addtogroup CMOX_RSA
  * @{
  */

/**
  * @defgroup CMOX_RSA_PUBLIC_METHODS RSA public method prototypes
  * @{
  */

/**
  * @brief         Mandatory function to set Low Level Mathematical Functions, and working memory buffer, for RSA
  * @param[in,out] P_pRsaCtx  Context for RSA operations
  * @param[in]     P_Math     Structure pointer with the Low-level Mathematical functions
  *                           This parameter can be one of the following:
  *                              @arg CMOX_MATH_FUNCS_SMALL
  *                              @arg CMOX_MATH_FUNCS_FAST
  * @param[in]     P_Modexp   Structure pointer with the Modular Exponentiation function
  *                           This parameter can be one of the following:
  *                              @arg CMOX_MODEXP_PUBLIC
  *                              @arg CMOX_MODEXP_PRIVATE_LOWMEM
  *                              @arg CMOX_MODEXP_PRIVATE_MIDMEM
  *                              @arg CMOX_MODEXP_PRIVATE_HIGHMEM
  * @param[in]     P_pBuf     The preallocated static buffer that will be used
  * @param[in]     P_BufLen   The size in bytes of the P_pBuf buffer
  */
void cmox_rsa_construct(cmox_rsa_handle_t        *P_pRsaCtx,
                        const cmox_math_funcs_t  P_Math,
                        const cmox_modexp_func_t P_Modexp,
                        uint8_t                  *P_pBuf,
                        size_t                   P_BufLen);

/**
  * @brief         Clean the RSA context and the internal temporary buffer
  * @param[in,out] P_pRsaCtx  Context for RSA operations
  */
void cmox_rsa_cleanup(cmox_rsa_handle_t *P_pRsaCtx);

/**
  * @brief       Set the key (public or private) in the key structure
  * @param[out]  P_pKey        Key to set
  * @param[in]   P_pModulus    Modulus
  * @param[in]   P_ModulusLen  Modulus Length (in Bytes)
  * @param[in]   P_pExp        Exponent (private or public)
  * @param[in]   P_ExpLen      Exponent Length (in Bytes)
  * @retval      CMOX_RSA_SUCCESS
  * @retval      CMOX_RSA_ERR_BAD_PARAMETER
  */
cmox_rsa_retval_t cmox_rsa_setKey(cmox_rsa_key_t *P_pKey,
                                  const uint8_t  *P_pModulus,
                                  size_t         P_ModulusLen,
                                  const uint8_t  *P_pExp,
                                  size_t         P_ExpLen);

/**
  * @brief       Set the private CRT key in the key structure
  * @param[out]  P_pPrivKey      Private Key to set
  * @param[in]   P_ModulusBitLen Modulus Length (in bits)
  * @param[in]   P_pExpP         Secret exponent (mod P) dP
  * @param[in]   P_ExpPLen       dP Length (in Bytes)
  * @param[in]   P_pExpQ         Secret exponent (mod Q) dQ
  * @param[in]   P_ExpQLen       dQ Length (in Bytes)
  * @param[in]   P_pP            First secret prime P
  * @param[in]   P_PLen          P Length (in Bytes)
  * @param[in]   P_pQ            Second secret prime Q
  * @param[in]   P_QLen          Q Length (in Bytes)
  * @param[in]   P_pIq           Inverse of Q (mod P) invQ
  * @param[in]   P_IqLen         invQ Length (in Bytes)
  * @retval      CMOX_RSA_SUCCESS
  * @retval      CMOX_RSA_ERR_BAD_PARAMETER
  */
cmox_rsa_retval_t cmox_rsa_setKeyCRT(cmox_rsa_key_t *P_pPrivKey,
                                     size_t         P_ModulusBitLen,
                                     const uint8_t  *P_pExpP,
                                     size_t         P_ExpPLen,
                                     const uint8_t  *P_pExpQ,
                                     size_t         P_ExpQLen,
                                     const uint8_t  *P_pP,
                                     size_t         P_PLen,
                                     const uint8_t  *P_pQ,
                                     size_t         P_QLen,
                                     const uint8_t  *P_pIq,
                                     size_t         P_IqLen);

/**
  * @brief       Set the private CRT key in the key structure, enabling the Fault Attacks Countermeasure.
  * @param[out]  P_pPrivKey      Private Key to set
  * @param[in]   P_ModulusBitLen Modulus Length (in bits)
  * @param[in]   P_pExpP         Secret exponent (mod P) dP
  * @param[in]   P_ExpPLen       dP Length (in Bytes)
  * @param[in]   P_pExpQ         Secret exponent (mod Q) dQ
  * @param[in]   P_ExpQLen       dQ Length (in Bytes)
  * @param[in]   P_pP            First secret prime P
  * @param[in]   P_PLen          P Length (in Bytes)
  * @param[in]   P_pQ            Second secret prime Q
  * @param[in]   P_QLen          Q Length (in Bytes)
  * @param[in]   P_pIq           Inverse of Q (mod P) invQ
  * @param[in]   P_IqLen         invQ Length (in Bytes)
  * @param[in]   P_pPubKey       Public Key
  * @param[in]   P_PubKeyLen     Public Key Length (in Bytes)
  * @retval      CMOX_RSA_SUCCESS
  * @retval      CMOX_RSA_ERR_BAD_PARAMETER
  * @note        This function enables the protection against the Bellcore attack for RSA-CRT computation.
  *              This countermeasure is useful to protect RSA-CRT using PKCS#1 v1.5 for Signature Creation.
  *              Indeed, PKCS#1 v2.2 Signature Creation is not vulnerable because of the presence of an input random,
  *              and Decryption processes (both for PKCS#1 v1.5 and v2.2) are not vulnerable because the plaintext
  *              is not returned if not correct (therefore not exploitable by an attacker).
  */
cmox_rsa_retval_t cmox_rsa_setKeyCRTwithFACM(cmox_rsa_key_t *P_pPrivKey,
                                             size_t         P_ModulusBitLen,
                                             const uint8_t  *P_pExpP,
                                             size_t         P_ExpPLen,
                                             const uint8_t  *P_pExpQ,
                                             size_t         P_ExpQLen,
                                             const uint8_t  *P_pP,
                                             size_t         P_PLen,
                                             const uint8_t  *P_pQ,
                                             size_t         P_QLen,
                                             const uint8_t  *P_pIq,
                                             size_t         P_IqLen,
                                             const uint8_t  *P_pPubKey,
                                             size_t         P_PubKeyLen);

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* CMOX_RSA_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
