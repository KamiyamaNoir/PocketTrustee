/**
  ******************************************************************************
  * @file    cmox_hash.h
  * @author  MCD Application Team
  * @brief   Header file for the Hash module
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
#ifndef CMOX_HASH_H
#define CMOX_HASH_H

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* Include files -------------------------------------------------------------*/
#include <stdint.h>
#include <stddef.h>
#include "cmox_hash_retvals.h"

/** @addtogroup CMOX_HASH
  * @{
  */

/* Macros --------------------------------------------------------------------*/
/* Public types --------------------------------------------------------------*/

/** @defgroup CMOX_HASH_PUBLIC_TYPES Hash module public types
  * @{
  */

/**
  * @brief Hash Virtual Table
  *
  * This type specifies a pointer to the virtual table containing the methods
  * for a particular algorithm (e.g. SHA256 or SM3)
  */
typedef const struct cmox_hash_vtableStruct_st *cmox_hash_vtable_t;

/**
  * @brief Hash algorithm type
  *
  * This type specifies the algorithm to use with the hash module (e.g. SHA256).
  * The type is defined as a pointer to a structure, that
  * contains the functions for the specific algorithm, defined in the library
  * internally
  */
typedef const struct cmox_hash_algoStruct_st *cmox_hash_algo_t;


/**
  * @brief Hash handle structure definition
  */
typedef struct
{
  cmox_hash_vtable_t table; /*!< Hash virtual table */
  size_t tagLen;            /*!< Size in bytes of the digest */
} cmox_hash_handle_t;

/**
  * @}
  */

/* Public constants ----------------------------------------------------------*/

/** @defgroup CMOX_HASH_PUBLIC_CONSTANTS Hash public constants
  * @{
  */

/**
  * @brief Identifier of the SHA1 hash function for single-call function
  *        (Defined internally)
  */
extern const cmox_hash_algo_t CMOX_SHA1_ALGO;

/**
  * @brief Identifier of the SHA224 hash function for single-call function
  *        (Defined internally)
  */
extern const cmox_hash_algo_t CMOX_SHA224_ALGO;

/**
  * @brief Identifier of the SHA256 hash function for single-call function
  *        (Defined internally)
  */
extern const cmox_hash_algo_t CMOX_SHA256_ALGO;

/**
  * @brief Identifier of the SHA384 hash function for single-call function
  *        (Defined internally)
  */
extern const cmox_hash_algo_t CMOX_SHA384_ALGO;

/**
  * @brief Identifier of the SHA512 hash function for single-call function
  *        (Defined internally)
  */
extern const cmox_hash_algo_t CMOX_SHA512_ALGO;

/**
  * @brief Identifier of the SHA512/224 hash function for single-call function
  *        (Defined internally)
  */
extern const cmox_hash_algo_t CMOX_SHA512_224_ALGO;

/**
  * @brief Identifier of the SHA512/256 hash function for single-call function
  *        (Defined internally)
  */
extern const cmox_hash_algo_t CMOX_SHA512_256_ALGO;

/**
  * @brief Identifier of the SHA3-224 hash function for single-call function
  *        (Defined internally)
  */
extern const cmox_hash_algo_t CMOX_SHA3_224_ALGO;

/**
  * @brief Identifier of the SHA3-256 hash function for single-call function
  *        (Defined internally)
  */
extern const cmox_hash_algo_t CMOX_SHA3_256_ALGO;

/**
  * @brief Identifier of the SHA3-384 hash function for single-call function
  *        (Defined internally)
  */
extern const cmox_hash_algo_t CMOX_SHA3_384_ALGO;

/**
  * @brief Identifier of the SHA3-512 hash function for single-call function
  *        (Defined internally)
  */
extern const cmox_hash_algo_t CMOX_SHA3_512_ALGO;

/**
  * @brief Identifier of the SHAKE128 hash function for single-call function
  *        (Defined internally)
  */
extern const cmox_hash_algo_t CMOX_SHAKE128_ALGO;

/**
  * @brief Identifier of the SHAKE256 hash function for single-call function
  *        (Defined internally)
  */
extern const cmox_hash_algo_t CMOX_SHAKE256_ALGO;

/**
  * @brief Identifier of the SM3 hash function for single-call function
  *        (Defined internally)
  */
extern const cmox_hash_algo_t CMOX_SM3_ALGO;

/**
  * @}
  */

/* Public methods prototypes -------------------------------------------------*/

/** @defgroup CMOX_HASH_PUBLIC_METHODS Hash public method prototypes
  * @{
  */

/**
  * @brief Cleanup the hash handler
  *
  * @param P_pThis Hash handler to cleanup
  * @return cmox_hash_retval_t Hash return value
  */
cmox_hash_retval_t cmox_hash_cleanup(cmox_hash_handle_t *P_pThis);

/**
  * @brief Initialize the hash handle based on the selected algorithm
  *
  * @param P_pThis Hash handle to initialize
  * @return cmox_hash_retval_t Hash return value
  * @note The hash handle must be derived from an algorithm-specific handle
  *       using the correct construct
  */
cmox_hash_retval_t cmox_hash_init(cmox_hash_handle_t *P_pThis);

/**
  * @brief Set the size of the digest
  *
  * @param P_pThis Hash handle to set
  * @param P_tagLen Size in bytes of the tag
  * @return cmox_hash_retval_t Hash return value
  */
cmox_hash_retval_t cmox_hash_setTagLen(cmox_hash_handle_t *P_pThis,
                                       size_t P_tagLen);

/**
  * @brief Append part or the totality of the plaintext to the hash handle
  *
  * @param P_pThis Hash handle to use for hashing the data
  * @param P_pInput Buffer of bytes containing the data to hash
  * @param P_inputLen Size in bytes of the data to hash
  * @return cmox_hash_retval_t Hash return value
  */
cmox_hash_retval_t cmox_hash_append(cmox_hash_handle_t *P_pThis,
                                    const uint8_t *P_pInput,
                                    size_t P_inputLen);

/**
  * @brief Generate the digest of the already appended data
  *
  * @param P_pThis Hash handle used for appending the data to hash
  * @param P_pDigest Buffer of bytes where the digest will be stored
  * @param P_pDigestLen Number of bytes generated by the function.
  *        It is an optional parameter and can be set to NULL if not needed.
  * @return cmox_hash_retval_t Hash return value
  */
cmox_hash_retval_t cmox_hash_generateTag(cmox_hash_handle_t *P_pThis,
                                         uint8_t *P_pDigest,
                                         size_t *P_pDigestLen);

/**
  * @brief Compute the digest of a message using a hash algorithm
  *
  * @param P_algo Identifier of the hash algorithm to use for the computation.
  *               This parameter can be one of the following:
  *               @arg CMOX_SHA1_ALGO
  *               @arg CMOX_SHA224_ALGO
  *               @arg CMOX_SHA256_ALGO
  *               @arg CMOX_SHA384_ALGO
  *               @arg CMOX_SHA512_ALGO
  *               @arg CMOX_SHA512_224_ALGO
  *               @arg CMOX_SHA512_256_ALGO
  *               @arg CMOX_SHA3_224_ALGO
  *               @arg CMOX_SHA3_256_ALGO
  *               @arg CMOX_SHA3_384_ALGO
  *               @arg CMOX_SHA3_512_ALGO
  *               @arg CMOX_SHAKE128_ALGO
  *               @arg CMOX_SHAKE256_ALGO
  *               @arg CMOX_SM3_ALGO
  * @param P_pPlaintext Buffer of bytes containing the message to hash
  * @param P_plaintextLen Size in bytes of the message to hash
  * @param P_pDigest Buffer of bytes that will contain the computed digest
  * @param P_expectedDigestLen Desired size in bytes of the digest to compute
  * @param P_pComputedDigestLen Number of bytes generated by the function.
  *        It is an optional parameter and can be set to NULL if not needed.
  * @return cmox_hash_retval_t
  */
cmox_hash_retval_t cmox_hash_compute(cmox_hash_algo_t P_algo,
                                     const uint8_t *P_pPlaintext,
                                     size_t P_plaintextLen,
                                     uint8_t *P_pDigest,
                                     const size_t P_expectedDigestLen,
                                     size_t *P_pComputedDigestLen);

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CMOX_HASH_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
