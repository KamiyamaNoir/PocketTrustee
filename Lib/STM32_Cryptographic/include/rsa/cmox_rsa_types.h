/**
  ******************************************************************************
  * @file    cmox_rsa_types.h
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

#ifndef CMOX_RSA_TYPES_H
#define CMOX_RSA_TYPES_H

#include <stdint.h>
#include <stdlib.h>
#include "cmox_common.h"
#include "rsa/cmox_rsa_retvals.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */


/**
  * @brief Structure for the RSA context
  */
typedef struct
{
  cmox_membuf_handle_st membuf_str;       /*!< Memory buffer structure */
  cmox_modexp_func_t    modexp_ptr;       /*!< Modexp customization structure */
  cmox_math_funcs_t     math_ptr;         /*!< Math customization structure */
  uint32_t              magic_num_check;  /*!< Magic number for diagnostic checks */
} cmox_rsa_handle_t;

/**
  * @brief Internal structure type for the RSA modular exponentiation function
  */
struct cmox_rsa_intfuncStruct_t;
/**
  * @brief Public pointer type for the RSA modular exponentiation function
  */
typedef struct cmox_rsa_intfuncStruct_t *cmox_rsa_intfunc_t;

/**
  * @brief Internal structure type for the Fault Attacks countermeasure
  */
struct cmox_rsa_facmStruct_t;
/**
  * @brief Public pointer type for the Fault Attacks countermeasure
  */
typedef const struct cmox_rsa_facmStruct_t *cmox_rsa_facm_t;

/**
  * @brief Structure to hold public or private key parameters
  */
typedef struct
{
  cmox_rsa_intfunc_t  f;          /*!< Function executing CRT/standard modexp */
  size_t              mod_bitlen; /*!< Length (in bytes) of Modulus */

  union
  {
    /* Standard public/private fields */
    struct
    {
      const uint8_t   *mod;       /*!< Modulus  */
      const uint8_t   *exp;       /*!< Public/secret Exponent */
      size_t          exp_len;    /*!< Length (in bytes) of Exponent */
    } std;

    /* CRT fields */
    struct
    {
      const uint8_t   *p;         /*!< Parameter P (in case of CRT) or normal modulus  */
      size_t          p_len;      /*!< Length (in bytes) of P/Modulus */
      const uint8_t   *q;         /*!< Parameter Q */
      size_t          q_len;      /*!< Length (in bytes) of Q */
      const uint8_t   *dp;        /*!< Secret exponent (mod P) (in case of CRT), or normal secret exponent */
      size_t          dp_len;     /*!< Length (in bytes) of dP/Exponent */
      const uint8_t   *dq;        /*!< Secret exponent (mod Q) */
      size_t          dq_len;     /*!< Length (in bytes) of dQ */
      const uint8_t   *iq;        /*!< Q^(-1) (mod P) */
      size_t          iq_len;     /*!< Length (in bytes) of iQ */
      const uint8_t   *pub_exp;   /*!< public exponent */
      size_t          pub_exp_len;/*!< Length (in bytes) of the public exponent */
      cmox_rsa_facm_t facm_flag;  /*!< Parameter to enable/disable the countermeasure to the Fault Attacks */
    } crt;
  } fields;                       /*!< Union for STD or CRT key structure */
} cmox_rsa_key_t;

/**
  * @brief Pointer type linked to the structure to hold Hash information for PKCS#1 v1.5
  */
typedef const struct cmox_rsa_pkcs1v15_hash_st *cmox_rsa_pkcs1v15_hash_t;

/**
  * @brief Pointer type linked to the structure to hold Hash information for PKCS#1 v2.2
  */
typedef const struct cmox_rsa_pkcs1v22_hash_st *cmox_rsa_pkcs1v22_hash_t;

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* CMOX_RSA_TYPES_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
