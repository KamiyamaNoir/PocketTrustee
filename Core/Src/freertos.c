/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
uint32_t defaultTaskBuffer[ 1024 ];
osStaticThreadDef_t defaultTaskControlBlock;
osThreadId manager_taskHandle;
uint32_t manager_Buffer[ 512 ];
osStaticThreadDef_t manager_ControlBlock;
osThreadId IdealTaskHandle;
uint32_t IdealTaskBuffer[ 128 ];
osStaticThreadDef_t IdealTaskControlBlock;
osThreadId ADCSampleTaskHandle;
uint32_t ADCSBuffer[ 128 ];
osStaticThreadDef_t ADCSControlBlock;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void StartManagerTask(void const * argument);
void StartIdealTask(void const * argument);
void StartADCSample(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN PREPOSTSLEEP */
__weak void PreSleepProcessing(uint32_t ulExpectedIdleTime)
{
/* place for user code */
}

__weak void PostSleepProcessing(uint32_t ulExpectedIdleTime)
{
/* place for user code */
}
/* USER CODE END PREPOSTSLEEP */

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadStaticDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 1024, defaultTaskBuffer, &defaultTaskControlBlock);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of manager_task */
  osThreadStaticDef(manager_task, StartManagerTask, osPriorityNormal, 0, 512, manager_Buffer, &manager_ControlBlock);
  manager_taskHandle = osThreadCreate(osThread(manager_task), NULL);

  /* definition and creation of IdealTask */
  osThreadStaticDef(IdealTask, StartIdealTask, osPriorityLow, 0, 128, IdealTaskBuffer, &IdealTaskControlBlock);
  IdealTaskHandle = osThreadCreate(osThread(IdealTask), NULL);

  /* definition and creation of ADCSampleTask */
  osThreadStaticDef(ADCSampleTask, StartADCSample, osPriorityBelowNormal, 0, 128, ADCSBuffer, &ADCSControlBlock);
  ADCSampleTaskHandle = osThreadCreate(osThread(ADCSampleTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  osThreadSuspend(manager_taskHandle);
  osThreadSuspend(IdealTaskHandle);
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
__weak void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartManagerTask */
/**
* @brief Function implementing the manager_task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartManagerTask */
__weak void StartManagerTask(void const * argument)
{
  /* USER CODE BEGIN StartManagerTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartManagerTask */
}

/* USER CODE BEGIN Header_StartIdealTask */
/**
* @brief Function implementing the IdealTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartIdealTask */
void StartIdealTask(void const * argument)
{
  /* USER CODE BEGIN StartIdealTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartIdealTask */
}

/* USER CODE BEGIN Header_StartADCSample */
/**
* @brief Function implementing the ADCSampleTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartADCSample */
__weak void StartADCSample(void const * argument)
{
  /* USER CODE BEGIN StartADCSample */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartADCSample */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
