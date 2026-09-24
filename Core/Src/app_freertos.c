/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_freertos.c
  * Description        : FreeRTOS applicative file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "app_freertos.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "event_groups.h"
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
//volatile uint32_t last_usb_sof_tick = 0;
//volatile uint8_t usb_device_is_connected = 0;

FX_MEDIA      my_media;
FX_FILE test_file;
FX_FILE file2;
uint8_t media_buffer[512];
uint8_t b=1;
static uint32_t i = 0U;
static char write_data[96];


GPIO_PinState previous_button_state = GPIO_PIN_RESET;
//CHAR write_data[] = "Hello from STM32 Internal Flash via FileX!";
UINT status;
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for TaskUSB */
osThreadId_t TaskUSBHandle;
const osThreadAttr_t TaskUSB_attributes = {
  .name = "TaskUSB",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 500 * 4
};
/* Definitions for TaskFileX */
osThreadId_t TaskFileXHandle;
const osThreadAttr_t TaskFileX_attributes = {
  .name = "TaskFileX",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 1002 * 4
};
/* Definitions for myMutex01 */
osMutexId_t myMutex01Handle;
const osMutexAttr_t myMutex01_attributes = {
  .name = "myMutex01"
};
/* Definitions for usbEvent */
osEventFlagsId_t usbEventHandle;
const osEventFlagsAttr_t usbEvent_attributes = {
  .name = "usbEvent"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */
  /* creation of myMutex01 */
  myMutex01Handle = osMutexNew(&myMutex01_attributes);

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
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of TaskUSB */
  TaskUSBHandle = osThreadNew(StartTaskUSB, NULL, &TaskUSB_attributes);

  /* creation of TaskFileX */
  TaskFileXHandle = osThreadNew(StartTaskFileX, NULL, &TaskFileX_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* creation of usbEvent */
  usbEventHandle = osEventFlagsNew(&usbEvent_attributes);

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}
/* USER CODE BEGIN Header_StartDefaultTask */
/**
* @brief Function implementing the defaultTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN defaultTask */
  /* Infinite loop */
  for(;;)
  {
	  HAL_GPIO_TogglePin(RED_LED_GPIO_Port, RED_LED_Pin);
    osDelay(500);
  }
  /* USER CODE END defaultTask */
}

/* USER CODE BEGIN Header_StartTaskUSB */
/**
* @brief Function implementing the TaskUSB thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskUSB */
void StartTaskUSB(void *argument)
{
  /* USER CODE BEGIN TaskUSB */
  /* Infinite loop */
  for(;;){
  ux_device_stack_tasks_run();

	  	    osDelay(1);



//
//	  UBaseType_t watermark;
//
//	      watermark = uxTaskGetStackHighWaterMark(TaskUSBHandle);


  }

  /* USER CODE END TaskUSB */
}

/* USER CODE BEGIN Header_StartTaskFileX */
/**
* @brief Function implementing the TaskFileX thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskFileX */
void StartTaskFileX(void *argument)
{
  /* USER CODE BEGIN TaskFileX */
	status = fx_media_open(
	        &my_media,
	        "STM32_FLASH_DISK",
	        fx_stm32_custom_driver,
	        NULL,
	        media_buffer,
	        sizeof(media_buffer)
	    );

	    if (status != FX_SUCCESS)
	    {

	    	status = fx_media_format(
	    	    	    &my_media,               /* media_ptr */
	    	    	    fx_stm32_custom_driver,  /* driver */
	    	    	    NULL,                    /* driver_info_ptr */
	    	    	    media_buffer,            /* memory_ptr */
	    	    	    sizeof(media_buffer),    /* memory_size (must be >= 512) */
	    	    	    "STM32_FLASH_DISK",              /* volume_name */
	    	    	    1,                       /* number_of_fats (set to 1) */
	    	    	    32,                      /* directory_entries */
	    	    	    0,                       /* hidden_sectors */
	    	    	    512,                      /* total_sectors */
	    	    	    512,                     /* bytes_per_sector */
	    	    	    16,                       /* sectors_per_cluster */
	    	    	    1,                       /* heads */
	    	    	    1                        /* sectors_per_track */
	    	   	);
	    	        if (status != FX_SUCCESS)
	    	        {
	    	            // Formatting failed - check your flash range and driver status
	    	            while(1);
	    	        }
	    	        status = fx_media_open(&my_media, "STM32_FLASH_DISK", fx_stm32_custom_driver, NULL,
	    	      	                                media_buffer, sizeof(media_buffer));


	    	      	        if (status != FX_SUCCESS)
	    	      	          {

	    	      	            Error_Handler();
	    	      	          }



	    }



	    status = fx_file_create(&my_media, "TEST.TXT");
	    if (status != FX_SUCCESS)
	             {
	    			if (status==FX_ALREADY_CREATED){}
	    			else	               Error_Handler();
	             }


  /* Infinite loop */
  for(;;){
//  {osEventFlagsWait(usbEventHandle, USB_DISCONNECTED_BIT, osFlagsWaitAny  , osWaitForever);
//
//do{
//	  HAL_GPIO_TogglePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin);
//	    osDelay(500);}
//while ((osEventFlagsGet(usbEventHandle) & USB_CONNECTED_BIT) != 0U);
//
//osEventFlagsClear(usbEventHandle,
//                          USB_CONNECTED_BIT |
//                          USB_DISCONNECTED_BIT);
//
//    osDelay(1000);

	  GPIO_PinState current_button_state =
	          HAL_GPIO_ReadPin(USER_BUTTON_GPIO_Port, USER_BUTTON_Pin);

	      /* Rising-edge detection: execute once when the button is pressed */
	      if (current_button_state == GPIO_PIN_SET &&
	          previous_button_state == GPIO_PIN_RESET)
	  {
		  status = fx_file_open(&my_media, &test_file, "TEST.TXT", FX_OPEN_FOR_WRITE);
		  	     if (status == FX_SUCCESS)
		  	     {
		  	    	 i++;
		  	    	snprintf(write_data,
		  	    	                              sizeof(write_data),
		  	    	                              "Hello from STM32 Internal Flash via FileX! i=%lu\r\n",
		  	    	                              (unsigned long)i);
		  	         // Write data to the file (triggers your driver's FX_DRIVER_WRITE and Read-Modify-Write)
		  	         fx_file_write(&test_file, write_data,  strlen(write_data));
		  	         fx_media_flush(&my_media);
		  	         // Close the file (flushes buffers)


		  	         fx_file_close(&test_file);
		  	       //file i pasted
		  	       status = fx_file_open(&my_media, &file2, "ana.txt", FX_OPEN_FOR_WRITE);
		  	      		  	     if (status == FX_SUCCESS)
		  	      		  	     {

		  	      		  	    	snprintf(write_data,
		  	      		  	    	                              sizeof(write_data),
		  	      		  	    	                              "Hello from STM32 Internal Flash via FileX! i=%lu\r\n",
		  	      		  	    	                              (unsigned long)i);
		  	      		  	         // Write data to the file (triggers your driver's FX_DRIVER_WRITE and Read-Modify-Write)
		  	      		  	         fx_file_write(&file2, write_data,  strlen(write_data));
		  	      		  	         fx_media_flush(&my_media);
		  	      		  	         // Close the file (flushes buffers)
		  	      		  	         fx_file_close(&file2);

		  	      		  	     }
		  	     }
		  	  }
	      previous_button_state = current_button_state;
	  osDelay(50);

  }
  /* USER CODE END TaskFileX */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */


/* USER CODE END Application */

