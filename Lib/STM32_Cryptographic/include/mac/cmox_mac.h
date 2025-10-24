/**
  ******************************************************************************
  * @file    cmox_mac.h
  * @author  MCD Application Team
  * @brief   Header file for the MAC module
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
#ifndef CMOX_MAC_H
#define CMOX_MAC_H

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* Include files -------------------------------------------------------------*/
#include <stdint.h>
#include <stddef.h>
#include "cmox_mac_retvals.h"

/** @addtogroup CMOX_MAC
  * @{
  */

/* Macros --------------------------------------------------------------------*/
/* Public types --------------------------------------------------------------*/

/** @defgroup CMOX_MAC_PUBLIC_TYPES MAC module public types
  * @{
  */

/**
  * @brief MAC Virtual Table
  *
  * This type specifies a pointer to the virtual table containing the methods
  * for a particular algorithm (e.g. HMAC-256 or CMAC)
  */
typedef const struct cmox_mac_vtableStruct_st *cmox_mac_vtable_t;

/**
  * @brief MAC algorithm type
  *
  * This type specifies the algorithm to use with the MAC module
  * (e.g. HMAC-SHA256). The type is defined as a pointer to a structure, that
  * contains the functions for the specific algorithm, defined in the library
  * internally
  */
typedef const struct cmox_mac_algoStruct_st *cmox_mac_algo_t;


/**
  * @brief MAC handle structure definition
  */
typedef struct
{
  cmox_mac_vtable_t table;  /*!< MAC virtual table */
  size_t tagLen;            /*!< Size in bytes of the authenticated tag */
  uint32_t internalState;   /*!< MAC internal state */
} cmox_mac_handle_t;

/**
  * @}
  */

/* Public methods prototypes -------------------------------------------------*/

/** @defgroup CMOX_MAC_PUBLIC_METHODS MAC public method prototypes
  * @{
  */

/**
  * @brief Cleanup the MAC handler
  *
  * @param P_pThis MAC handler to cleanup
  * @return cmox_mac_retval_t MAC return value
  */
cmox_mac_retval_t cmox_mac_cleanup(cmox_mac_handle_t *P_pThis);

/**
  * @brief Initialize the MAC handle based on the selected algorithm
  *
  * @param P_pThis MAC handle to initialize
  * @return cmox_mac_retval_t Hash return value
  * @note The MAC handle must be derived from an algorithm-specific handle
  *       using the correct construct
  */
cmox_mac_retval_t cmox_mac_init(cmox_mac_handle_t *P_pThis);

/**
  * @brief Set the size of the tag
  *
  * @param P_pThis MAC handle to set
  * @param P_tagLen Size in bytes of the tag
  * @return cmox_mac_retval_t MAC return value
  * @note This function is optional when used for HMAC or CMAC algorithms. If not
  *       called, the tag length will be set as the default specified by the
  *       algorithm (e.g. 16 bytes for CMAC) during the initialization phase.
  * @note With KMAC algorithm, this function is mandatory and must be called
  *       before the cmox_mac_setKey function
  */
cmox_mac_retval_t cmox_mac_setTagLen(cmox_mac_handle_t *P_pThis,
                                     size_t P_tagLen);

/**
  * @brief Set the custom data to be combined with the plaintext for the MAC
  *        computation
  *
  * @param P_pThis MAC handle to use for computing the authenticated tag
  * @param P_pCustomData Buffer of bytes containing the custom data
  * @param P_customDataLen Size in bytes of the custom data
  * @return cmox_mac_retval_t MAC return value
  * @note This function is useful only for KMAC algorithm. It can be called for
  *       other algorithms but will not produce any results
  * @note If the KMAC to compute doesn't need any custom data, this function can
  *       be skipped.
  */
cmox_mac_retval_t cmox_mac_setCustomData(cmox_mac_handle_t *P_pThis,
                                         const uint8_t *P_pCustomData,
                                         size_t P_customDataLen);

/**
  * @brief Set the key to be used for computing the authenticated tag
  *
  * @param P_pThis MAC handle to use for computing the authenticated tag
  * @param P_pKey Buffer of bytes containing the key
  * @param P_keyLen Size in bytes of the key
  * @return cmox_mac_retval_t MAC return value
  * @note The KMAC algorithm is specified to be used even without key. However,
  *       this function is mandatory to be called with the parameter P_keyLen
  *       set to 0.
  */
cmox_mac_retval_t cmox_mac_setKey(cmox_mac_handle_t *P_pThis,
                                  const uint8_t *P_pKey,
                                  size_t P_keyLen);

/**
  * @brief Append part or the totality of the plaintext to the MAC handle
  *
  * @param P_pThis MAC handle to use for computing the authenticated tag
  * @param P_pInput Buffer of bytes containing the data to append
  * @param P_inputLen Size in bytes of the data to append
  * @return cmox_hash_retval_t MAC return value
  */
cmox_mac_retval_t cmox_mac_append(cmox_mac_handle_t *P_pThis,
                                  const uint8_t *P_pInput,
                                  size_t P_inputLen);

/**
  * @brief Compute the authenticated tag of the already appended data
  *
  * @param P_pThis  MAC handle where the data has been appended
  * @param P_pTag Buffer of bytes where the authenticated tag will be stored
  * @param P_pTagLen Number of bytes generated by the function.
  *        It is an optional parameter and can be set to NULL if not needed.
  * @return cmox_mac_retval_t MAC return value
  */
cmox_mac_retval_t cmox_mac_generateTag(cmox_mac_handle_t *P_pThis,
                                       uint8_t *P_pTag,
                                       size_t *P_pTagLen);

/**
  * @brief Verify the already appended data with the reference tag
  *
  * @param P_pThis MAC handle where the data has been appended
  * @param P_pTag Buffer of bytes containing the reference tag
  * @param P_pFaultCheck Optional value to check, together with the retval,
  *                      to verify if some fault happened
  * @return cmox_mac_retval_t
  * @note P_pFaultCheck value, if the parameter is provided, MUST be checked to
  *       be equal to the retval, and both MUST be equal to the successful value.
  *       P_pFaultCheck MUST be checked only if the main result is successful,
  *       and has no relevance if the main result is not successful.
  *       Every comparison (both for the return value and for P_pFaultCheck) must
  *       be done against the success value, and not comparing the value with the
  *       failure value. Indeed, in presence of faults, especially P_pFaultCheck,
  *       could be a dirty value.
  */
cmox_mac_retval_t cmox_mac_verifyTag(cmox_mac_handle_t *P_pThis,
                                     const uint8_t *P_pTag,
                                     uint32_t *P_pFaultCheck);

/**
  * @brief Compute the authenticated tag of a message using a MAC algorithm
  *
  * @param P_algo Identifier of the hash algorithm to use for the computation.
  *               This parameter can be one of the following:
  *               @arg CMOX_CMAC_AES_ALGO
  *               @arg CMOX_HMAC_SHA1_ALGO
  *               @arg CMOX_HMAC_SHA224_ALGO
  *               @arg CMOX_HMAC_SHA256_ALGO
  *               @arg CMOX_HMAC_SHA384_ALGO
  *               @arg CMOX_HMAC_SHA512_ALGO
  *               @arg CMOX_HMAC_SHA512_224_ALGO
  *               @arg CMOX_HMAC_SHA512_256_ALGO
  *               @arg CMOX_HMAC_SM3_ALGO
  *               @arg CMOX_KMAC_128_ALGO
  *               @arg CMOX_KMAC_256_ALGO
  * @param P_pInput Buffer of bytes containing the message to process
  * @param P_inputLen Size in bytes of the message to process
  * @param P_pKey Buffer of bytes containing the key
  * @param P_keyLen Size in bytes of the key
  * @param P_pCustomData Buffer of bytes containing the custom data
  * @param P_customDataLen Size in bytes of the custom data
  * @param P_pTag Buffer of bytes where the authenticated tag will be stored
  * @param P_expectedTagLen Size in bytes of the tag to compute
  * @param P_pComputedTagLen Number of bytes generated by the function.
  *        It is an optional parameter and can be set to NULL if not needed.
  * @return cmox_mac_retval_t
  */
cmox_mac_retval_t cmox_mac_compute(cmox_mac_algo_t P_algo,
                                   const uint8_t *P_pInput,
                                   size_t P_inputLen,
                                   const uint8_t *P_pKey,
                                   size_t P_keyLen,
                                   const uint8_t *P_pCustomData,
                                   size_t P_customDataLen,
                                   uint8_t *P_pTag,
                                   size_t P_expectedTagLen,
                                   size_t *P_pComputedTagLen);

/**
  * @brief Verify the authenticity of a message using a MAC algorithm
  *
  * @param P_algo Identifier of the hash algorithm to use for the computation.
  *               This parameter can be one of the following:
  *               @arg CMOX_CMAC_AES_ALGO
  *               @arg CMOX_HMAC_SHA1_ALGO
  *               @arg CMOX_HMAC_SHA224_ALGO
  *               @arg CMOX_HMAC_SHA256_ALGO
  *               @arg CMOX_HMAC_SHA384_ALGO
  *               @arg CMOX_HMAC_SHA512_ALGO
  *               @arg CMOX_HMAC_SHA512_224_ALGO
  *               @arg CMOX_HMAC_SHA512_256_ALGO
  *               @arg CMOX_HMAC_SM3_ALGO
  *               @arg CMOX_KMAC_128_ALGO
  *               @arg CMOX_KMAC_256_ALGO
  * @param P_pInput Buffer of bytes containing the message to process
  * @param P_inputLen Size in bytes of the message to process
  * @param P_pKey Buffer of bytes containing the key
  * @param P_keyLen Size in bytes of the key
  * @param P_pCustomData Buffer of bytes containing the custom data
  * @param P_customDataLen Size in bytes of the custom data
  * @param P_pReceivedTag Buffer of bytes containing the received tag
  * @param P_receivedTagLen Size in bytes of the received tag
  * @return cmox_mac_retval_t
  */
cmox_mac_retval_t cmox_mac_verify(cmox_mac_algo_t P_algo,
                                  const uint8_t *P_pInput,
                                  size_t P_inputLen,
                                  const uint8_t *P_pKey,
                                  size_t P_keyLen,
                                  const uint8_t *P_pCustomData,
                                  size_t P_customDataLen,
                                  const uint8_t *P_pReceivedTag,
                                  size_t P_receivedTagLen);

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CMOX_MAC_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
