/**
  ******************************************************************************
  * @file    cmox_cta.h
  * @author  MCD Application Team
  * @brief   CTA specific features
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
#ifndef CMOX_CTA_H
#define CMOX_CTA_H

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/**
  * @brief Provides a label in order to move tables into RAM/CCM/TCM and being
  *        protected against Cache Timing Attacks (CTA)
  * @note  This macro uses extended GCC preprocessor features, that can be used
  *        with ARMCC compiler and EWARM compiler with GCC extension enabled.
  *        If not supported by the user toolchain configuration, it is possible
  *        to replace the macro with the following:
  *
  * \code
  * #define CMOX_CTA_RESISTANT _Pragma("location=\"CMOX_CTA_PROTECTED_DATA\"")
  * \endcode
  */
#define CMOX_CTA_RESISTANT __attribute__((section("CMOX_CTA_PROTECTED_DATA")))


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CMOX_CTA_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
