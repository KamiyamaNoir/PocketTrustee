/**
  *******************************************************************************
  * @file    cmox_default_defs.h
  * @author  MCD Application Team
  * @brief   Header file that defines the macros for default algorithms
  *          optimizations
  * @note    Do not modify the content of this file for changing the default
  *          algorithm optimizations. Instead, modify the content of
  *          cmox_default_config.h or use a custom header file with desired
  *          configuration. In this case it must be set the macro
  *          CMOX_DEFAULT_FILE with the name of the custom file.
  *******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  *******************************************************************************
  */

#ifndef CMOX_DEFAULT_DEFS_H
#define CMOX_DEFAULT_DEFS_H


#if !defined(CMOX_DEFAULT_FILE)
#include "cmox_default_config.h"
#else
#include CMOX_DEFAULT_FILE
#endif /* CMOX_DEFAULT_FILE */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/** @defgroup CMOX_DEFAULT_MACROS Default algorithms optimizations
  * @{
  */

/** @defgroup CMOX_DEFAULT_IMPL Default implementations
  * @{
  */

#if (CMOX_AES_IMPLEMENTATION == CMOX_AES_FAST)

/**
  * @brief Implementation of CBC Encryption using AES
  */
#define CMOX_AES_CBC_ENC CMOX_AESFAST_CBC_ENC

/**
  * @brief Implementation of CBC Decryption using AES
  */
#define CMOX_AES_CBC_DEC CMOX_AESFAST_CBC_DEC

/**
  * @brief Implementation of CCM Encryption using AES
  */
#define CMOX_AES_CCM_ENC CMOX_AESFAST_CCM_ENC

/**
  * @brief Implementation of CCM Decryption using AES
  */
#define CMOX_AES_CCM_DEC CMOX_AESFAST_CCM_DEC

/**
  * @brief Implementation of CFB Encryption using AES
  */
#define CMOX_AES_CFB_ENC CMOX_AESFAST_CFB_ENC

/**
  * @brief Implementation of CFB Decryption using AES
  */
#define CMOX_AES_CFB_DEC CMOX_AESFAST_CFB_DEC

/**
  * @brief Implementation of CTR Encryption using AES
  */
#define CMOX_AES_CTR_ENC CMOX_AESFAST_CTR_ENC

/**
  * @brief Implementation of CTR Decryption using AES
  */
#define CMOX_AES_CTR_DEC CMOX_AESFAST_CTR_DEC

/**
  * @brief Implementation of ECB Encryption using AES
  */
#define CMOX_AES_ECB_ENC CMOX_AESFAST_ECB_ENC

/**
  * @brief Implementation of ECB Decryption using AES
  */
#define CMOX_AES_ECB_DEC CMOX_AESFAST_ECB_DEC

#if (CMOX_GCM_IMPLEMENTATION == CMOX_GCM_FAST)

/**
  * @brief Implementation of GCM Encryption using AES
  */
#define CMOX_AES_GCM_ENC CMOX_AESFAST_GCMFAST_ENC

/**
  * @brief Implementation of GCM Decryption using AES
  */
#define CMOX_AES_GCM_DEC CMOX_AESFAST_GCMFAST_DEC


#else /* CMOX_GCM_IMPLEMENTATION == CMOX_GCM_SMALL */

/**
  * @brief Implementation of GCM Encryption using AES
  */
#define CMOX_AES_GCM_ENC CMOX_AESFAST_GCMSMALL_ENC

/**
  * @brief Implementation of GCM Decryption using AES
  */
#define CMOX_AES_GCM_DEC CMOX_AESFAST_GCMSMALL_DEC

#endif /* CMOX_GCM_IMPLEMENTATION == CMOX_GCM_FAST */

/**
  * @brief Implementation of KEYWRAP Encryption using AES
  */
#define CMOX_AES_KEYWRAP_ENC CMOX_AESFAST_KEYWRAP_ENC

/**
  * @brief Implementation of KEYWRAP Decryption using AES
  */
#define CMOX_AES_KEYWRAP_DEC CMOX_AESFAST_KEYWRAP_DEC

/**
  * @brief Implementation of OFB Encryption using AES
  */
#define CMOX_AES_OFB_ENC CMOX_AESFAST_OFB_ENC

/**
  * @brief Implementation of OFB Decryption using AES
  */
#define CMOX_AES_OFB_DEC CMOX_AESFAST_OFB_DEC

/**
  * @brief Implementation of XTS Encryption using AES
  */
#define CMOX_AES_XTS_ENC CMOX_AESFAST_XTS_ENC

/**
  * @brief Implementation of XTS Decryption using AES
  */
#define CMOX_AES_XTS_DEC CMOX_AESFAST_XTS_DEC

/**
  * @brief Implementation of CTR-DRBG Encryption using AES128 with derivation
  *        function and no prediction resistance
  */
#define CMOX_CTR_DRBG_AES128 CMOX_CTR_DRBG_AES128_FAST

/**
  * @brief Implementation of CTR-DRBG Encryption using AES256 with derivation
  *        function and no prediction resistance
  */
#define CMOX_CTR_DRBG_AES256 CMOX_CTR_DRBG_AES256_FAST

/**
  * @brief Implementation of  AES-CMAC
  */
#define CMOX_CMAC_AES CMOX_CMAC_AESFAST

#else /* CMOX_AES_IMPLEMENTATION == CMOX_AES_SMALL */

/**
  * @brief Implementation of CBC Encryption using AES
  */
#define CMOX_AES_CBC_ENC CMOX_AESSMALL_CBC_ENC

/**
  * @brief Implementation of CBC Decryption using AES
  */
#define CMOX_AES_CBC_DEC CMOX_AESSMALL_CBC_DEC

/**
  * @brief Implementation of CCM Encryption using AES
  */
#define CMOX_AES_CCM_ENC CMOX_AESSMALL_CCM_ENC

/**
  * @brief Implementation of CCM Decryption using AES
  */
#define CMOX_AES_CCM_DEC CMOX_AESSMALL_CCM_DEC

/**
  * @brief Implementation of CFB Encryption using AES
  */
#define CMOX_AES_CFB_ENC CMOX_AESSMALL_CFB_ENC

/**
  * @brief Implementation of CFB Decryption using AES
  */
#define CMOX_AES_CFB_DEC CMOX_AESSMALL_CFB_DEC

/**
  * @brief Implementation of CTR Encryption using AES
  */
#define CMOX_AES_CTR_ENC CMOX_AESSMALL_CTR_ENC

/**
  * @brief Implementation of CTR Decryption using AES
  */
#define CMOX_AES_CTR_DEC CMOX_AESSMALL_CTR_DEC

/**
  * @brief Implementation of ECB Encryption using AES
  */
#define CMOX_AES_ECB_ENC CMOX_AESSMALL_ECB_ENC

/**
  * @brief Implementation of ECB Decryption using AES
  */
#define CMOX_AES_ECB_DEC CMOX_AESSMALL_ECB_DEC

#if (CMOX_GCM_IMPLEMENTATION == CMOX_GCM_FAST)

/**
  * @brief Implementation of GCM Encryption using AES
  */
#define CMOX_AES_GCM_ENC CMOX_AESSMALL_GCMFAST_ENC

/**
  * @brief Implementation of GCM Decryption using AES
  */
#define CMOX_AES_GCM_DEC CMOX_AESSMALL_GCMFAST_DEC

#else /* CMOX_GCM_IMPLEMENTATION == CMOX_GCM_SMALL */

/**
  * @brief Implementation of GCM Encryption using AES
  */
#define CMOX_AES_GCM_ENC CMOX_AESSMALL_GCMSMALL_ENC

/**
  * @brief Implementation of GCM Decryption using AES
  */
#define CMOX_AES_GCM_DEC CMOX_AESSMALL_GCMSMALL_DEC

#endif /* CMOX_GCM_IMPLEMENTATION == CMOX_GCM_FAST */

/**
  * @brief Implementation of KEYWRAP Encryption using AES
  */
#define CMOX_AES_KEYWRAP_ENC CMOX_AESSMALL_KEYWRAP_ENC

/**
  * @brief Implementation of KEYWRAP Decryption using AES
  */
#define CMOX_AES_KEYWRAP_DEC CMOX_AESSMALL_KEYWRAP_DEC

/**
  * @brief Implementation of OFB Encryption using AES
  */
#define CMOX_AES_OFB_ENC CMOX_AESSMALL_OFB_ENC

/**
  * @brief Implementation of OFB Decryption using AES
  */
#define CMOX_AES_OFB_DEC CMOX_AESSMALL_OFB_DEC

/**
  * @brief Implementation of XTS Encryption using AES
  */
#define CMOX_AES_XTS_ENC CMOX_AESSMALL_XTS_ENC

/**
  * @brief Implementation of XTS Decryption using AES
  */
#define CMOX_AES_XTS_DEC CMOX_AESSMALL_XTS_DEC

/**
  * @brief Implementation of CTR-DRBG Encryption using AES128 with derivation
  *        function and no prediction resistance
  */
#define CMOX_CTR_DRBG_AES128 CMOX_CTR_DRBG_AES128_SMALL

/**
  * @brief Implementation of CTR-DRBG Encryption using AES256 with derivation
  *        function and no prediction resistance
  */
#define CMOX_CTR_DRBG_AES256 CMOX_CTR_DRBG_AES256_SMALL

/*
  * @brief Implementation of  AES-CMAC
  */
#define CMOX_CMAC_AES CMOX_CMAC_AESSMALL

#endif /* CMOX_AES_IMPLEMENTATION == CMOX_AES_FAST */

/**
  * @}
  */

/** @defgroup CMOX_DEFAULT_ALGO Default single-call algorithms
  * @{
  */

#if (CMOX_AES_IMPLEMENTATION == CMOX_AES_FAST)

/**
  * @brief Implementation of CBC Encryption using AES
  */
#define CMOX_AES_CBC_ENC_ALGO CMOX_AESFAST_CBC_ENC_ALGO

/**
  * @brief Implementation of CBC Decryption using AES
  */
#define CMOX_AES_CBC_DEC_ALGO CMOX_AESFAST_CBC_DEC_ALGO

/**
  * @brief Implementation of CCM Encryption using AES
  */
#define CMOX_AES_CCM_ENC_ALGO CMOX_AESFAST_CCM_ENC_ALGO

/**
  * @brief Implementation of CCM Decryption using AES
  */
#define CMOX_AES_CCM_DEC_ALGO CMOX_AESFAST_CCM_DEC_ALGO

/**
  * @brief Implementation of CFB Encryption using AES
  */
#define CMOX_AES_CFB_ENC_ALGO CMOX_AESFAST_CFB_ENC_ALGO

/**
  * @brief Implementation of CFB Decryption using AES
  */
#define CMOX_AES_CFB_DEC_ALGO CMOX_AESFAST_CFB_DEC_ALG

/**
  * @brief Implementation of CTR Encryption using AES
  */
#define CMOX_AES_CTR_ENC_ALGO CMOX_AESFAST_CTR_ENC_ALGO

/**
  * @brief Implementation of CTR Decryption using AES
  */
#define CMOX_AES_CTR_DEC_ALGO CMOX_AESFAST_CTR_DEC_ALGO

/**
  * @brief Implementation of ECB Encryption using AES
  */
#define CMOX_AES_ECB_ENC_ALGO CMOX_AESFAST_ECB_ENC_ALGO

/**
  * @brief Implementation of ECB Decryption using AES
  */
#define CMOX_AES_ECB_DEC_ALGO CMOX_AESFAST_ECB_DEC_ALGO

#if (CMOX_GCM_IMPLEMENTATION == CMOX_GCM_FAST)

/**
  * @brief Implementation of GCM Encryption using AES
  */
#define CMOX_AES_GCM_ENC_ALGO CMOX_AESFAST_GCMFAST_ENC_ALGO

/**
  * @brief Implementation of GCM Decryption using AES
  */
#define CMOX_AES_GCM_DEC_ALGO CMOX_AESFAST_GCMFAST_DEC_ALGO

#else /* CMOX_GCM_IMPLEMENTATION == CMOX_GCM_SMALL */

/**
  * @brief Implementation of GCM Encryption using AES
  */
#define CMOX_AES_GCM_ENC_ALGO CMOX_AESFAST_GCMSMALL_ENC_ALGO

/**
  * @brief Implementation of GCM Decryption using AES
  */
#define CMOX_AES_GCM_DEC_ALGO CMOX_AESFAST_GCMSMALL_DEC_ALGO

#endif /* CMOX_GCM_IMPLEMENTATION == CMOX_GCM_FAST */

/**
  * @brief Implementation of KEYWRAP Encryption using AES
  */
#define CMOX_AES_KEYWRAP_ENC_ALGO CMOX_AESFAST_KEYWRAP_ENC_ALGO

/**
  * @brief Implementation of KEYWRAP Decryption using AES
  */
#define CMOX_AES_KEYWRAP_DEC_ALGO CMOX_AESFAST_KEYWRAP_DEC_ALGO

/**
  * @brief Implementation of OFB Encryption using AES
  */
#define CMOX_AES_OFB_ENC_ALGO CMOX_AESFAST_OFB_ENC_ALGO

/**
  * @brief Implementation of OFB Decryption using AES
  */
#define CMOX_AES_OFB_DEC_ALGO CMOX_AESFAST_OFB_DEC_ALGO

/**
  * @brief Implementation of XTS Encryption using AES
  */
#define CMOX_AES_XTS_ENC_ALGO CMOX_AESFAST_XTS_ENC_ALGO

/**
  * @brief Implementation of XTS Decryption using AES
  */
#define CMOX_AES_XTS_DEC_ALGO CMOX_AESFAST_XTS_DEC_ALGO

/**
  * @brief Implementation of AES-CMAC algorithm
  */
#define CMOX_CMAC_AES_ALGO CMOX_CMAC_AESFAST_ALGO

#else /* CMOX_AES_IMPLEMENTATION == CMOX_AES_SMALL */

/**
  * @brief Implementation of CBC Encryption using AES
  */
#define CMOX_AES_CBC_ENC_ALGO CMOX_AESSMALL_CBC_ENC_ALGO

/**
  * @brief Implementation of CBC Decryption using AES
  */
#define CMOX_AES_CBC_DEC_ALGO CMOX_AESSMALL_CBC_DEC_ALGO

/**
  * @brief Implementation of CCM Encryption using AES
  */
#define CMOX_AES_CCM_ENC_ALGO CMOX_AESSMALL_CCM_ENC_ALGO

/**
  * @brief Implementation of CCM Decryption using AES
  */
#define CMOX_AES_CCM_DEC_ALGO CMOX_AESSMALL_CCM_DEC_ALGO

/**
  * @brief Implementation of CFB Encryption using AES
  */
#define CMOX_AES_CFB_ENC_ALGO CMOX_AESSMALL_CFB_ENC_ALGO

/**
  * @brief Implementation of CFB Decryption using AES
  */
#define CMOX_AES_CFB_DEC_ALGO CMOX_AESSMALL_CFB_DEC_ALGO

/**
  * @brief Implementation of CTR Encryption using AES
  */
#define CMOX_AES_CTR_ENC_ALGO CMOX_AESSMALL_CTR_ENC_ALGO

/**
  * @brief Implementation of CTR Decryption using AES
  */
#define CMOX_AES_CTR_DEC_ALGO CMOX_AESSMALL_CTR_DEC_ALGO

/**
  * @brief Implementation of ECB Encryption using AES
  */
#define CMOX_AES_ECB_ENC_ALGO CMOX_AESSMALL_ECB_ENC_ALGO

/**
  * @brief Implementation of ECB Decryption using AES
  */
#define CMOX_AES_ECB_DEC_ALGO CMOX_AESSMALL_ECB_DEC_ALGO

#if (CMOX_GCM_IMPLEMENTATION == CMOX_GCM_FAST)

/**
  * @brief Implementation of GCM Encryption using AES
  */
#define CMOX_AES_GCM_ENC_ALGO CMOX_AESSMALL_GCMFAST_ENC_ALGO

/**
  * @brief Implementation of GCM Decryption using AES
  */
#define CMOX_AES_GCM_DEC_ALGO CMOX_AESSMALL_GCMFAST_DEC_ALGO

#else /* CMOX_GCM_IMPLEMENTATION == CMOX_GCM_SMALL */

/**
  * @brief Implementation of GCM Encryption using AES
  */
#define CMOX_AES_GCM_ENC_ALGO CMOX_AESSMALL_GCMSMALL_ENC_ALGO

/**
  * @brief Implementation of GCM Decryption using AES
  */
#define CMOX_AES_GCM_DEC_ALGO CMOX_AESSMALL_GCMSMALL_DEC_ALGO

#endif /* CMOX_GCM_IMPLEMENTATION == CMOX_GCM_FAST */

/**
  * @brief Implementation of KEYWRAP Encryption using AES
  */
#define CMOX_AES_KEYWRAP_ENC_ALGO CMOX_AESSMALL_KEYWRAP_ENC_ALGO

/**
  * @brief Implementation of KEYWRAP Decryption using AES
  */
#define CMOX_AES_KEYWRAP_DEC_ALGO CMOX_AESSMALL_KEYWRAP_DEC_ALGO

/**
  * @brief Implementation of OFB Encryption using AES
  */
#define CMOX_AES_OFB_ENC_ALGO CMOX_AESSMALL_OFB_ENC_ALGO

/**
  * @brief Implementation of OFB Decryption using AES
  */
#define CMOX_AES_OFB_DEC_ALGO CMOX_AESSMALL_OFB_DEC_ALGO

/**
  * @brief Implementation of XTS Encryption using AES
  */
#define CMOX_AES_XTS_ENC_ALGO CMOX_AESSMALL_XTS_ENC_ALGO

/**
  * @brief Implementation of XTS Decryption using AES
  */
#define CMOX_AES_XTS_DEC_ALGO CMOX_AESSMALL_XTS_DEC_ALGO

/**
  * @brief Implementation of AES-CMAC algorithm
  */
#define CMOX_CMAC_AES_ALGO CMOX_CMAC_AESSMALL_ALGO

#endif /* CMOX_AES_IMPLEMENTATION == CMOX_AES_FAST */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CMOX_DEFAULT_DEFS_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
