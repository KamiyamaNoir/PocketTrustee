/**
  ******************************************************************************
  * @file    cmox_gcm.h
  * @author  MCD Application Team
  * @brief   Header file for the GCM AEAD cipher definitions and functions
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
#ifndef CMOX_GCM_H
#define CMOX_GCM_H

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* Include files -------------------------------------------------------------*/
#include "cmox_cipher.h"
#include "cmox_blockcipher.h"

#include "cmox_check_default_gcm.h"
#include "cmox_check_default_aes.h"

#include "cmox_default_defs.h"

/** @addtogroup CMOX_CIPHER
  * @{
  */

/** @defgroup CMOX_GCM GCM cipher
  * @{
  */

/* Public macros -------------------------------------------------------------*/
/* Public types --------------------------------------------------------------*/

/** @defgroup CMOX_GCM_PUBLIC_TYPES GCM public types
  * @{
  */

/**
  * @brief Type for specifying the GFMUL operation to use (internally used)
  */
typedef const struct cmox_gcm_gfmulTable_st *cmox_gcm_gfmul_t;

/**
  * @brief GCM SMALL mode implementation
  *
  * This type specifies the used block cipher for the GCM construct and if the
  * algorithm will be used for encryption or decryption.
  * This type is defined as a pointer to a structure, that contains the
  * functions needed for the specific implementation, defined in the library
  * internally
  */
typedef const struct cmox_gcmSmall_implStruct_st *cmox_gcmSmall_impl_t;


/**
  * @brief GCM FAST mode implementation
  *
  * This type specifies the used block cipher for the GCM construct and if the
  * algorithm will be used for encryption or decryption.
  * This type is defined as a pointer to a structure, that contains the
  * functions needed for the specific implementation, defined in the library
  * internally
  */
typedef const struct cmox_gcmFast_implStruct_st *cmox_gcmFast_impl_t;

/**
  * @brief Polynomial type used internally by GCM
  */
typedef uint32_t cmox_gcm_poly_t[4];

/**
  * @brief Polynomial table type used by GCM SMALL implementation
  */
typedef cmox_gcm_poly_t cmox_gcm_table16_t[16];

/**
  * @brief Polynomial table type used by GCM FAST implementation
  */
typedef cmox_gcm_poly_t cmox_gcm_table8x16_t[8][16];


/**
  * @brief Common fields for both GCM FAST and GCM SMALL handles
  */
typedef struct
{
  cmox_blockcipher_handle_t blockCipher; /*!< Block cipher handle */
  uint32_t iv[CMOX_CIPHER_BLOCK_SIZE];  /*!< Buffer containing the IV */
  size_t tagLen;  /*!< Size in bytes of the aithentication tag */
  size_t AdLen;   /*!< Size in bytes of the Additional Data */
  size_t payloadLen;   /*!< Processed payload size in bytes */
  cmox_gcm_poly_t partialAuth; /*!< Partial authentication value */
  cmox_gcm_gfmul_t gfmul;  /*!< GF mul implementation */
} cmox_gcm_common_t;

/**
  * @brief GCM SMALL handle structure definition
  */
typedef struct
{
  cmox_cipher_handle_t super;           /*!< General cipher handle */
  cmox_gcm_common_t common;             /*!< Common values with the GCM FAST handle type */
  cmox_gcm_table16_t precomputedValues; /*!< Precomputation of polynomial according to Shoup's 4-bit table */
} cmox_gcmSmall_handle_t;

/**
  * @brief GCM fast context
  */
typedef struct
{
  cmox_cipher_handle_t super;             /*!< General cipher handle */
  cmox_gcm_common_t common;               /*!< Common values with the GCM SMALL handle type */
  cmox_gcm_table8x16_t precomputedValues; /*!< Precomputation of polynomial according to Shoup's 8-bit table */
} cmox_gcmFast_handle_t;

#if (CMOX_GCM_IMPLEMENTATION == CMOX_GCM_FAST)

/**
  * @brief Default GCM handle definition
  */
typedef cmox_gcmFast_handle_t cmox_gcm_handle_t;

/**
  * @brief Default GCM implementation definition
  */
typedef cmox_gcmFast_impl_t cmox_gcm_impl_t;

#elif (CMOX_GCM_IMPLEMENTATION == CMOX_GCM_SMALL)
/**
  * @brief Default GCM handle definition
  */
typedef cmox_gcmSmall_handle_t cmox_gcm_handle_t;

/**
  * @brief Default GCM implementation definition
  */
typedef cmox_gcmSmall_impl_t cmox_gcm_impl_t;

#endif /* CMOX_GCM_IMPLEMENTATION == CMOX_GCM_FAST */

/**
  * @}
  */

/* Public constants ----------------------------------------------------------*/

/** @defgroup CMOX_GCM_PUBLIC_CONSTANTS GCM public constants
  * @{
  */

/** @defgroup CMOX_GCM_IMPL GCM implementations
  * @{
  */

/**
  * @brief Implementation of GCMSMALL encryption using AES (fast implementation)
  *        (Defined internally)
  */
extern const cmox_gcmSmall_impl_t CMOX_AESFAST_GCMSMALL_ENC;

/**
  * @brief Implementation of GCMSMALL decryption using AES (fast implementation)
  *        (Defined internally)
  */
extern const cmox_gcmSmall_impl_t CMOX_AESFAST_GCMSMALL_DEC;

/**
  * @brief Implementation of GCMSMALL encryption using AES (small implementation)
  *        (Defined internally)
  */
extern const cmox_gcmSmall_impl_t CMOX_AESSMALL_GCMSMALL_ENC;

/**
  * @brief Implementation of GCMSMALL decryption using AES (small implementation)
  *        (Defined internally)
  */
extern const cmox_gcmSmall_impl_t CMOX_AESSMALL_GCMSMALL_DEC;

/**
  * @brief Implementation of GCMFAST encryption using AES (fast implementation)
  *        (Defined internally)
  */
extern const cmox_gcmFast_impl_t CMOX_AESFAST_GCMFAST_ENC;

/**
  * @brief Implementation of GCMFAST decryption using AES (fast implementation)
  *        (Defined internally)
  */
extern const cmox_gcmFast_impl_t CMOX_AESFAST_GCMFAST_DEC;

/**
  * @brief Implementation of GCMFAST encryption using AES (small implementation)
  *        (Defined internally)
  */
extern const cmox_gcmFast_impl_t CMOX_AESSMALL_GCMFAST_ENC;

/**
  * @brief Implementation of GCMFAST decryption using AES (small implementation)
  *        (Defined internally)
  */
extern const cmox_gcmFast_impl_t CMOX_AESSMALL_GCMFAST_DEC;

/**
  * @}
  */

/** @defgroup CMOX_GCM_ALGO GCM single-call algorithms
  * @{
  */

/**
  * @brief Identifier of the GCM (fast implementation) encryption using AES
  *        (small implementation) for single-call function (Defined internally)
  */
extern const cmox_aead_algo_t CMOX_AESSMALL_GCMFAST_ENC_ALGO;

/**
  * @brief Identifier of the GCM (fast implementation) decryption using AES
  *        (small implementation) for single-call function (Defined internally)
  */
extern const cmox_aead_algo_t CMOX_AESSMALL_GCMFAST_DEC_ALGO;

/**
  * @brief Identifier of the GCM (fast implementation) encryption using AES
  *        (fast implementation) for single-call function (Defined internally)
  */
extern const cmox_aead_algo_t CMOX_AESFAST_GCMFAST_ENC_ALGO;

/**
  * @brief Identifier of the GCM (fast implementation) decryption using AES
  *        (fast implementation) for single-call function (Defined internally)
  */
extern const cmox_aead_algo_t CMOX_AESFAST_GCMFAST_DEC_ALGO;

/**
  * @brief Identifier of the GCM (small implementation) encryption using AES
  *        (small implementation) for single-call function (Defined internally)
  */
extern const cmox_aead_algo_t CMOX_AESSMALL_GCMSMALL_ENC_ALGO;

/**
  * @brief Identifier of the GCM (small implementation) decryption using AES
  *        (small implementation) for single-call function (Defined internally)
  */
extern const cmox_aead_algo_t CMOX_AESSMALL_GCMSMALL_DEC_ALGO;

/**
  * @brief Identifier of the GCM (small implementation) encryption using AES
  *        (fast implementation) for single-call function (Defined internally)
  */
extern const cmox_aead_algo_t CMOX_AESFAST_GCMSMALL_ENC_ALGO;

/**
  * @brief Identifier of the GCM (small implementation) decryption using AES
  *        (fast implementation) for single-call function (Defined internally)
  */
extern const cmox_aead_algo_t CMOX_AESFAST_GCMSMALL_DEC_ALGO;

/**
  * @}
  */

/**
  * @}
  */

/* Public methods prototypes -------------------------------------------------*/

/** @defgroup CMOX_GCM_PUBLIC_METHODS GCM public method prototypes
  * @{
  */

/**
  * @brief GCMSMALL constructor
  *
  * The function is used for specifying which block cipher algorithm to use in
  * order to implement the GCM algorithm and if the algorithm will be used for
  * encryption or decryption.
  *
  * @param P_pThis Pointer to the GCM handle to initialize
  * @param P_impl  Constant that specifies the implementation to use.
  *                This parameter can be one of the following values:
  *                @arg CMOX_AESFAST_GCMSMALL_ENC
  *                @arg CMOX_AESFAST_GCMSMALL_DEC
  *                @arg CMOX_AESSMALL_GCMSMALL_ENC
  *                @arg CMOX_AESSMALL_GCMSMALL_DEC
  * @return cmox_cipher_handle_t* Pointer to a general cipher handle. This will
  *                be used by the general purpose cipher functions in order to
  *                perform the algorithm
  * @note If CMOX_GCM_IMPLEMENTATION macro has been set to CMOX_GCM_SMALL, this
  *       constructor can be called also through the wrapper cmox_gcm_construct
  */
cmox_cipher_handle_t *cmox_gcmSmall_construct(cmox_gcmSmall_handle_t *P_pThis,
                                              cmox_gcmSmall_impl_t P_impl);

/**
  * @brief GCMFAST constructor
  *
  * The function is used for specifying which block cipher algorithm to use in
  * order to implement the GCM algorithm and if the algorithm will be used for
  * encryption or decryption.
  *
  * @param P_pThis Pointer to the GCM handle to initialize
  * @param P_impl  Constant that specifies the implementation to use.
  *                This parameter can be one of the following values:
  *                @arg CMOX_AESFAST_GCMFAST_ENC
  *                @arg CMOX_AESFAST_GCMFAST_DEC
  *                @arg CMOX_AESSMALL_GCMFAST_ENC
  *                @arg CMOX_AESSMALL_GCMFAST_DEC
  * @return cmox_cipher_handle_t* Pointer to a general cipher handle. This will
  *                be used by the general purpose cipher functions in order to
  *                perform the algorithm
  * @note If CMOX_GCM_IMPLEMENTATION macro has been set to CMOX_GCM_FAST, this
  *       constructor can be called also through the wrapper cmox_gcm_construct
  */
cmox_cipher_handle_t *cmox_gcmFast_construct(cmox_gcmFast_handle_t *P_pThis,
                                             cmox_gcmFast_impl_t P_impl);

#if (CMOX_GCM_IMPLEMENTATION == CMOX_GCM_FAST)

/**
  * @brief GCM default constructor
  *
  * The function is used for specifying which block cipher algorithm to use in
  * order to implement the GCM algorithm (default implementation) and if the
  * algorithm will be used for encryption or decryption.
  *
  * @param P_pThis Pointer to the GCM handle to initialize
  * @param P_impl  Constant that specifies the implementation to use.
  *                This parameter can be one of the following values:
  *                @arg CMOX_AES_GCM_ENC
  *                @arg CMOX_AES_GCM_DEC
  * @return cmox_cipher_handle_t* Pointer to a general cipher handle. This will
  *                be used by the general purpose cipher functions in order to
  *                perform the algorithm
  * @note This function is just a wrapper of the default implementation of the
  *       GCM algorithm. It is possible to select the preferred GCM
  *       implementation from the cmox_default_config.h header file or using a custom
  *       default configuration header file, to be specified by the
  *       CMOX_DEFAULT_FILE macro
  */
static inline
cmox_cipher_handle_t *cmox_gcm_construct(cmox_gcm_handle_t *P_pThis,
                                         cmox_gcm_impl_t P_impl)
{
  return cmox_gcmFast_construct(P_pThis, P_impl);
}

#elif (CMOX_GCM_IMPLEMENTATION == CMOX_GCM_SMALL)

/**
  * @brief GCM default constructor
  *
  * The function is used for specifying which block cipher algorithm to use in
  * order to implement the GCM algorithm (default implementation) and if the
  * algorithm will be used for encryption or decryption.
  *
  * @param P_pThis Pointer to the GCM handle to initialize
  * @param P_impl  Constant that specifies the implementation to use.
  *                This parameter can be one of the following values:
  *                @arg CMOX_AES_GCM_ENC
  *                @arg CMOX_AES_GCM_DEC
  * @return cmox_cipher_handle_t* Pointer to a general cipher handle. This will
  *                be used by the general purpose cipher functions in order to
  *                perform the algorithm
  * @note This function is just a wrapper of the default implementation of the
  *       GCM algorithm. It is possible to select the preferred GCM
  *       implementation from the cmox_default_config.h header file or using a custom
  *       default configuration header file, to be specified by the
  *       CMOX_DEFAULT_FILE macro
  */
static inline
cmox_cipher_handle_t *cmox_gcm_construct(cmox_gcm_handle_t *P_pThis,
                                         cmox_gcm_impl_t P_impl)
{
  return cmox_gcmSmall_construct(P_pThis, P_impl);
}

#endif /* CMOX_GCM_IMPLEMENTATION == CMOX_GCM_FAST */

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

#endif /* CMOX_GCM_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
