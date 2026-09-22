/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_usbx_device.h
  * @author  MCD Application Team
  * @brief   USBX Device applicative header file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_USBX_DEVICE_H__
#define __APP_USBX_DEVICE_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "ux_api.h"
#include "ux_device_msc.h"
#include "ux_device_descriptors.h"
#include "ux_dcd_stm32.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usb_otg.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/

/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
UINT MX_USBX_Device_Init(VOID);
UINT MX_USBX_Device_Stack_Init(void);
UINT MX_USBX_Device_Stack_DeInit(void);

/* USER CODE BEGIN EFP */
VOID USBX_APP_Device_Init(VOID);
HAL_StatusTypeDef write_data_to_flash( uint8_t *buf, uint32_t blk_addr, uint16_t blk_len );

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN PD */
/**
  * @brief  flash and usb settings
  * @note   last 32 pages of Bank 2 will be used
  *         a
  */
#define STORAGE_LUN_NBR                  1
#define STORAGE_BLK_NBR                  512
#define STORAGE_BLK_SIZ                  0x200


#define USB_FLASH_START_ADDRESS  0x081C0000    //page 96 bank 2
#define TOTAL_USB_DEVICE_SIZE   ( STORAGE_BLK_NBR * STORAGE_BLK_SIZ )

/* USER CODE END PD */

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

#ifdef __cplusplus
}
#endif
#endif /* __APP_USBX_DEVICE_H__ */
