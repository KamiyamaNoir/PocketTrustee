/**
  ******************************************************************************
  * @file    cmox_rsa_retvals.h
  * @author  MCD Application Team
  * @brief   This file provides the error types and code for RSA modules
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

#ifndef CMOX_RSA_RETVALS_H
#define CMOX_RSA_RETVALS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/**
  * @addtogroup CMOX_RSA
  * @{
  */

/**
  * @defgroup CMOX_RSA_RETVALS RSA return values
  * @{
  */

/**
  * @brief Return value type for RSA module
  */
typedef uint32_t cmox_rsa_retval_t;

#define CMOX_RSA_SUCCESS                  ((cmox_rsa_retval_t)0x00050000) /*!< RSA operation successfully performed */
#define CMOX_RSA_ERR_INTERNAL             ((cmox_rsa_retval_t)0x00050001) /*!< Internal computat. error (e.g. hash) */
#define CMOX_RSA_ERR_BAD_PARAMETER        ((cmox_rsa_retval_t)0x00050003) /*!< One of the expected parameter is invalid */
#define CMOX_RSA_ERR_MODULUS_TOO_SHORT    ((cmox_rsa_retval_t)0x00050007) /*!< Input too long for the current modulus */
#define CMOX_RSA_ERR_INVALID_SIGNATURE    ((cmox_rsa_retval_t)0x00050009) /*!< RSA invalid signature value */
#define CMOX_RSA_ERR_WRONG_DECRYPTION     ((cmox_rsa_retval_t)0x0005000A) /*!< RSA invalid decryption, due to mismatch between private key and input */
#define CMOX_RSA_ERR_WRONG_RANDOM         ((cmox_rsa_retval_t)0x0005000B) /*!< Random not compliant with the API (Recall with other random material) */
#define CMOX_RSA_ERR_MEMORY_FAIL          ((cmox_rsa_retval_t)0x0005000C) /*!< Not enough memory */
#define CMOX_RSA_ERR_MATH_ALGO_MISMATCH   ((cmox_rsa_retval_t)0x00050010) /*!< Math customization not supported by current functionality */
#define CMOX_RSA_ERR_MEXP_ALGO_MISMATCH   ((cmox_rsa_retval_t)0x00050011) /*!< Modexp function not supported by current functionality */
#define CMOX_RSA_AUTH_SUCCESS             ((cmox_rsa_retval_t)0x0005C726) /*!< RSA signature successful verification */
#define CMOX_RSA_AUTH_FAIL                ((cmox_rsa_retval_t)0x00056E93) /*!< RSA signature not verified */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* CMOX_RSA_RETVALS_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
