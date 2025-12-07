/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file            : usb_host.c
  * @version         : v1.0_Cube
  * @brief           : This file implements the USB Host
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

#include "usb_host.h"
#include "usbh_core.h"
#include "usbh_audio.h"
#include "usbh_cdc.h"
#include "usbh_msc.h"
#include "usbh_hid.h"
#include "usbh_mtp.h"

/* USER CODE BEGIN Includes */
#include "hmiBridge.h"
#include "usbh_ctlreq.h"

#include <stdio.h>
#include <string.h>
/* USER CODE END Includes */

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USB Host core handle declaration */
USBH_HandleTypeDef hUsbHostHS;
ApplicationTypeDef Appli_state = APPLICATION_IDLE;

/*
 * -- Insert your variables declaration here --
 */
/* USER CODE BEGIN 0 */
static const char* usb_host_speed_to_str(USBH_SpeedTypeDef speed)
{
  switch (speed)
  {
    case USBH_SPEED_HIGH: return "High-speed";
    case USBH_SPEED_FULL: return "Full-speed";
    case USBH_SPEED_LOW:  return "Low-speed";
    default:              return "Unknown-speed";
  }
}

static const char* usb_class_to_str(uint8_t class_code)
{
  switch (class_code)
  {
    case 0x01: return "Audio";
    case 0x02: return "Communications";
    case 0x03: return "HID";
    case 0x05: return "Physical";
    case 0x06: return "Image";
    case 0x07: return "Printer";
    case 0x08: return "Mass Storage";
    case 0x09: return "Hub";
    case 0x0A: return "CDC-Data";
    case 0x0B: return "Smart Card";
    case 0x0D: return "Content Security";
    case 0x0E: return "Video";
    case 0x0F: return "Personal Healthcare";
    case 0xDC: return "Diagnostic";
    case 0xE0: return "Wireless";
    case 0xEF: return "Misc";
    case 0xFE: return "Application Specific";
    case 0xFF: return "Vendor Specific";
    default:   return "Unknown";
  }
}

static void log_string_descriptor(USBH_HandleTypeDef *phost, uint8_t index, const char *label)
{
  if (index == 0U)
  {
    return;
  }

  uint8_t buffer[64] = {0};
  if (USBH_Get_StringDesc(phost, index, buffer, sizeof(buffer)) == USBH_OK)
  {
    char msg[96];
    snprintf(msg, sizeof(msg), "%s: %s", label, buffer);
    HMI_addSystemMessage(msg);
  }
  else
  {
    char msg[64];
    snprintf(msg, sizeof(msg), "%s string read failed", label);
    HMI_addSystemMessage(msg);
  }
}

static void log_connected_device_info(USBH_HandleTypeDef *phost)
{
  const USBH_DevDescTypeDef *dev = &phost->device.DevDesc;
  const USBH_CfgDescTypeDef *cfg = &phost->device.CfgDesc;

  char msg[128];
  snprintf(msg, sizeof(msg), "VID:PID 0x%04X:0x%04X | USB %x.%02x | %s", dev->idVendor,
           dev->idProduct, (dev->bcdUSB >> 8) & 0xFF, dev->bcdUSB & 0xFF,
           usb_host_speed_to_str(phost->device.speed));
  HMI_addSystemMessage(msg);

  snprintf(msg, sizeof(msg), "Device class 0x%02X (%s), subclass 0x%02X, protocol 0x%02X",
           dev->bDeviceClass, usb_class_to_str(dev->bDeviceClass), dev->bDeviceSubClass,
           dev->bDeviceProtocol);
  HMI_addSystemMessage(msg);

  snprintf(msg, sizeof(msg), "Configuration %u: %u interface(s), %u mA max",
           cfg->bConfigurationValue, cfg->bNumInterfaces,
           (unsigned int)(cfg->bMaxPower * 2U));
  HMI_addSystemMessage(msg);

  if (cfg->bNumInterfaces > 0U)
  {
    const USBH_InterfaceDescTypeDef *itf = &cfg->Itf_Desc[phost->device.current_interface];
    snprintf(msg, sizeof(msg), "Interface 0 class 0x%02X (%s), subclass 0x%02X, protocol 0x%02X",
             itf->bInterfaceClass, usb_class_to_str(itf->bInterfaceClass),
             itf->bInterfaceSubClass, itf->bInterfaceProtocol);
    HMI_addSystemMessage(msg);
  }

  log_string_descriptor(phost, dev->iManufacturer, "Manufacturer");
  log_string_descriptor(phost, dev->iProduct, "Product");
  log_string_descriptor(phost, dev->iSerialNumber, "Serial");
}

static const char* usb_host_state_to_str(HOST_StateTypeDef state)
{
  switch (state)
  {
    case HOST_IDLE:                    return "HOST_IDLE";
    case HOST_DEV_WAIT_FOR_ATTACHMENT: return "HOST_DEV_WAIT_FOR_ATTACHMENT";
    case HOST_DEV_ATTACHED:            return "HOST_DEV_ATTACHED";
    case HOST_DEV_DISCONNECTED:        return "HOST_DEV_DISCONNECTED";
    case HOST_DETECT_DEVICE_SPEED:     return "HOST_DETECT_DEVICE_SPEED";
    case HOST_ENUMERATION:             return "HOST_ENUMERATION";
    case HOST_CLASS_REQUEST:           return "HOST_CLASS_REQUEST";
    case HOST_INPUT:                   return "HOST_INPUT";
    case HOST_SET_CONFIGURATION:       return "HOST_SET_CONFIGURATION";
    case HOST_SET_WAKEUP_FEATURE:      return "HOST_SET_WAKEUP_FEATURE";
    case HOST_CHECK_CLASS:             return "HOST_CHECK_CLASS";
    case HOST_CLASS:                   return "HOST_CLASS";
    case HOST_SUSPENDED:               return "HOST_SUSPENDED";
    case HOST_ABORT_STATE:             return "HOST_ABORT_STATE";
    default:                           return "HOST_STATE_UNKNOWN";
  }
}

static const char* usb_enum_state_to_str(ENUM_StateTypeDef state)
{
  switch (state)
  {
    case ENUM_IDLE:                     return "ENUM_IDLE";
    case ENUM_GET_FULL_DEV_DESC:        return "ENUM_GET_FULL_DEV_DESC";
    case ENUM_SET_ADDR:                 return "ENUM_SET_ADDR";
    case ENUM_GET_CFG_DESC:             return "ENUM_GET_CFG_DESC";
    case ENUM_GET_FULL_CFG_DESC:        return "ENUM_GET_FULL_CFG_DESC";
    case ENUM_GET_MFC_STRING_DESC:      return "ENUM_GET_MFC_STRING_DESC";
    case ENUM_GET_PRODUCT_STRING_DESC:  return "ENUM_GET_PRODUCT_STRING_DESC";
    case ENUM_GET_SERIALNUM_STRING_DESC:return "ENUM_GET_SERIALNUM_STRING_DESC";
    default:                            return "ENUM_STATE_UNKNOWN";
  }
}
/* USER CODE END 0 */

/*
 * user callback declaration
 */
static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id);

/*
 * -- Insert your external function declaration here --
 */
/* USER CODE BEGIN 1 */
static const char* usb_host_speed_to_str(USBH_SpeedTypeDef speed);
static const char* usb_class_to_str(uint8_t class_code);
static void log_string_descriptor(USBH_HandleTypeDef *phost, uint8_t index, const char *label);
static void log_connected_device_info(USBH_HandleTypeDef *phost);
static const char* usb_host_state_to_str(HOST_StateTypeDef state);
static const char* usb_enum_state_to_str(ENUM_StateTypeDef state);
/* USER CODE END 1 */

/**
  * Init USB host library, add supported class and start the library
  * @retval None
  */
void MX_USB_HOST_Init(void)
{
  /* USER CODE BEGIN USB_HOST_Init_PreTreatment */
  /* USER CODE END USB_HOST_Init_PreTreatment */

  /* Init host Library, add supported class and start the library. */
  if (USBH_Init(&hUsbHostHS, USBH_UserProcess, HOST_HS) != USBH_OK)
  {
			int errorvalue = 1;
			if (errorvalue == 1){
				Error_Handler();
			}
  }

  /* if (USBH_RegisterClass(&hUsbHostHS, USBH_AUDIO_CLASS) != USBH_OK)
  {
    Error_Handler();
  }*/

  if (USBH_RegisterClass(&hUsbHostHS, USBH_CDC_CLASS) != USBH_OK)
  {
			int errorvalue = 2;
			if (errorvalue == 2){
				Error_Handler();
			}
  }

  if (USBH_RegisterClass(&hUsbHostHS, USBH_MSC_CLASS) != USBH_OK)
  {
    Error_Handler();
  }

  /*
  if (USBH_RegisterClass(&hUsbHostHS, USBH_HID_CLASS) != USBH_OK)
  {
  	int errorvalue = 3;
  	if (errorvalue == 3){
			Error_Handler();
		}
  }
  */
  /*if (USBH_RegisterClass(&hUsbHostHS, USBH_MTP_CLASS) != USBH_OK)
  {
    Error_Handler();
  }*/

  if (USBH_Start(&hUsbHostHS) != USBH_OK)
  {
			int errorvalue = 4;
			if (errorvalue == 4){
				Error_Handler();
			}
  }
  /* USER CODE BEGIN USB_HOST_Init_PostTreatment */
  HMI_addSystemMessage("USB Host stack started (HS PHY): registered CDC, MSC, HID");
  /* USER CODE END USB_HOST_Init_PostTreatment */
}

/*
 * Background task
 */
void MX_USB_HOST_Process(void)
{
  /* USB Host Background task */
  USBH_Process(&hUsbHostHS);

  static HOST_StateTypeDef last_host_state = (HOST_StateTypeDef)(-1);
  static ENUM_StateTypeDef last_enum_state = (ENUM_StateTypeDef)(-1);

  if (hUsbHostHS.gState != last_host_state)
  {
    last_host_state = hUsbHostHS.gState;
    char msg[64];
    snprintf(msg, sizeof(msg), "USBH state -> %s", usb_host_state_to_str(last_host_state));
    HMI_addSystemMessage(msg);
  }

  if (hUsbHostHS.EnumState != last_enum_state)
  {
    last_enum_state = hUsbHostHS.EnumState;
    char msg[64];
    snprintf(msg, sizeof(msg), "Enum state -> %s", usb_enum_state_to_str(last_enum_state));
    HMI_addSystemMessage(msg);
  }
}
/*
 * user callback definition
 */
static void USBH_UserProcess  (USBH_HandleTypeDef *phost, uint8_t id)
{
  /* USER CODE BEGIN CALL_BACK_1 */
  switch(id)
  {
  case HOST_USER_SELECT_CONFIGURATION:
  break;

  case HOST_USER_DISCONNECTION:
  Appli_state = APPLICATION_DISCONNECT;
  HMI_addSystemMessage("USB device disconnected");
  break;

  case HOST_USER_CLASS_ACTIVE:
  Appli_state = APPLICATION_READY;
  HMI_addSystemMessage("USB device enumerated (class ready)");
  log_connected_device_info(phost);
  break;

  case HOST_USER_CONNECTION:
  Appli_state = APPLICATION_START;
  HMI_addSystemMessage("USB device connected, enumerating...");
  break;

  default:
  break;
  }
  /* USER CODE END CALL_BACK_1 */
}

/**
  * @}
  */

/**
  * @}
  */

