/**
  ******************************************************************************
  * @file    cmox_ecc_custom_curves.h
  * @author  MCD Application Team
  * @brief   This file provides everything to define custom ECC curves
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

#ifndef CMOX_ECC_CUSTOM_CURVES_H
#define CMOX_ECC_CUSTOM_CURVES_H

#include <stdint.h>
#include "cmox_common.h"
#include "ecc/cmox_ecc_types.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/** @addtogroup CMOX_ECC
  * @{
  */

/**
  * @defgroup CMOX_ECC_CUSTOM_CURVES_OPT speed/RAM optimizations for custom curves implementations
  * @{
  */

struct cmox_ecc_customCurve_optStr_st; /*!< struct for speed/RAM optimizations */
typedef const struct cmox_ecc_customCurve_optStr_st *cmox_ecc_customCurve_opt_t; /*!< struct ptr for speed/RAM opt */

/**
  * @brief Low RAM usage (slowest)
  */
extern const cmox_ecc_customCurve_opt_t cmox_ecc_customCurve_opt_low;
/**
  * @brief Mid RAM usage
  */
extern const cmox_ecc_customCurve_opt_t cmox_ecc_customCurve_opt_mid;
/**
  * @brief High RAM usage (fastest)
  */
extern const cmox_ecc_customCurve_opt_t cmox_ecc_customCurve_opt_high;

/**
  * @}
  */

/**
  * @defgroup CMOX_ECC_CUSTOM_CURVES_METHODS ECC public method prototypes to define custom curves
  * @{
  */

/**
  * @brief      Function to create a custom Short-Weierstrass curve
  * @param[in]  P_pEccCtx   Context for ECC operations
  * @param[out] P_pCurve    Pointer to a curve that will be built and filled
  * @param[in]  P_OptLevel  Optimization level (speed vs. RAM usage)
  *                         This parameter can be one of the following:
  *                            @arg cmox_ecc_customCurve_opt_low
  *                            @arg cmox_ecc_customCurve_opt_mid
  *                            @arg cmox_ecc_customCurve_opt_high
  * @param[in]  P_pP        Modulus of the curve
  * @param[in]  P_PLen      Byte length of the modulus
  * @param[in]  P_pN        Order of the curve
  * @param[in]  P_NLen      Byte length of the order
  * @param[in]  P_pA        Parameter A of the curve
  * @param[in]  P_ALen      Byte length of the parameter A
  * @param[in]  P_pB        Parameter B of the curve
  * @param[in]  P_BLen      Byte length of the parameter B
  * @param[in]  P_pGx       X coordinate of the Generator Point
  * @param[in]  P_GxLen     Byte length of the X coordinate
  * @param[in]  P_pGy       Y coordinate of the Generator Point
  * @param[in]  P_GyLen     Byte length of the Y coordinate
  * @note       All the array parameters are big-endian arrays, where the first byte is the most significant one.
  * @retval     CMOX_ECC_SUCCESS
  * @retval     CMOX_ECC_ERR_BAD_PARAMETERS
  * @retval     CMOX_ECC_ERR_MEMORY_FAIL
  * @warning    This function does not check if the parameters effectively form an elliptic curve,
  *             so the user must be sure about the parameters that are passed to the function
  */
cmox_ecc_retval_t cmox_ecc_customCurveConstruct(cmox_ecc_handle_t *P_pEccCtx,
                                                cmox_ecc_impl_t *P_pCurve,
                                                cmox_ecc_customCurve_opt_t P_OptLevel,
                                                const uint8_t *P_pP, size_t P_PLen,
                                                const uint8_t *P_pN, size_t P_NLen,
                                                const uint8_t *P_pA, size_t P_ALen,
                                                const uint8_t *P_pB, size_t P_BLen,
                                                const uint8_t *P_pGx, size_t P_GxLen,
                                                const uint8_t *P_pGy, size_t P_GyLen);

/**
  * @brief         Destroy (and release the allocated memory) a custom curve
  * @param[in]     P_pEccCtx Context for ECC operations
  * @param[in,out] P_pCurve  Pointer to the curve to destroy
  */
void cmox_ecc_customCurveCleanup(cmox_ecc_handle_t *P_pEccCtx, cmox_ecc_impl_t *P_pCurve);

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* CMOX_ECC_CUSTOM_CURVES_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
