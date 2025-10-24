/**
  ******************************************************************************
  * @file      stm32_lock.h
  * @author    STMicroelectronics
  * @brief     STMicroelectronics lock mechanisms
  *
  * @details
  * This implementation supports the following strategies for handling
  * thread-safe locks. The strategy can be explicitly selected by
  * defining exactly one of the following macros in the project:
  * 1. STM32_THREAD_SAFE_USER_LOCKS=1
  * 2. STM32_THREAD_SAFE_BAREMETAL_ALLOW_LOCKS=1
  * 3. STM32_THREAD_SAFE_BAREMETAL_DENY_LOCKS=1
  * 4. STM32_THREAD_SAFE_FREERTOS_ALLOW_LOCKS=1
  * 5. STM32_THREAD_SAFE_FREERTOS_DENY_LOCKS=1
  * Please look at the '<toolchain/library>_lock_glue.c' file for more details.
  *
  * 1. User defined thread-safe implementation.
  *    User defined solution for handling thread-safety.
  *    <br>
  *    <b>NOTE:</b> The stubs in stm32_lock_user.h need to be implemented to gain
  *    thread-safety.
  *
  * 2. [<b>DEFAULT</b>] Allow lock usage from interrupts.
  *    This implementation will ensure thread-safety by disabling all interrupts
  *    during e.g. calls to malloc.
  *    <br>
  *    <b>NOTE:</b> Disabling all interrupts creates interrupt latency which
  *    might not be desired for this application!
  *
  * 3. Deny lock usage from interrupts.
  *    This implementation assumes single thread of execution.
  *    <br>
  *    <b>NOTE:</b> Thread-safety dependent functions will enter an infinity loop
  *    if used in interrupt context.
  *
  * 4. Allow lock usage from interrupts. Implemented using FreeRTOS locks.
  *    This implementation will ensure thread-safety by entering RTOS ISR capable
  *    critical sections during e.g. calls to malloc.
  *    By default this implementation supports 2 levels of recursive locking.
  *    Adding additional levels requires 4 bytes per lock per level of RAM.
  *    <br>
  *    <b>NOTE:</b> Interrupts with high priority are not disabled. This implies
  *    that the lock is not thread-safe from high priority interrupts!
  *
  * 5. Deny lock usage from interrupts. Implemented using FreeRTOS locks.
  *    This implementation will ensure thread-safety by suspending all tasks
  *    during e.g. calls to malloc.
  *    <br>
  *    <b>NOTE:</b> Thread-safety dependent functions will enter an infinity loop
  *    if used in interrupt context.
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021, 2023 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
 */

#ifndef __STM32_LOCK_H__
#define __STM32_LOCK_H__

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stddef.h>
#include <cmsis_compiler.h>


/** Ensure compatibility with older versions */
#ifdef STM32_THREAD_SAFE_STRATEGY

#if STM32_THREAD_SAFE_STRATEGY == 1
#define STM32_THREAD_SAFE_USER_LOCKS 1
#elif STM32_THREAD_SAFE_STRATEGY == 2
#define STM32_THREAD_SAFE_BAREMETAL_ALLOW_LOCKS 1
#elif STM32_THREAD_SAFE_STRATEGY == 3
#define STM32_THREAD_SAFE_BAREMETAL_DENY_LOCKS 1
#elif STM32_THREAD_SAFE_STRATEGY == 4
#define STM32_THREAD_SAFE_FREERTOS_ALLOW_LOCKS 1
#elif STM32_THREAD_SAFE_STRATEGY == 5
#define STM32_THREAD_SAFE_FREERTOS_DENY_LOCKS 1
#else
#error Invalid value for STM32_THREAD_SAFE_STRATEGY: expected 1..5
#endif /* STM32_THREAD_SAFE_STRATEGY value switch */

#endif /* STM32_THREAD_SAFE_STRATEGY */

#if !defined(STM32_THREAD_SAFE_USER_LOCKS) \
 && !defined(STM32_THREAD_SAFE_BAREMETAL_ALLOW_LOCKS) \
 && !defined(STM32_THREAD_SAFE_BAREMETAL_DENY_LOCKS) \
 && !defined(STM32_THREAD_SAFE_FREERTOS_ALLOW_LOCKS) \
 && !defined(STM32_THREAD_SAFE_FREERTOS_DENY_LOCKS)

/** Assume strategy "bare metal - allow" if not specified */
#define STM32_THREAD_SAFE_BAREMETAL_ALLOW_LOCKS 1

#warning No STM32_THREAD_SAFE_* macro was defined
#warning Defaulting to STM32_THREAD_SAFE_BAREMETAL_ALLOW_LOCKS

#endif /* no strategy macro defined */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Function prototypes -------------------------------------------------------*/
void Error_Handler(void);

/* Public macros -------------------------------------------------------------*/
/** Blocks execution */
#define STM32_LOCK_BLOCK()                      \
  do                                            \
  {                                             \
    __disable_irq();                            \
    Error_Handler();                            \
    while (1);                                  \
  } while (0)

/** Blocks execution if argument is NULL */
#define STM32_LOCK_BLOCK_IF_NULL_ARGUMENT(x)    \
  do                                            \
  {                                             \
    if ((x) == NULL)                            \
    {                                           \
      STM32_LOCK_BLOCK();                       \
    }                                           \
  } while (0)

/** Blocks execution if in interrupt context */
#define STM32_LOCK_BLOCK_IF_INTERRUPT_CONTEXT() \
  do                                            \
  {                                             \
    if (__get_IPSR())                           \
    {                                           \
      STM32_LOCK_BLOCK();                       \
    }                                           \
  } while (0)

/** Hide unused parameter warning from compiler */
#define STM32_LOCK_UNUSED(var) (void)var

/** Size of array */
#define STM32_LOCK_ARRAY_SIZE(array) (sizeof(array) / sizeof((array)[0]))

#if defined(STM32_THREAD_SAFE_USER_LOCKS) && (STM32_THREAD_SAFE_USER_LOCKS != 0)
/*
  * User defined thread-safe implementation.
  */

/* Includes ----------------------------------------------------------------*/
/** STM32 lock API version */
#define STM32_LOCK_API 1
#include "stm32_lock_user.h"
#undef STM32_LOCK_API

#elif defined(STM32_THREAD_SAFE_BAREMETAL_ALLOW_LOCKS) && (STM32_THREAD_SAFE_BAREMETAL_ALLOW_LOCKS != 0)
/*
 * Allow lock usage from interrupts.
 */

/* Private defines ---------------------------------------------------------*/
/** Initialize members in instance of <code>LockingData_t</code> structure */
#define LOCKING_DATA_INIT 0

/* Private typedef ---------------------------------------------------------*/
typedef uint8_t LockingData_t; /* not used */

/* Private variables -------------------------------------------------------*/
extern uint32_t gflag;
extern uint32_t call_counter;

/* Private functions -------------------------------------------------------*/
/**
  * @brief Initialize STM32 lock
  * @param lock The lock to init
  */
static inline void stm32_lock_init(LockingData_t *lock)
{
  STM32_LOCK_BLOCK_IF_NULL_ARGUMENT(lock);
}

/**
  * @brief Acquire STM32 lock
  * @param lock The lock to acquire
  */
static inline void stm32_lock_acquire(LockingData_t *lock)
{
  uint32_t flag = __get_PRIMASK();
  (void)(lock);
  __disable_irq();
  if (call_counter == 0)
  {
    gflag = flag;
  }
  call_counter++;
}

/**
  * @brief Release STM32 lock
  * @param lock The lock to release
  */
static inline void stm32_lock_release(LockingData_t *lock)
{
  (void)(lock);
  call_counter--;
  if (call_counter == 0)
  {
    __set_PRIMASK(gflag);
  }
}

#elif defined(STM32_THREAD_SAFE_BAREMETAL_DENY_LOCKS) && (STM32_THREAD_SAFE_BAREMETAL_DENY_LOCKS != 0)
/*
 * Deny lock usage from interrupts.
 */

/* Private defines ---------------------------------------------------------*/
/** Initialize members in instance of <code>LockingData_t</code> structure */
#define LOCKING_DATA_INIT 0

/* Private typedef ---------------------------------------------------------*/
typedef uint8_t LockingData_t;  /**< Unused */

/* Private functions -------------------------------------------------------*/

/**
  * @brief Initialize STM32 lock
  * @param lock The lock to init
  */
static inline void stm32_lock_init(LockingData_t *lock)
{
  STM32_LOCK_BLOCK_IF_NULL_ARGUMENT(lock);
}

/**
  * @brief Acquire STM32 lock
  * @param lock The lock to acquire
  */
static inline void stm32_lock_acquire(LockingData_t *lock)
{
  STM32_LOCK_BLOCK_IF_NULL_ARGUMENT(lock);
  STM32_LOCK_BLOCK_IF_INTERRUPT_CONTEXT();
}

/**
  * @brief Release ST lock
  * @param lock The lock to release
  */
static inline void stm32_lock_release(LockingData_t *lock)
{
  STM32_LOCK_BLOCK_IF_NULL_ARGUMENT(lock);
  STM32_LOCK_BLOCK_IF_INTERRUPT_CONTEXT();
}

#elif defined(STM32_THREAD_SAFE_FREERTOS_ALLOW_LOCKS) && (STM32_THREAD_SAFE_FREERTOS_ALLOW_LOCKS != 0)
/*
 * Allow lock usage from interrupts. Implemented using FreeRTOS locks.
 */

/* Includes ----------------------------------------------------------------*/
#include <FreeRTOS.h>
#include <task.h>

#if defined (__GNUC__) && !defined (__CC_ARM) && !(defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)) && configUSE_NEWLIB_REENTRANT == 0
#warning Please set configUSE_NEWLIB_REENTRANT to 1 in FreeRTOSConfig.h, otherwise newlib will not be thread-safe
#endif /* defined (__GNUC__) && !defined (__CC_ARM) && !(defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)) && configUSE_NEWLIB_REENTRANT == 0 */

/* Private defines ---------------------------------------------------------*/
/** Initialize members in instance of <code>LockingData_t</code> structure */
#define LOCKING_DATA_INIT { {0, 0}, 0 }
#define STM32_LOCK_MAX_NESTED_LEVELS 2 /**< Max nesting level of interrupts */
typedef struct
{
  uint32_t basepri[STM32_LOCK_MAX_NESTED_LEVELS];
  uint8_t nesting_level;
} LockingData_t;

/* Private macros ----------------------------------------------------------*/
/** Blocks execution if reached max nesting level */
#define STM32_LOCK_ASSERT_VALID_NESTING_LEVEL(lock)                   \
  do                                                                  \
  {                                                                   \
    if (lock->nesting_level >= STM32_LOCK_ARRAY_SIZE(lock->basepri))  \
    {                                                                 \
      STM32_LOCK_BLOCK();                                             \
    }                                                                 \
  } while (0)

/* Private functions -------------------------------------------------------*/

/**
  * @brief Initialize STM32 lock
  * @param lock The lock to init
  */
static inline void stm32_lock_init(LockingData_t *lock)
{
  STM32_LOCK_BLOCK_IF_NULL_ARGUMENT(lock);
  for (size_t i = 0; i < STM32_LOCK_ARRAY_SIZE(lock->basepri); i++)
  {
    lock->basepri[i] = 0;
  }
  lock->nesting_level = 0;
}

/**
  * @brief Acquire STM32 lock
  * @param lock The lock to acquire
  */
static inline void stm32_lock_acquire(LockingData_t *lock)
{
  STM32_LOCK_BLOCK_IF_NULL_ARGUMENT(lock);
  STM32_LOCK_ASSERT_VALID_NESTING_LEVEL(lock);
  if (xPortIsInsideInterrupt() == pdFALSE)
  {
   taskENTER_CRITICAL();
  }
  else
  {
    lock->basepri[lock->nesting_level] = taskENTER_CRITICAL_FROM_ISR();
    lock->nesting_level++;
  }
}

/**
  * @brief Release STM32 lock
  * @param lock The lock to release
  */
static inline void stm32_lock_release(LockingData_t *lock)
{
  STM32_LOCK_BLOCK_IF_NULL_ARGUMENT(lock);
  if (xPortIsInsideInterrupt() == pdFALSE)
  {
    taskEXIT_CRITICAL();
  }
  else
  {
    lock->nesting_level--;
    STM32_LOCK_ASSERT_VALID_NESTING_LEVEL(lock);
    taskEXIT_CRITICAL_FROM_ISR(lock->basepri[lock->nesting_level]);
  }
}

#undef STM32_LOCK_ASSERT_VALID_NESTING_LEVEL
#undef STM32_LOCK_MAX_NESTED_LEVELS

#elif defined(STM32_THREAD_SAFE_FREERTOS_DENY_LOCKS) && (STM32_THREAD_SAFE_FREERTOS_DENY_LOCKS != 0)
/*
 * Deny lock usage from interrupts. Implemented using FreeRTOS locks.
 */

/* Includes ----------------------------------------------------------------*/
#include <FreeRTOS.h>
#include <task.h>
#if defined (__GNUC__) && !defined (__CC_ARM) && configUSE_NEWLIB_REENTRANT == 0
#warning Please set configUSE_NEWLIB_REENTRANT to 1 in FreeRTOSConfig.h, otherwise newlib will not be thread-safe
#endif /* defined (__GNUC__) && !defined (__CC_ARM) && configUSE_NEWLIB_REENTRANT == 0 */

/* Private defines ---------------------------------------------------------*/
/** Initialize members in instance of <code>LockingData_t</code> structure */
#define LOCKING_DATA_INIT 0

/* Private typedef ---------------------------------------------------------*/
typedef uint8_t LockingData_t;  /**< Unused */

/* Private functions -------------------------------------------------------*/

/**
  * @brief Initialize STM32 lock
  * @param lock The lock to init
  */
static inline void stm32_lock_init(LockingData_t *lock)
{
  STM32_LOCK_BLOCK_IF_NULL_ARGUMENT(lock);
}

/**
  * @brief Acquire STM32 lock
  * @param lock The lock to acquire
  */
static inline void stm32_lock_acquire(LockingData_t *lock)
{
  STM32_LOCK_BLOCK_IF_NULL_ARGUMENT(lock);
  STM32_LOCK_BLOCK_IF_INTERRUPT_CONTEXT();
#if configUSE_PREEMPTION == 1
  vTaskSuspendAll();
#endif /* configUSE_PREEMPTION == 1 */
}

/**
  * @brief Release STM32 lock
  * @param lock The lock to release
  */
static inline void stm32_lock_release(LockingData_t *lock)
{
  STM32_LOCK_BLOCK_IF_NULL_ARGUMENT(lock);
  STM32_LOCK_BLOCK_IF_INTERRUPT_CONTEXT();
#if configUSE_PREEMPTION == 1
  xTaskResumeAll();
#endif /* configUSE_PREEMPTION == 1 */
}

#endif /* STM32_THREAD_SAFE_xx */

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* __STM32_LOCK_H__ */
