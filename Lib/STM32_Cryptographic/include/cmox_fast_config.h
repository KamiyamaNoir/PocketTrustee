/**
  ******************************************************************************
  * @file    cmox_fast_config.h
  * @author  MCD Application Team
  * @brief   Header file for fastest configuration of some algorithms
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
#ifndef CMOX_FAST_CONFIG_H
#define CMOX_FAST_CONFIG_H

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/**
  * @addtogroup CMOX_CRYPTO
  * @{
  */

/**
  * @defgroup CMOX_FAST CMOX fast configurations
  * @{
  */

/**
  * @defgroup CMOX_AES_FAST Fast AES configuration
  * @{
  */

/**
  * @brief Flag indicating the fast implementation of AES
  */
#define CMOX_AES_FAST   0x01U

/**
  * @brief Flag indicating the small implementation of AES
  */
#define CMOX_AES_SMALL  0x02U

/**
  * @brief Flag indicating the fast implementation of AES
  * @note  Value can be
  *        - @ref CMOX_AES_SMALL
  *        - @ref CMOX_AES_FAST
  */
#define CMOX_AES_IMPLEMENTATION CMOX_AES_FAST

/**
  * @}
  */

/**
  * @defgroup CMOX_GCM_FAST Fast GCM configuration
  * @{
  */

/**
  * @brief Flag indicating the fast implementation of GCM
  */
#define CMOX_GCM_FAST   0x0AU

/**
  * @brief Flag indicating the small implementation of GCM
  */
#define CMOX_GCM_SMALL  0x0BU

/**
  * @brief Flag indicating the fast implementation of GCM
  * @note  Value can be
  *        - @ref CMOX_GCM_SMALL
  *        - @ref CMOX_GCM_FAST
  */
#define CMOX_GCM_IMPLEMENTATION CMOX_GCM_FAST

/**
  * @}
  */

/**
  * @defgroup CMOX_MATH_FAST Fast Math customizations
  * @{
  */

/**
  * @brief Flag indicating the fast implementation for RSA low level mathematical functions
  * @note  Value can be
  *        - @ref CMOX_MATH_FUNCS_SMALL
  *        - @ref CMOX_MATH_FUNCS_FAST
  */
#define CMOX_RSA_MATH_FUNCS               CMOX_MATH_FUNCS_FAST

/**
  * @brief Flag indicating the fast implementation for ECC 256 bits curves
  *        low level mathematical functions
  * @note  Value can be
  *        - @ref CMOX_MATH_FUNCS_SMALL
  *        - @ref CMOX_MATH_FUNCS_FAST
  *        - @ref CMOX_MATH_FUNCS_SUPERFAST256
  */
#define CMOX_ECC256_MATH_FUNCS            CMOX_MATH_FUNCS_SUPERFAST256

/**
  * @brief Flag indicating the fast implementation for ECC 128 bits multiple curves
  *        low level mathematical functions
  * @note  Value can be
  *        - @ref CMOX_MATH_FUNCS_SMALL
  *        - @ref CMOX_MATH_FUNCS_FAST
  */
#define CMOX_ECC128MULT_MATH_FUNCS        CMOX_MATH_FUNCS_FAST

/**
  * @brief Flag indicating the fast implementation for ECC curves not defined by the above
  *        low level mathematical functions
  * @note  Value can be
  *        * for Cortex-M0/Cortex-M0+ devices
  *         - @ref CMOX_MATH_FUNCS_SMALL
  *         - @ref CMOX_MATH_FUNCS_FAST
  *        * Others
  *         - @ref CMOX_MATH_FUNCS_SMALL
  */
#if defined(__TARGET_ARCH_6M)
#define CMOX_ECC_MATH_FUNCS               CMOX_MATH_FUNCS_FAST
#else /* __TARGET_ARCH_6M */
#define CMOX_ECC_MATH_FUNCS               CMOX_MATH_FUNCS_SMALL
#endif /* __TARGET_ARCH_6M */

/**
  * @}
  */

/**
  * @defgroup CMOX_ECC_FAST Fast ECC customizations
  * @{
  */

/**
  * @brief Flag indicating the fast implementation for EDWARDS Ed25519 ECC curves
  * @note  Value can be
  *        - @ref CMOX_ECC_ED25519_OPT_LOWMEM
  *        - @ref CMOX_ECC_ED25519_HIGHMEM
  *        - @ref CMOX_ECC_ED25519_OPT_HIGHMEM
  */
#define CMOX_ECC_CURVE_ED25519       CMOX_ECC_ED25519_OPT_HIGHMEM
/**
  * @brief Flag indicating the fast implementation for EDWARDS Ed448 ECC curves
  * @note  Value can be
  *        - @ref CMOX_ECC_ED448_LOWMEM
  *        - @ref CMOX_ECC_ED448_HIGHMEM
  */
#define CMOX_ECC_CURVE_ED448         CMOX_ECC_ED448_HIGHMEM

/**
  * @brief Flag indicating the fast implementation for NIST-R P-224 ECC curves
  * @note  Value can be
  *        - @ref CMOX_ECC_SECP224R1_LOWMEM
  *        - @ref CMOX_ECC_SECP224R1_HIGHMEM
  */
#define CMOX_ECC_CURVE_SECP224R1     CMOX_ECC_SECP224R1_HIGHMEM
/**
  * @brief Flag indicating the fast implementation for NIST-R P-256 ECC curves
  * @note  Value can be
  *        - @ref CMOX_ECC_SECP256R1_LOWMEM
  *        - @ref CMOX_ECC_SECP256R1_HIGHMEM
  */
#define CMOX_ECC_CURVE_SECP256R1     CMOX_ECC_SECP256R1_HIGHMEM
/**
  * @brief Flag indicating the fast implementation for NIST-R P-384 ECC curves
  * @note  Value can be
  *        - @ref CMOX_ECC_SECP384R1_LOWMEM
  *        - @ref CMOX_ECC_SECP384R1_HIGHMEM
  */
#define CMOX_ECC_CURVE_SECP384R1     CMOX_ECC_SECP384R1_HIGHMEM
/**
  * @brief Flag indicating the fast implementation for NIST-R P-521 ECC curves
  * @note  Value can be
  *        - @ref CMOX_ECC_SECP521R1_LOWMEM
  *        - @ref CMOX_ECC_SECP521R1_HIGHMEM
  */
#define CMOX_ECC_CURVE_SECP521R1     CMOX_ECC_SECP521R1_HIGHMEM
/**
  * @brief Flag indicating the fast implementation for NIST-K P-256 ECC curves
  * @note  Value can be
  *        - @ref CMOX_ECC_SECP256K1_LOWMEM
  *        - @ref CMOX_ECC_SECP256K1_HIGHMEM
  */
#define CMOX_ECC_CURVE_SECP256K1     CMOX_ECC_SECP256K1_HIGHMEM

/**
  * @brief Flag indicating the fast implementation for BRAINPOOL-R P-160 ECC curves
  * @note  Value can be
  *        - @ref CMOX_ECC_BPP160R1_LOWMEM
  *        - @ref CMOX_ECC_BPP160R1_HIGHMEM
  */
#define CMOX_ECC_CURVE_BPP160R1      CMOX_ECC_BPP160R1_HIGHMEM
/**
  * @brief Flag indicating the fast implementation for BRAINPOOL-R P-192 ECC curves
  * @note  Value can be
  *        - @ref CMOX_ECC_BPP192R1_LOWMEM
  *        - @ref CMOX_ECC_BPP192R1_HIGHMEM
  */
#define CMOX_ECC_CURVE_BPP192R1      CMOX_ECC_BPP192R1_HIGHMEM
/**
  * @brief Flag indicating the fast implementation for BRAINPOOL-R P-224 ECC curves
  * @note  Value can be
  *        - @ref CMOX_ECC_BPP224R1_LOWMEM
  *        - @ref CMOX_ECC_BPP224R1_HIGHMEM
  */
#define CMOX_ECC_CURVE_BPP224R1      CMOX_ECC_BPP224R1_HIGHMEM
/**
  * @brief Flag indicating the fast implementation for BRAINPOOL-R P-256 ECC curves
  * @note  Value can be
  *        - @ref CMOX_ECC_BPP256R1_LOWMEM
  *        - @ref CMOX_ECC_BPP256R1_HIGHMEM
  */
#define CMOX_ECC_CURVE_BPP256R1      CMOX_ECC_BPP256R1_HIGHMEM
/**
  * @brief Flag indicating the fast implementation for BRAINPOOL-R P-320 ECC curves
  * @note  Value can be
  *        - @ref CMOX_ECC_BPP320R1_LOWMEM
  *        - @ref CMOX_ECC_BPP320R1_HIGHMEM
  */
#define CMOX_ECC_CURVE_BPP320R1      CMOX_ECC_BPP320R1_HIGHMEM
/**
  * @brief Flag indicating the fast implementation for BRAINPOOL-R P-384 ECC curves
  * @note  Value can be
  *        - @ref CMOX_ECC_BPP384R1_LOWMEM
  *        - @ref CMOX_ECC_BPP384R1_HIGHMEM
  */
#define CMOX_ECC_CURVE_BPP384R1      CMOX_ECC_BPP384R1_HIGHMEM
/**
  * @brief Flag indicating the fast implementation for BRAINPOOL-R P-512 ECC curves
  * @note  Value can be
  *        - @ref CMOX_ECC_BPP512R1_LOWMEM
  *        - @ref CMOX_ECC_BPP512R1_HIGHMEM
  */
#define CMOX_ECC_CURVE_BPP512R1      CMOX_ECC_BPP512R1_HIGHMEM
/**
  * @brief Flag indicating the fast implementation for BRAINPOOL-T P-160 ECC curves
  * @note  Value can be
  *        - @ref CMOX_ECC_BPP160T1_LOWMEM
  *        - @ref CMOX_ECC_BPP160T1_HIGHMEM
  */
#define CMOX_ECC_CURVE_BPP160T1      CMOX_ECC_BPP160T1_HIGHMEM
/**
  * @brief Flag indicating the fast implementation for BRAINPOOL-T P-192 ECC curves
  * @note  Value can be
  *        - @ref CMOX_ECC_BPP192T1_LOWMEM
  *        - @ref CMOX_ECC_BPP192T1_HIGHMEM
  */
#define CMOX_ECC_CURVE_BPP192T1      CMOX_ECC_BPP192T1_HIGHMEM
/**
  * @brief Flag indicating the fast implementation for BRAINPOOL-T P-224 ECC curves
  * @note  Value can be
  *        - @ref CMOX_ECC_BPP224T1_LOWMEM
  *        - @ref CMOX_ECC_BPP224T1_HIGHMEM
  */
#define CMOX_ECC_CURVE_BPP224T1      CMOX_ECC_BPP224T1_HIGHMEM
/**
  * @brief Flag indicating the fast implementation for BRAINPOOL-T P-256 ECC curves
  * @note  Value can be
  *        - @ref CMOX_ECC_BPP256T1_LOWMEM
  *        - @ref CMOX_ECC_BPP256T1_HIGHMEM
  */
#define CMOX_ECC_CURVE_BPP256T1      CMOX_ECC_BPP256T1_HIGHMEM
/**
  * @brief Flag indicating the fast implementation for BRAINPOOL-T P-320 ECC curves
  * @note  Value can be
  *        - @ref CMOX_ECC_BPP320T1_LOWMEM
  *        - @ref CMOX_ECC_BPP320T1_HIGHMEM
  */
#define CMOX_ECC_CURVE_BPP320T1      CMOX_ECC_BPP320T1_HIGHMEM
/**
  * @brief Flag indicating the fast implementation for BRAINPOOL-T P-384 ECC curves
  * @note  Value can be
  *        - @ref CMOX_ECC_BPP384T1_LOWMEM
  *        - @ref CMOX_ECC_BPP384T1_HIGHMEM
  */
#define CMOX_ECC_CURVE_BPP384T1      CMOX_ECC_BPP384T1_HIGHMEM
/**
  * @brief Flag indicating the fast implementation for BRAINPOOL-T P-512 ECC curves
  * @note  Value can be
  *        - @ref CMOX_ECC_BPP512T1_LOWMEM
  *        - @ref CMOX_ECC_BPP512T1_HIGHMEM
  */
#define CMOX_ECC_CURVE_BPP512T1      CMOX_ECC_BPP512T1_HIGHMEM

/**
  * @brief Flag indicating the fast implementation for ANSSI P-256 ECC curves
  * @note  Value can be
  *        - @ref CMOX_ECC_FRP256V1_LOWMEM
  *        - @ref CMOX_ECC_FRP256V1_HIGHMEM
  */
#define CMOX_ECC_CURVE_FRP256V1      CMOX_ECC_FRP256V1_HIGHMEM

/**
  * @brief Flag indicating the fast implementation for OSCCA 256 bit ECC curves
  * @note  Value can be
  *        - @ref CMOX_ECC_SM2_LOWMEM
  *        - @ref CMOX_ECC_SM2_HIGHMEM
  */
#define CMOX_ECC_CURVE_SM2           CMOX_ECC_SM2_HIGHMEM

/**
  * @brief Flag indicating the fast implementation for OSCCA 256 bit test ECC curves
  * @note  Value can be
  *        - @ref CMOX_ECC_SM2TEST_LOWMEM
  *        - @ref CMOX_ECC_SM2TEST_HIGHMEM
  */
#define CMOX_ECC_CURVE_SM2TEST       CMOX_ECC_SM2TEST_HIGHMEM

/**
  * @}
  */

/**
  * @defgroup CMOX_RSA_FAST Fast RSA customizations
  * @{
  */

/**
  * @brief Flag indicating the fast private modular exponentiation implementation
  * @note  Value can be
  *        - @ref CMOX_MODEXP_PRIVATE_LOWMEM
  *        - @ref CMOX_MODEXP_PRIVATE_MIDMEM
  *        - @ref CMOX_MODEXP_PRIVATE_HIGHMEM
  */
#define CMOX_MODEXP_PRIVATE         CMOX_MODEXP_PRIVATE_HIGHMEM

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

#endif /* CMOX_FAST_CONFIG_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
