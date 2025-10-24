/**
  ******************************************************************************
  * @file    cmox_crypto.h
  * @author  MCD Application Team
  * @brief   Header file including all the supported cryptographic algorithms
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

#ifndef CMOX_CRYPTO_H
#define CMOX_CRYPTO_H

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/** @defgroup CMOX_CRYPTO Cortex-M Optimized Crypto Stack
  * @{
  */

/** @defgroup CMOX_HASH Hash module
  * @{
  */

#include "hash/cmox_sha1.h"
#include "hash/cmox_sha224.h"
#include "hash/cmox_sha256.h"
#include "hash/cmox_sha384.h"
#include "hash/cmox_sha512.h"
#include "hash/cmox_sha3.h"
#include "hash/cmox_sm3.h"

/**
  * @}
  */

/** @defgroup CMOX_CIPHER Cipher module
  * @{
  */

#include "cipher/cmox_cbc.h"
#include "cipher/cmox_ccm.h"
#include "cipher/cmox_cfb.h"
#include "cipher/cmox_chachapoly.h"
#include "cipher/cmox_ctr.h"
#include "cipher/cmox_ecb.h"
#include "cipher/cmox_gcm.h"
#include "cipher/cmox_keywrap.h"
#include "cipher/cmox_ofb.h"
#include "cipher/cmox_xts.h"

/**
  * @}
  */

/** @defgroup CMOX_MAC MAC module
  * @{
  */

#include "mac/cmox_cmac.h"
#include "mac/cmox_hmac.h"
#include "mac/cmox_kmac.h"

/**
  * @}
  */

/** @defgroup CMOX_RSA RSA module
  * @{
  */
#include "rsa/cmox_rsa_pkcs1v15.h"
#include "rsa/cmox_rsa_pkcs1v22.h"
/**
  * @}
  */

/** @defgroup CMOX_ECC ECC module
  * @{
  */
#include "ecc/cmox_ecdsa.h"
#include "ecc/cmox_eddsa.h"
#include "ecc/cmox_sm2.h"
#include "ecc/cmox_ecdh.h"
/**
  * @}
  */

/** @defgroup CMOX_DRBG DRBG module
  *  @{
  */
#include "drbg/cmox_ctr_drbg.h"
/**
  * @}
  */

/** @defgroup CMOX_UTILS Utils module
  *  @{
  */
#include "utils/cmox_utils_compare.h"
/**
  * @}
  */

/** @defgroup CMOX_INFO Information module
  *  @{
  */
#include "cmox_info.h"
/**
  * @}
  */

/** @defgroup CMOX_INIT Initialization module
  *  @{
  */
#include "cmox_init.h"
/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* CMOX_CRYPTO_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
