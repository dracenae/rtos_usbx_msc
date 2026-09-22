/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_usbx_device.c
  * @author  MCD Application Team
  * @brief   USBX Device applicative file
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
#include "app_usbx_device.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
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

static ULONG storage_interface_number;
static ULONG storage_configuration_number;
static UX_SLAVE_CLASS_STORAGE_PARAMETER storage_parameter;
extern PCD_HandleTypeDef           hpcd_USB_OTG_FS;

/* USER CODE BEGIN PV */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static UINT USBD_ChangeFunction(ULONG Device_State);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/**
  * @brief  Application USBX Device Initialization.
  * @param  none
  * @retval status
  */

UINT MX_USBX_Device_Init(VOID)
{
  UINT ret = UX_SUCCESS;
  /* USER CODE BEGIN MX_USBX_Device_Init 0 */
  /* USER CODE END MX_USBX_Device_Init 0 */

  /* Initialize the Stack USB Device*/
  if (MX_USBX_Device_Stack_Init() != UX_SUCCESS)
  {
    /* USER CODE BEGIN MAIN_INITIALIZE_STACK_ERROR */
    return UX_ERROR;
    /* USER CODE END MAIN_INITIALIZE_STACK_ERROR */
  }

  /* USER CODE BEGIN MX_USBX_Device_Init 1 */
  USBX_APP_Device_Init();
  /* USER CODE END MX_USBX_Device_Init 1 */

  /* USER CODE BEGIN MX_USBX_Device_Init 2 */
  /* USER CODE END MX_USBX_Device_Init 2 */

  return ret;
}

/**
  * @brief  MX_USBX_Device_Stack_Init
  *         Intialization of USB Device.
  *         Initialize the device stack, register of device class stack
  *         Register of the usb device controller
  * @param  None
  * @retval ret
  */
UINT MX_USBX_Device_Stack_Init(void)
{
  UINT ret = UX_SUCCESS;
  UCHAR *device_framework_high_speed;
  UCHAR *device_framework_full_speed;
  ULONG device_framework_hs_length;
  ULONG device_framework_fs_length;
  ULONG string_framework_length;
  ULONG language_id_framework_length;
  UCHAR *string_framework;
  UCHAR *language_id_framework;

  /* USER CODE BEGIN MX_USBX_Device_Stack_Init 0 */

  /* USER CODE END MX_USBX_Device_Stack_Init 0 */
  /* Get Device Framework High Speed and get the length */
  device_framework_high_speed = USBD_Get_Device_Framework_Speed(USBD_HIGH_SPEED,
                                                                &device_framework_hs_length);

  /* Get Device Framework Full Speed and get the length */
  device_framework_full_speed = USBD_Get_Device_Framework_Speed(USBD_FULL_SPEED,
                                                                &device_framework_fs_length);

  /* Get String Framework and get the length */
  string_framework = USBD_Get_String_Framework(&string_framework_length);

  /* Get Language Id Framework and get the length */
  language_id_framework = USBD_Get_Language_Id_Framework(&language_id_framework_length);

  /* Install the device portion of USBX */
  if (ux_device_stack_initialize(device_framework_high_speed,
                                 device_framework_hs_length,
                                 device_framework_full_speed,
                                 device_framework_fs_length,
                                 string_framework,
                                 string_framework_length,
                                 language_id_framework,
                                 language_id_framework_length,
                                 USBD_ChangeFunction) != UX_SUCCESS)
  {
    /* USER CODE BEGIN USBX_DEVICE_INITIALIZE_ERROR */
    return UX_ERROR;
    /* USER CODE END USBX_DEVICE_INITIALIZE_ERROR */
  }

  /* Initialize the storage class parameters for the device */
  storage_parameter.ux_slave_class_storage_instance_activate   = USBD_STORAGE_Activate;
  storage_parameter.ux_slave_class_storage_instance_deactivate = USBD_STORAGE_Deactivate;

  /* Store the number of LUN in this device storage instance */
  storage_parameter.ux_slave_class_storage_parameter_number_lun = STORAGE_NUMBER_LUN;

  /* Initialize the storage class parameters for reading/writing to the Flash Disk */
  storage_parameter.ux_slave_class_storage_parameter_lun[0].
    ux_slave_class_storage_media_last_lba = USBD_STORAGE_GetMediaLastLba();

  storage_parameter.ux_slave_class_storage_parameter_lun[0].
    ux_slave_class_storage_media_block_length = USBD_STORAGE_GetMediaBlocklength();

  storage_parameter.ux_slave_class_storage_parameter_lun[0].
    ux_slave_class_storage_media_type = 0;

  storage_parameter.ux_slave_class_storage_parameter_lun[0].
    ux_slave_class_storage_media_removable_flag = STORAGE_REMOVABLE_FLAG;

  storage_parameter.ux_slave_class_storage_parameter_lun[0].
    ux_slave_class_storage_media_read_only_flag = STORAGE_READ_ONLY;

  storage_parameter.ux_slave_class_storage_parameter_lun[0].
    ux_slave_class_storage_media_read = USBD_STORAGE_Read;

  storage_parameter.ux_slave_class_storage_parameter_lun[0].
    ux_slave_class_storage_media_write = USBD_STORAGE_Write;

  storage_parameter.ux_slave_class_storage_parameter_lun[0].
    ux_slave_class_storage_media_flush = USBD_STORAGE_Flush;

  storage_parameter.ux_slave_class_storage_parameter_lun[0].
    ux_slave_class_storage_media_status = USBD_STORAGE_Status;

  storage_parameter.ux_slave_class_storage_parameter_lun[0].
    ux_slave_class_storage_media_notification = USBD_STORAGE_Notification;

  /* USER CODE BEGIN STORAGE_PARAMETER */

  /* USER CODE END STORAGE_PARAMETER */

  /* Get storage configuration number */
  storage_configuration_number = USBD_Get_Configuration_Number(CLASS_TYPE_MSC, 0);

  /* Find storage interface number */
  storage_interface_number = USBD_Get_Interface_Number(CLASS_TYPE_MSC, 0);

  /* Initialize the device storage class */
  if (ux_device_stack_class_register(_ux_system_slave_class_storage_name,
                                     ux_device_class_storage_entry,
                                     storage_configuration_number,
                                     storage_interface_number,
                                     &storage_parameter) != UX_SUCCESS)
  {
    /* USER CODE BEGIN USBX_DEVICE_STORAGE_REGISTER_ERROR */
    return UX_ERROR;
    /* USER CODE END USBX_DEVICE_STORAGE_REGISTER_ERROR */
  }

  /* USER CODE BEGIN MX_USBX_Device_Stack_Init_PostTreatment */
  /* USER CODE END MX_USBX_Device_Stack_Init_PostTreatment */

  /* USER CODE BEGIN MX_USBX_Device_Stack_Init 1 */

  /* USER CODE END MX_USBX_Device_Stack_Init 1 */

  return ret;
}

/**
  * @brief  MX_USBX_Device_Stack_DeInit
  *         Unitialization of USB Device.
  *         uninitialize the device stack, unregister of device class stack
  *         unregister of the usb device controller
  * @retval ret
  */
UINT MX_USBX_Device_Stack_DeInit(void)
{
  UINT ret = UX_SUCCESS;

  /* USER CODE BEGIN MX_USBX_Device_Stack_DeInit_PreTreatment_0 */
  /* USER CODE END MX_USBX_Device_Stack_DeInit_PreTreatment_0 */

  /* Unregister USB device controller. */

/* Unregister storage class. */
  if (ux_device_stack_class_unregister(_ux_system_slave_class_storage_name,
                                       ux_device_class_storage_entry) != UX_SUCCESS)
  {
    return UX_ERROR;
  }

  /* The code below is required for uninstalling the device portion of USBX.  */
  if (ux_device_stack_uninitialize() != UX_SUCCESS)
  {
    return UX_ERROR;
  }

  /* USER CODE BEGIN MX_USBX_Device_Stack_DeInit_PreTreatment_1 */
  /* USER CODE END MX_USBX_Device_Stack_DeInit_PreTreatment_1 */

  /* USER CODE BEGIN MX_USBX_Device_Stack_DeInit_PostTreatment */
  /* USER CODE END MX_USBX_Device_Stack_DeInit_PostTreatment */

  return ret;
}

/**
  * @brief  USBD_ChangeFunction
  *         This function is called when the device state changes.
  * @param  Device_State: USB Device State
  * @retval status
  */
static UINT USBD_ChangeFunction(ULONG Device_State)
{
   UINT status = UX_SUCCESS;

  /* USER CODE BEGIN USBD_ChangeFunction0 */

  /* USER CODE END USBD_ChangeFunction0 */

  switch (Device_State)
  {
    case UX_DEVICE_ATTACHED:

      /* USER CODE BEGIN UX_DEVICE_ATTACHED */

      /* USER CODE END UX_DEVICE_ATTACHED */

      break;

    case UX_DEVICE_REMOVED:

      /* USER CODE BEGIN UX_DEVICE_REMOVED */

      /* USER CODE END UX_DEVICE_REMOVED */

      break;

    case UX_DCD_STM32_DEVICE_CONNECTED:

      /* USER CODE BEGIN UX_DCD_STM32_DEVICE_CONNECTED */

      /* USER CODE END UX_DCD_STM32_DEVICE_CONNECTED */

      break;

    case UX_DCD_STM32_DEVICE_DISCONNECTED:

      /* USER CODE BEGIN UX_DCD_STM32_DEVICE_DISCONNECTED */

      /* USER CODE END UX_DCD_STM32_DEVICE_DISCONNECTED */

      break;

    case UX_DCD_STM32_DEVICE_SUSPENDED:

      /* USER CODE BEGIN UX_DCD_STM32_DEVICE_SUSPENDED */

      /* USER CODE END UX_DCD_STM32_DEVICE_SUSPENDED */

      break;

    case UX_DCD_STM32_DEVICE_RESUMED:

      /* USER CODE BEGIN UX_DCD_STM32_DEVICE_RESUMED */

      /* USER CODE END UX_DCD_STM32_DEVICE_RESUMED */

      break;

    case UX_DCD_STM32_SOF_RECEIVED:

      /* USER CODE BEGIN UX_DCD_STM32_SOF_RECEIVED */

      /* USER CODE END UX_DCD_STM32_SOF_RECEIVED */

      break;

    default:

      /* USER CODE BEGIN DEFAULT */

      /* USER CODE END DEFAULT */

      break;

  }

  /* USER CODE BEGIN USBD_ChangeFunction1 */

  /* USER CODE END USBD_ChangeFunction1 */

  return status;
}
/* USER CODE BEGIN 1 */
/**
  * @brief  Writes data into the FLASH page by page
  * @param  buf: data buffer.
  * @param  blk_addr: Logical block address.
  * @param  blk_len: Blocks number.
  * @retval HAL_StatusTypeDef
  *
  * @note disabling cache is necessary
  */

#define FLASH_PAGE_SIZE_BYTES  (8 * 1024) // 8 Kbytes per page
#define BLOCKS_PER_PAGE        (FLASH_PAGE_SIZE_BYTES / STORAGE_BLK_SIZ) // 16 blocks

HAL_StatusTypeDef write_data_to_flash(uint8_t *buf, uint32_t blk_addr, uint16_t blk_len)
{
  HAL_StatusTypeDef ret = HAL_OK;

  // Explicitly align the buffer for safe flash programming ?
  //static __attribute__((aligned(16))) uint8_t page_buffer[FLASH_PAGE_SIZE_BYTES];

  static uint8_t page_buffer[FLASH_PAGE_SIZE_BYTES];
  while (blk_len > 0)
  {
    uint32_t target_page_index   = blk_addr / BLOCKS_PER_PAGE;
    uint32_t absolute_page_num    = 96 + target_page_index;
    uint32_t page_start_address   = USB_FLASH_START_ADDRESS + (target_page_index * FLASH_PAGE_SIZE_BYTES);

    uint32_t block_offset_in_page = blk_addr % BLOCKS_PER_PAGE;
    uint32_t blocks_remaining_in_page = BLOCKS_PER_PAGE - block_offset_in_page;

    uint16_t blocks_to_write = (blk_len < blocks_remaining_in_page) ? blk_len : blocks_remaining_in_page;

    // 1. Read the current 8 KB page from Flash into RAM
    memcpy(page_buffer, (const void *)page_start_address, FLASH_PAGE_SIZE_BYTES);

    // 2. Modify only the targeted blocks inside this page buffer
    uint32_t byte_offset = block_offset_in_page * STORAGE_BLK_SIZ;
    memcpy(&page_buffer[byte_offset], buf, (blocks_to_write * STORAGE_BLK_SIZ));

    // 3. Unlock Flash and manage caches for STM32U5 safety
    ret = HAL_FLASH_Unlock();
    if (ret != HAL_OK) break;

#ifdef HAL_ICACHE_MODULE_ENABLED
    HAL_ICACHE_Disable();

    HAL_ICACHE_WaitForInvalidateComplete();
#endif


    // 4. Erase the single 8 KB page
    FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t PageError;
    EraseInitStruct.TypeErase    = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.Page         = absolute_page_num;
    EraseInitStruct.NbPages      = 1;
    EraseInitStruct.Banks        = FLASH_BANK_2;

    ret = HAL_FLASHEx_Erase(&EraseInitStruct, &PageError);
    if (ret != HAL_OK)
    {
      HAL_FLASH_Lock();
      break;
    }

    // 5. Reprogram the 8 KB page using Quadword writes (16 bytes at a time)
    uint32_t quadword[4];
    for (uint32_t i = 0; i < FLASH_PAGE_SIZE_BYTES; i += 16U)
    {
      memcpy(quadword, &page_buffer[i], 16U);
      ret = HAL_FLASH_Program(FLASH_TYPEPROGRAM_QUADWORD,
                            (page_start_address + i),
                            (uint32_t)quadword);
      if (ret != HAL_OK)
      {
        break;
      }
    }


#ifdef HAL_ICACHE_MODULE_ENABLED
    HAL_ICACHE_Enable();
#endif

    HAL_FLASH_Lock();
    if (ret != HAL_OK) break;

    // 6. Advance pointers for the next iteration
    blk_addr += blocks_to_write;
    buf      += blocks_to_write * STORAGE_BLK_SIZ;
    blk_len  -= blocks_to_write;
  }

  return ret;
}
/**
  * @brief  USBX_APP_Device_Init
  *         Initialization of USB device.
  * @param  none
  * @retval none
  */
VOID USBX_APP_Device_Init(VOID)
{
 /* USER CODE BEGIN USB_Device_Init_PreTreatment_0 */
 /* USER CODE END USB_Device_Init_PreTreatment_0 */
 /* initialize the device controller HAL driver */
	 MX_USB_OTG_FS_PCD_Init();
 /* USER CODE BEGIN USB_Device_Init_PreTreatment_1 */

	 HAL_PCDEx_SetRxFiFo(&hpcd_USB_OTG_FS, 43);
	 HAL_PCDEx_SetTxFiFo(&hpcd_USB_OTG_FS, 0, 16);
	 HAL_PCDEx_SetTxFiFo(&hpcd_USB_OTG_FS, 1, 0x100);


 /* USER CODE END USB_Device_Init_PreTreatment_1 */
 /* initialize and link controller HAL driver to USBx */
 if(_ux_dcd_stm32_initialize((ULONG)USB_OTG_FS, (ULONG)&hpcd_USB_OTG_FS) != UX_SUCCESS)
 {
 Error_Handler();
 }
 /* USER CODE BEGIN USB_Device_Init_PostTreatment */
 HAL_PCD_Start(&hpcd_USB_OTG_FS);
 /* USER CODE END USB_Device_Init_PostTreatment */
}
/* USER CODE END 1 */
