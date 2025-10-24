/**
  ******************************************************************************
  * @file    cmox_ecdh.h
  * @author  MCD Application Team
  * @brief   Header file for ECDH key exchange definition and function
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

#ifndef CMOX_ECDH_H
#define CMOX_ECDH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ecc/cmox_ecc.h"

/** @addtogroup CMOX_ECC
  * @{
  */

/**
  * @defgroup CMOX_ECC_ECDH ECDH algorithm
  * @{
  */

/** @defgroup CMOX_ECC_ECDH_PUBLIC_METHODS ECDH public method prototypes
  * @{
  */

/**
  * @brief      Generate a shared secret from a private key and a public key
  * @param[in]  P_pEccCtx           Context for ECC operations
  * @param[in]  P_CurveParams       Curve Parameters and point functions
  *                                 This parameter can be one of the following:
  *                                    @arg CMOX_ECC_CURVE25519
  *                                    @arg CMOX_ECC_CURVE448
  *                                    @arg CMOX_ECC_SECP224R1_LOWMEM
  *                                    @arg CMOX_ECC_SECP224R1_HIGHMEM
  *                                    @arg CMOX_ECC_SECP256R1_LOWMEM
  *                                    @arg CMOX_ECC_SECP256R1_HIGHMEM
  *                                    @arg CMOX_ECC_SECP384R1_LOWMEM
  *                                    @arg CMOX_ECC_SECP384R1_HIGHMEM
  *                                    @arg CMOX_ECC_SECP521R1_LOWMEM
  *                                    @arg CMOX_ECC_SECP521R1_HIGHMEM
  *                                    @arg CMOX_ECC_SECP256K1_LOWMEM
  *                                    @arg CMOX_ECC_SECP256K1_HIGHMEM
  *                                    @arg CMOX_ECC_BPP160R1_LOWMEM
  *                                    @arg CMOX_ECC_BPP160R1_HIGHMEM
  *                                    @arg CMOX_ECC_BPP192R1_LOWMEM
  *                                    @arg CMOX_ECC_BPP192R1_HIGHMEM
  *                                    @arg CMOX_ECC_BPP224R1_LOWMEM
  *                                    @arg CMOX_ECC_BPP224R1_HIGHMEM
  *                                    @arg CMOX_ECC_BPP256R1_LOWMEM
  *                                    @arg CMOX_ECC_BPP256R1_HIGHMEM
  *                                    @arg CMOX_ECC_BPP320R1_LOWMEM
  *                                    @arg CMOX_ECC_BPP320R1_HIGHMEM
  *                                    @arg CMOX_ECC_BPP384R1_LOWMEM
  *                                    @arg CMOX_ECC_BPP384R1_HIGHMEM
  *                                    @arg CMOX_ECC_BPP512R1_LOWMEM
  *                                    @arg CMOX_ECC_BPP512R1_HIGHMEM
  *                                    @arg CMOX_ECC_BPP160T1_LOWMEM
  *                                    @arg CMOX_ECC_BPP160T1_HIGHMEM
  *                                    @arg CMOX_ECC_BPP192T1_LOWMEM
  *                                    @arg CMOX_ECC_BPP192T1_HIGHMEM
  *                                    @arg CMOX_ECC_BPP224T1_LOWMEM
  *                                    @arg CMOX_ECC_BPP224T1_HIGHMEM
  *                                    @arg CMOX_ECC_BPP256T1_LOWMEM
  *                                    @arg CMOX_ECC_BPP256T1_HIGHMEM
  *                                    @arg CMOX_ECC_BPP320T1_LOWMEM
  *                                    @arg CMOX_ECC_BPP320T1_HIGHMEM
  *                                    @arg CMOX_ECC_BPP384T1_LOWMEM
  *                                    @arg CMOX_ECC_BPP384T1_HIGHMEM
  *                                    @arg CMOX_ECC_BPP512T1_LOWMEM
  *                                    @arg CMOX_ECC_BPP512T1_HIGHMEM
  *                                    @arg CMOX_ECC_FRP256V1_LOWMEM
  *                                    @arg CMOX_ECC_FRP256V1_HIGHMEM
  * @param[in]  P_pPrivKey          Buffer with the private key
  * @param[in]  P_PrivKeyLen        Private key length
  * @param[in]  P_pPubKey           Buffer with the public key
  * @param[in]  P_PubKeyLen         Public key length
  * @param[out] P_pSharedSecret     Buffer for the shared secret
  * @param[out] P_pSharedSecretLen  Shared secret length
  * @retval     CMOX_ECC_SUCCESS                Everything OK
  * @retval     CMOX_ECC_ERR_MATHCURVE_MISMATCH Mathematical function set is not compatible with current ECC curve
  * @retval     CMOX_ECC_ERR_ALGOCURVE_MISMATCH Curve is not compatible with current functionality
  * @retval     CMOX_ECC_ERR_BAD_PARAMETERS     Some NULL/wrong/empty parameter or Construct API not called
  * @retval     CMOX_ECC_ERR_MEMORY_FAIL        Not enough memory
  * @retval     CMOX_ECC_ERR_INVALID_PUBKEY     Public key not in a valid format
  */
cmox_ecc_retval_t cmox_ecdh(cmox_ecc_handle_t     *P_pEccCtx,
                            const cmox_ecc_impl_t P_CurveParams,
                            const uint8_t         *P_pPrivKey,
                            size_t                P_PrivKeyLen,
                            const uint8_t         *P_pPubKey,
                            size_t                P_PubKeyLen,
                            uint8_t               *P_pSharedSecret,
                            size_t                *P_pSharedSecretLen);

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

#endif /* CMOX_ECDH_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
