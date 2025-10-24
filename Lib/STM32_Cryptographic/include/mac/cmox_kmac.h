/**
  ******************************************************************************
  * @file    cmox_kmac.h
  * @author  MCD Application Team
  * @brief   Header file for the KMAC algorithm definitions and functions
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
#ifndef CMOX_KMAC_H
#define CMOX_KMAC_H

/* Include files -------------------------------------------------------------*/
#include "cmox_mac.h"
#include "hash/cmox_sponge.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/** @addtogroup CMOX_MAC
  * @{
  */

/** @defgroup CMOX_KMAC KMAC
  * @{
  */

/* Public types --------------------------------------------------------------*/

/** @defgroup CMOX_KMAC_PUBLIC_TYPES KMAC public types
  * @{
  */

/**
  * @brief KMAC implementation
  *
  * This type specifies the implementation to use for the KMAC construct.
  * This type is defined as a pointer to a structure, that contains the
  * functions needed for the specific implementation, defined in the library
  * internally
  */
typedef const struct cmox_kmac_implStruct_st *cmox_kmac_impl_t;

/**
  * @brief Keccak-P phases
  */
typedef uint32_t cmox_keccak_phase_t;

/**
  * @brief Structure defining the cSHAKE context
  */
typedef struct
{
  cmox_sponge_handle_t sponge;  /*!< Sponge context */
  uint32_t fixedOutputLength;   /*!< Fixed output length in bits */
  uint32_t lastByteBitLen;      /*!< Last byte bit length */
  uint8_t lastByteValue;        /*!< Last byte value */
  cmox_keccak_phase_t phase;    /*!< Phase */
} cmox_cshake_handle_t;

/**
  * @brief Structure defining the KMAC inner context
  */
typedef struct
{
  cmox_cshake_handle_t csi;     /*!< cSHAKE context */
  size_t outputBitLen;          /*!< Output length in bits */
} cmox_kmac_inner_t;


/**
  * @brief KMAC handle structure definition
  */
typedef struct
{
  cmox_mac_handle_t super;        /*!< General MAC handle */
  const uint8_t *custom_data;     /*!< Pointer to buffer containing custom data */
  size_t customDataLen;           /*!< Size in bytes of the custom data */
  cmox_kmac_inner_t internal_ctx; /*!< Internal handle for KMAC */
  cmox_kmac_impl_t impl;          /*!< Implementation of KMAC */
} cmox_kmac_handle_t;

/**
  * @}
  */

/* Public constants ----------------------------------------------------------*/

/** @defgroup CMOX_KMAC_PUBLIC_CONSTANTS KMAC public constants
  * @{
  */

/** @defgroup CMOX_KMAC_IMPL KMAC implementations
  * @{
  */

/**
  * @brief Implementation of KMAC-128 algorithm (Defined internally)
  */
extern const cmox_kmac_impl_t CMOX_KMAC_128;

/**
  * @brief Implementation of KMAC-256 algorithm (Defined internally)
  */
extern const cmox_kmac_impl_t CMOX_KMAC_256;

/**
  * @}
  */

/** @defgroup CMOX_KMAC_ALGO KMAC single-call algorithms
  * @{
  */

/**
  * @brief Identifier of the KMAC-128 mac algorithm for single-call function
  *        (Defined internally)
  */
extern const cmox_mac_algo_t CMOX_KMAC_128_ALGO;

/**
  * @brief Identifier of the KMAC-256 mac algorithm for single-call function
  *        (Defined internally)
  */
extern const cmox_mac_algo_t CMOX_KMAC_256_ALGO;

/**
  * @}
  */

/**
  * @}
  */

/* Public methods prototypes -------------------------------------------------*/

/** @defgroup CMOX_KMAC_PUBLIC_METHODS KMAC public method prototypes
  * @{
  */

/**
  * @brief KMAC constructor
  *
  * The function is used for specifying which KMAC implementation to use in
  * order to implement the MAC algorithm.
  *
  * @param P_pThis Pointer to the KMAC handle to initialize
  * @param P_impl  Constant that specifies the implementation to use.
  *                This parameter can be one of the following values:
  *                @arg CMOX_KMAC_128
  *                @arg CMOX_KMAC_256
  * @return cmox_mac_handle_t* Pointer to a general MAC handle. This will
  *                be used by the general purpose MAC functions in order to
  *                perform the algorithm
  */
cmox_mac_handle_t *cmox_kmac_construct(cmox_kmac_handle_t *P_pThis,
                                       cmox_kmac_impl_t P_impl);

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

#endif /* CMOX_KMAC_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
