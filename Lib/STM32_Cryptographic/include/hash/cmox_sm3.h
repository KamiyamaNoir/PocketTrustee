/**
  ******************************************************************************
  * @file    cmox_sm3.h
  * @author  MCD Application Team
  * @brief   Header file for the SM3 hash definition and function
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
#ifndef CMOX_SM3_H
#define CMOX_SM3_H

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* Include files -------------------------------------------------------------*/
#include "cmox_hash.h"
#include "cmox_md.h"

/**
  * @addtogroup CMOX_MD
  * @{
  */

/**
  * @defgroup CMOX_SM3 SM3 hash module
  * @{
  */

/* Macros --------------------------------------------------------------------*/

/** @defgroup CMOX_SM3_MACROS SM3 macros
  * @{
  */

#define CMOX_SM3_SIZE 32U /*!< Maximum size in bytes of a SM3 digest */

/**
  * @}
  */

/* Public types --------------------------------------------------------------*/

/** @defgroup CMOX_SM3_PUBLIC_TYPES SM3 module public types
  * @{
  */

/**
  * @brief SM3 handle type definition
  */
typedef cmox_mdSmall_handle_t cmox_sm3_handle_t;

/**
  * @}
  */

/* Public methods prototypes -------------------------------------------------*/

/** @defgroup CMOX_SM3_PUBLIC_METHODS SM3 module public method prototypes
  * @{
  */

/**
  * @brief SM3 constructor
  *
  * @param P_pThis Pointer to the SM3 handle to initialize
  * @return cmox_hash_handle_t* Pointer to a general hash handle. This will be
  *              used by the general purpose hash functions in order to perform
  *              the algorithm
  */
cmox_hash_handle_t *cmox_sm3_construct(cmox_sm3_handle_t *P_pThis);

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


#endif /* CMOX_SM3_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
