/**
  ******************************************************************************
  * @file    cmox_ecc_types.h
  * @author  MCD Application Team
  * @brief   This file provides the types used within the ECC module
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

#ifndef CMOX_ECC_TYPES_H
#define CMOX_ECC_TYPES_H

#include "cmox_common.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/**
  * @brief Structure for the ECC context
  */
typedef struct
{
  cmox_membuf_handle_st membuf_str;       /*!< Memory buffer structure */
  cmox_math_funcs_t     math_ptr;         /*!< Math customization structure */
  uint32_t              magic_num_check;  /*!< Magic number for diagnostic checks */
} cmox_ecc_handle_t;

/**
  * @brief Pointer type for the ECC curve function parameter
  */
typedef const struct cmox_ecc_implStruct_st *cmox_ecc_impl_t;

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* CMOX_ECC_TYPES_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
