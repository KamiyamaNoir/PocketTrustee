/**
  ******************************************************************************
  * @file    cmox_check_default_gcm.h
  * @author  MCD Application Team
  * @brief   Header file for checking that the default values for AES are
  *          correctly set
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
#ifndef CMOX_CHECK_DEFAULT_GCM_H
#define CMOX_CHECK_DEFAULT_GCM_H

#if !defined(CMOX_DEFAULT_FILE)
#include "cmox_default_config.h"
#else
#include CMOX_DEFAULT_FILE
#endif /* CMOX_DEFAULT_FILE */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#ifndef CMOX_GCM_IMPLEMENTATION
#error CMOX_GCM_IMPLEMENTATION must be defined for using GCM
#endif /* CMOX_GCM_IMPLEMENTATION */

#ifndef CMOX_GCM_FAST
#error CMOX_GCM_FAST must be defined for using GCM
#endif /* CMOX_GCM_FAST */

#ifndef CMOX_GCM_SMALL
#error CMOX_GCM_SMALL must be defined for using GCM
#endif /* CMOX_GCM_SMALL */

#if ((CMOX_GCM_IMPLEMENTATION != CMOX_GCM_FAST) && (CMOX_GCM_IMPLEMENTATION != CMOX_GCM_SMALL))
#error CMOX_GCM_IMPLEMENTATION value must be CMOX_GCM_FAST or CMOX_GCM_SMALL
#endif /* ((CMOX_GCM_IMPLEMENTATION != CMOX_GCM_FAST) && (CMOX_GCM_IMPLEMENTATION != CMOX_GCM_SMALL)) */

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* CMOX_CHECK_DEFAULT_GCM_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
