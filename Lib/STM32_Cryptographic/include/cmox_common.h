/**
  ******************************************************************************
  * @file    cmox_common.h
  * @author  MCD Application Team
  * @brief   This file provides the types used within the RSA module
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

#ifndef CMOX_COMMON_H
#define CMOX_COMMON_H

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/**
  *  @brief Structure to store information on the static memory
  */
typedef struct
{
  uint8_t *MemBuf;    /*!< Pointer to the pre-allocated memory buffer */
  size_t  MemBufSize; /*!< Total size of the pre-allocated memory buffer */
  size_t  MemBufUsed; /*!< Currently used portion of the buffer */
  size_t  MaxMemUsed; /*!< Max memory used */
} cmox_membuf_handle_st;


/**
  * @brief Mathematical functions customizations for RSA and ECC
  */
typedef const struct cmox_math_funcsStruct_st *cmox_math_funcs_t;

extern const cmox_math_funcs_t CMOX_MATH_FUNCS_SMALL;              /*!< Smaller footprint and slower performance */
extern const cmox_math_funcs_t CMOX_MATH_FUNCS_FAST;               /*!< Bigger footprint and faster performance */
extern const cmox_math_funcs_t CMOX_MATH_FUNCS_SUPERFAST256;       /*!< Bigger footprint and faster performance,
                                                                        speed up those ECC curves whose length is in the
                                                                        range [225, 256] bits, e.g. Curve25519, Secp256,
                                                                        Bpp256, Ed25519, Frp256, SM2. */


/**
  * @brief Modular exponentiation functions customizations for RSA
  */
typedef const struct cmox_modexp_funcStruct_st *cmox_modexp_func_t;

extern const cmox_modexp_func_t CMOX_MODEXP_PUBLIC;           /*!< Suggested Modexp value for the target device public operations */

extern const cmox_modexp_func_t CMOX_MODEXP_PRIVATE_LOWMEM;   /*!< Constant-time (for Private operations) using Low Memory */
extern const cmox_modexp_func_t CMOX_MODEXP_PRIVATE_MIDMEM;   /*!< Constant-time (for Private operations) using Mid Memory */
extern const cmox_modexp_func_t CMOX_MODEXP_PRIVATE_HIGHMEM;  /*!< Constant-time (for Private operations) using High Memory */


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* CMOX_COMMON_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
