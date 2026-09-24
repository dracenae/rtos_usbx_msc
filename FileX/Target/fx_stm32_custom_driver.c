/**************************************************************************/
/*                                                                        */
/*       Copyright (c) Microsoft Corporation. All rights reserved.        */
/*                                                                        */
/*       This software is licensed under the Microsoft Software License   */
/*       Terms for Microsoft Azure RTOS. Full text of the license can be  */
/*       found in the LICENSE file at https://aka.ms/AzureRTOS_EULA       */
/*       and in the root directory of this software.                      */
/*                                                                        */
/**************************************************************************/

/* Private includes ----------------------------------------------------------*/
#include "fx_stm32_custom_driver.h"

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
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* USER CODE BEGIN USER_CODE_SECTION_0 */

/* USER CODE END USER_CODE_SECTION_0 */

VOID  fx_stm32_custom_driver(FX_MEDIA *media_ptr)
{
  /* USER CODE BEGIN USER_CODE_SECTION_1 */
	UCHAR *source_buffer;
	UCHAR *destination_buffer;
	UINT   bytes_per_sector;
	UCHAR *destination_buffer_end;
	UCHAR *source_buffer_end;

	UCHAR * disk_end = (UCHAR *)0x081FFFFF;
  /* USER CODE END USER_CODE_SECTION_1 */

  switch (media_ptr->fx_media_driver_request)
  {
    case FX_DRIVER_INIT:
    {

     /* USER CODE BEGIN DRIVER_INIT */

     /* USER CODE END DRIVER_INIT */

      media_ptr->fx_media_driver_status = FX_SUCCESS;

     /* USER CODE BEGIN POST_DRIVER_INIT */

     /* USER CODE END POST_DRIVER_INIT */
      break;
    }

    case FX_DRIVER_UNINIT:
    {
     /* USER CODE BEGIN DRIVER_UNINIT */

     /* USER CODE END DRIVER_UNINIT */

      media_ptr->fx_media_driver_status = FX_SUCCESS;

    /* USER CODE BEGIN POST_DRIVER_UNINIT */

     /* USER CODE END POST_DRIVER_UNINIT */
      break;
    }

    case FX_DRIVER_BOOT_READ:
    {
    /* USER CODE BEGIN DRIVER_BOOT_READ */
    	  /* Calculate the disk boot sector offset, which is at the very beginning of
    	             * the disk.
    	             */
    	            source_buffer =  (UCHAR *)FX_DISK_BASE_ADDRESS;
    	            /* ? pickup the bytes per sector.*/

    	            bytes_per_sector =  _fx_utility_16_unsigned_read(&source_buffer[FX_BYTES_SECTOR]);

    	            /* Ensure this is less than the media memory size.  */
    	            if (bytes_per_sector > media_ptr->fx_media_memory_size)
    	            {
    	                media_ptr->fx_media_driver_status =  FX_BUFFER_ERROR;
    	                break;
    	            }

    	            /* Copy the flash memory boot sector into the destination.  */
    	            _fx_utility_memory_copy(source_buffer, media_ptr -> fx_media_driver_buffer,
    	                                    bytes_per_sector);

     /* USER CODE END DRIVER_BOOT_READ */

      media_ptr->fx_media_driver_status = FX_SUCCESS;

    /* USER CODE BEGIN POST_DRIVER_BOOT_READ */

     /* USER CODE END POST_DRIVER_BOOT_READ */
      break;
    }

    case FX_DRIVER_READ:
    {
    /* USER CODE BEGIN DRIVER_READ */


        /* Calculate the disk sector offset.*/
        source_buffer = ((UCHAR *)FX_DISK_BASE_ADDRESS) +
                         ((media_ptr->fx_media_driver_logical_sector + media_ptr->fx_media_hidden_sectors) * media_ptr->fx_media_bytes_per_sector);

        /* Calculate source buffer end address */
        source_buffer_end = source_buffer + (media_ptr->fx_media_driver_sectors * media_ptr->fx_media_bytes_per_sector);

        /* Check source buffer does not exceed disk end address */
        if (source_buffer_end  > disk_end)
        {
          media_ptr -> fx_media_driver_status =  FX_PTR_ERROR;
          break;
        }

          /* Copy the from flash sector into the destination.  */
          _fx_utility_memory_copy(source_buffer, media_ptr -> fx_media_driver_buffer,
                                   media_ptr->fx_media_driver_sectors * media_ptr->fx_media_bytes_per_sector);

          /* Successful driver request.  */


     /* USER CODE END DRIVER_READ */

      media_ptr->fx_media_driver_status = FX_SUCCESS;

    /* USER CODE BEGIN POST_DRIVER_READ */

     /* USER CODE END POST_DRIVER_READ */
      break;
    }

    case FX_DRIVER_BOOT_WRITE:
    {
    /* USER CODE BEGIN DRIVER_BOOT_WRITE */
    	  /*
    	             * Calculate the disk boot sector offset, which is at the very beginning of the disk.
    	             */
    	            destination_buffer =  (UCHAR *)FX_DISK_BASE_ADDRESS;

    	            /* write  */

    	           write_data_to_flash((uint8_t *)media_ptr->fx_media_driver_buffer,0+media_ptr->fx_media_hidden_sectors,1);


     /* USER CODE END DRIVER_BOOT_WRITE */
    	            /* Successful driver request.  */

      media_ptr->fx_media_driver_status = FX_SUCCESS;

    /* USER CODE BEGIN POST_DRIVER_BOOT_WRITE */

     /* USER CODE END POST_DRIVER_BOOT_WRITE */
      break;
    }

    case FX_DRIVER_WRITE:
    {

    /* USER CODE BEGIN DRIVER_WRITE */
    	 /* Calculate the disk sector offset */
    	            destination_buffer =  (UCHAR *)FX_DISK_BASE_ADDRESS +
    	                                  ((media_ptr->fx_media_driver_logical_sector +  media_ptr->fx_media_hidden_sectors) * media_ptr->fx_media_bytes_per_sector);
    	            /* Calculate destination buffer end address */
    	            destination_buffer_end = destination_buffer + (media_ptr->fx_media_driver_sectors * media_ptr->fx_media_bytes_per_sector);

    	            /* Check destination buffer does not exceed disk end address */
    	            if (destination_buffer_end  > disk_end)
    	            {
    	              media_ptr -> fx_media_driver_status =  FX_PTR_ERROR;
    	              break;
    	            }


    	              /* Copy the source to the flash sector.  */
    	            if (write_data_to_flash((uint8_t *)media_ptr->fx_media_driver_buffer,(media_ptr->fx_media_driver_logical_sector +  media_ptr->fx_media_hidden_sectors),   media_ptr->fx_media_driver_sectors) != HAL_OK) {
    	                media_ptr->fx_media_driver_status = FX_IO_ERROR;
    	                break;
    	            }


     /* USER CODE END DRIVER_WRITE */

      media_ptr->fx_media_driver_status = FX_SUCCESS;

     /* USER CODE BEGIN POST_DRIVER_WRITE */

     /* USER CODE END POST_DRIVER_WRITE */
      break;
    }

    case FX_DRIVER_FLUSH:
    {
    /* USER CODE BEGIN DRIVER_FLUSH */

     /* USER CODE END DRIVER_FLUSH */

      media_ptr->fx_media_driver_status = FX_SUCCESS;

    /* USER CODE BEGIN POST_DRIVER_FLUSH */

     /* USER CODE END POST_DRIVER_FLUSH */
      break;
    }

    case FX_DRIVER_ABORT:
    {

    /* USER CODE BEGIN DRIVER_ABORT */

     /* USER CODE END DRIVER_ABORT */

      media_ptr->fx_media_driver_status = FX_SUCCESS;

    /* USER CODE BEGIN POST_DRIVER_ABORT */

     /* USER CODE END POST_DRIVER_ABORT */
      break;
    }

    default:
    {
        media_ptr->fx_media_driver_status = FX_IO_ERROR;
        break;
    }
  }
}

/* USER CODE BEGIN USER_CODE_SECTION_2 */

/* USER CODE END USER_CODE_SECTION_2 */

