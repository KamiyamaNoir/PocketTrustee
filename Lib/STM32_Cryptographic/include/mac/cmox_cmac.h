/**
  ******************************************************************************
  * @file    cmox_cmac.h
  * @author  MCD Application Team
  * @brief   Header file for the CMAC algorithm definitions and functions
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
#ifndef CMOX_CMAC_H
#define CMOX_CMAC_H

/* Include files -------------------------------------------------------------*/
#include "cmox_mac.h"
#include "cipher/cmox_blockcipher.h"

#include "cipher/cmox_check_default_aes.h"
#include "cmox_default_defs.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/** @addtogroup CMOX_MAC
  * @{
  */

/** @defgroup CMOX_CMAC CMAC
  * @{
  */

/* Public macros -------------------------------------------------------------*/
/* Public types --------------------------------------------------------------*/

/** @defgroup CMOX_CMAC_PUBLIC_TYPES CMAC public types
  * @{
  */

/**
  * @brief CMAC implementation
  *
  * This type specifies the used block cipher for the CMAC construct.
  * This type is defined as a pointer to a structure, that contains the
  * functions needed for the specific implementation, defined in the library
  * internally
  */
typedef const struct cmox_cmac_implStruct_st *cmox_cmac_impl_t;

/**
  * @brief CMAC handle structure definition
  */
typedef struct
{
  cmox_mac_handle_t super;                /*!< General MAC handle */
  cmox_blockcipher_handle_t blockCipher;  /*!< Block cipher handle */
  uint32_t iv[CMOX_CIPHER_BLOCK_SIZE];    /*!< Buffer containing the Initialization Vector */
  uint8_t temp_buffer[16];                /*!< Temporary buffer to storing unprocessed data */
  uint32_t unprocessed_bytes;             /*!< Number of bytes in the temporary buffer */
} cmox_cmac_handle_t;

/**
  * @}
  */

/* Public constants ----------------------------------------------------------*/

/** @defgroup CMOX_CMAC_PUBLIC_CONSTANTS CMAC public constants
  * @{
  */

/** @defgroup CMOX_CMAC_IMPL CMAC implementations
  * @{
  */

/**
  * @brief Implementation of CMAC using AES (small implementation)
  *        (Defined internally)
  */
extern const cmox_cmac_impl_t CMOX_CMAC_AESSMALL;

/**
  * @brief Implementation of CMAC using AES (fast implementation)
  *        (Defined internally)
  */
extern const cmox_cmac_impl_t CMOX_CMAC_AESFAST;

/**
  * @}
  */

/** @defgroup CMOX_CMAC_ALGO CMAC single-call algorithms
  * @{
  */

/**
  * @brief Identifier of the CMAC using AES (fast implementation)
  *   for single-call function (Defined internally)
  */
extern const cmox_mac_algo_t CMOX_CMAC_AESFAST_ALGO;

/**
  * @brief Identifier of the CMAC using AES (small implementation)
  *   for single-call function (Defined internally)
  */
extern const cmox_mac_algo_t CMOX_CMAC_AESSMALL_ALGO;

/**
  * @}
  */

/**
  * @}
  */

/* Public methods prototypes -------------------------------------------------*/

/** @defgroup CMOX_CMAC_PUBLIC_METHODS CMAC public method prototypes
  * @{
  */

/**
  * @brief CMAC constructor
  *
  * The function is used for specifying which block cipher algorithm to use in
  * order to implement the CMAC algorithm.
  *
  * @param P_pThis Pointer to the CMAC handle to initialize
  * @param P_impl  Constant that specifies the implementation to use.
  *                This parameter shall be set with the following value:
  *                @arg CMOX_CMAC_AESFAST
  *                @arg CMOX_CMAC_AESSMALL
  * @return cmox_mac_handle_t* Pointer to a general MAC handle. This will
  *                be used by the general purpose MAC functions in order to
  *                perform the algorithm
  */
cmox_mac_handle_t *cmox_cmac_construct(cmox_cmac_handle_t *P_pThis,
                                       cmox_cmac_impl_t P_impl);

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

#endif /* CMOX_CMAC_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
