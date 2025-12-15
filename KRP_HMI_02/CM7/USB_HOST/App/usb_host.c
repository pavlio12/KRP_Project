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
#include "stm32h7xx_ll_usb.h"

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

#define USB_ENUM_STALL_TIMEOUT 20000U    // [ms] recovery trigger (only when control idle)

/* Deferred string descriptor logging state */
static __ALIGNED(32) uint8_t usb_string_buf[64];
static uint8_t string_log_stage = 0U; // 0=idle, 1=manufacturer, 2=product, 3=serial

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

static void log_connected_device_info(USBH_HandleTypeDef *phost)
{
  const USBH_DevDescTypeDef *dev = &phost->device.DevDesc;
  const USBH_CfgDescTypeDef *cfg = &phost->device.CfgDesc;

  char msg[128];
  snprintf(msg, sizeof(msg), "VID:PID 0x%04X:0x%04X | USB %x.%02x | %s", dev->idVendor,
           dev->idProduct, (dev->bcdUSB >> 8) & 0xFF, dev->bcdUSB & 0xFF,
           usb_host_speed_to_str(phost->device.speed));
  HMI_addSystemMessage(msg);
  HMI_setDeviceInfo(msg);

  snprintf(msg, sizeof(msg), "Device class 0x%02X (%s), subclass 0x%02X, protocol 0x%02X",
           dev->bDeviceClass, usb_class_to_str(dev->bDeviceClass), dev->bDeviceSubClass,
           dev->bDeviceProtocol);
  HMI_addSystemMessage(msg);
  HMI_appendDeviceInfo(msg);

  snprintf(msg, sizeof(msg), "Configuration %u: %u interface(s), %u mA max",
           cfg->bConfigurationValue, cfg->bNumInterfaces,
           (unsigned int)(cfg->bMaxPower * 2U));
  HMI_addSystemMessage(msg);
  HMI_appendDeviceInfo(msg);

  if (cfg->bNumInterfaces > 0U)
  {
    const USBH_InterfaceDescTypeDef *itf = &cfg->Itf_Desc[phost->device.current_interface];
    snprintf(msg, sizeof(msg), "Interface 0 class 0x%02X (%s), subclass 0x%02X, protocol 0x%02X",
             itf->bInterfaceClass, usb_class_to_str(itf->bInterfaceClass),
             itf->bInterfaceSubClass, itf->bInterfaceProtocol);
    HMI_addSystemMessage(msg);
    HMI_appendDeviceInfo(msg);
  }
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

static void log_enum_stuck_detail(uint32_t elapsed_ms)
{
  USBH_URBStateTypeDef urb_out = USBH_LL_GetURBState(&hUsbHostHS, hUsbHostHS.Control.pipe_out);
  USBH_URBStateTypeDef urb_in  = USBH_LL_GetURBState(&hUsbHostHS, hUsbHostHS.Control.pipe_in);

  uint32_t hc_err = 0;
  uint8_t hc_out_state = 0xFF;
  uint8_t hc_in_state = 0xFF;
  uint32_t hprt = 0;
  uint32_t haint = 0;
  uint32_t gintsts = 0;
  uint32_t hcfg = 0;
  uint32_t gahbcfg = 0;
  uint32_t gotgctl = 0;
  uint32_t usb_base = 0;
  uint32_t hcd_state = 0;

  if (hUsbHostHS.pData != NULL)
  {
    HCD_HandleTypeDef *hhcd = (HCD_HandleTypeDef *)hUsbHostHS.pData;
    hc_err = hhcd->ErrorCode;

    if (hUsbHostHS.Control.pipe_out < 16U)
    {
      hc_out_state = (uint8_t)hhcd->hc[hUsbHostHS.Control.pipe_out].state;
    }
    if (hUsbHostHS.Control.pipe_in < 16U)
    {
      hc_in_state = (uint8_t)hhcd->hc[hUsbHostHS.Control.pipe_in].state;
    }

    /* Read host/global registers directly from the current instance base */
    USB_OTG_GlobalTypeDef *USBx = hhcd->Instance;
    usb_base = (uint32_t)USBx;
    hcd_state = (uint32_t)hhcd->State;
    if (USBx != NULL)
    {
      gotgctl = *(__IO uint32_t *)(usb_base + 0x000U);                   /* GOTGCTL */
      gahbcfg = *(__IO uint32_t *)(usb_base + 0x008U);                   /* GAHBCFG */
      hprt    = *(__IO uint32_t *)(usb_base + USB_OTG_HOST_PORT_BASE);   /* HPRT */
      hcfg    = *(__IO uint32_t *)(usb_base + USB_OTG_HOST_BASE + 0x00U);/* HCFG */
      haint   = *(__IO uint32_t *)(usb_base + USB_OTG_HOST_BASE + 0x14U);/* HAINT */
      gintsts = USBx->GINTSTS;                                           /* GINTSTS */
    }
  }

  char msg[256];
  if (DEBUG_PRINTS == 1U) {
    // We switch the message order because the messages are prepend, not append
    // Third message
    snprintf(msg, sizeof(msg),
                                    "...HPRT=0x%08lX HAINT=0x%08lX GINTSTS=0x%08lX",
                                    (unsigned long)hprt,
                                    (unsigned long)haint,
                                    (unsigned long)gintsts);
        HMI_addSystemMessage(msg);

        // Second message
    snprintf(msg, sizeof(msg),
                    "...hc_out=%u hc_in=%u err=0x%lX...",
                    (unsigned int)hc_out_state,
                    (unsigned int)hc_in_state,
                    (unsigned long)hc_err);
    HMI_addSystemMessage(msg);

    // First message
    snprintf(msg, sizeof(msg),
                                    "ENUM stalled %lums: req=%u ctrl=%u enum=%u urb_out=%d urb_in=%d...",
                                    (unsigned long)elapsed_ms,
                                    (unsigned int)hUsbHostHS.RequestState,
                                    (unsigned int)hUsbHostHS.Control.state,
                                    (unsigned int)hUsbHostHS.EnumState,
                                    urb_out,
                                    urb_in);
        HMI_addSystemMessage(msg);
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
static void log_connected_device_info(USBH_HandleTypeDef *phost);
static const char* usb_host_state_to_str(HOST_StateTypeDef state);
static const char* usb_enum_state_to_str(ENUM_StateTypeDef state);
static void log_enum_stuck_detail(uint32_t elapsed_ms);
static void process_deferred_string_logs(void);
/* USER CODE END 1 */

/**
  * Init USB host library, add supported class and start the library
  * @retval None
  */
void MX_USB_HOST_Init(void)
{
  /* USER CODE BEGIN USB_HOST_Init_PreTreatment */
  /* Force-reset the OTG HS + ULPI block to mimic a cold power-up */
  __HAL_RCC_USB_OTG_HS_FORCE_RESET();
  HAL_Delay(100);
  __HAL_RCC_USB_OTG_HS_RELEASE_RESET();
  HAL_Delay(100);

  /* Power-cycle VBUS before starting the host stack */
  (void)USBH_LL_DriverVBUS(&hUsbHostHS, 0U);
  USBH_Delay(250);
  (void)USBH_LL_DriverVBUS(&hUsbHostHS, 1U);
  USBH_Delay(250);
  /* USER CODE END USB_HOST_Init_PreTreatment */

  /* Init host Library, add supported class and start the library. */
  if (USBH_Init(&hUsbHostHS, USBH_UserProcess, HOST_HS) != USBH_OK)
  {
			int errorvalue = 1;
			if (errorvalue == 1){
				Error_Handler();
			}
  }

  if (USBH_RegisterClass(&hUsbHostHS, USBH_AUDIO_CLASS) != USBH_OK)
  {
  		int errorvalue = 2;
			if (errorvalue == 2){
				Error_Handler();
			}
  }

  if (USBH_RegisterClass(&hUsbHostHS, USBH_CDC_CLASS) != USBH_OK)
  {
			int errorvalue = 3;
			if (errorvalue == 3){
				Error_Handler();
			}
  }

  if (USBH_RegisterClass(&hUsbHostHS, USBH_MSC_CLASS) != USBH_OK)
  {
			int errorvalue = 4;
			if (errorvalue == 4){
				Error_Handler();
			}
  }

  if (USBH_RegisterClass(&hUsbHostHS, USBH_HID_CLASS) != USBH_OK)
  {
			int errorvalue = 5;
			if (errorvalue == 5){
				Error_Handler();
			}
  }

  if (USBH_RegisterClass(&hUsbHostHS, USBH_MTP_CLASS) != USBH_OK)
  {
			int errorvalue = 6;
			if (errorvalue == 6){
				Error_Handler();
			}
  }

  if (USBH_Start(&hUsbHostHS) != USBH_OK)
  {
			int errorvalue = 7;
			if (errorvalue == 7){
				Error_Handler();
			}
  }
  /* USER CODE BEGIN USB_HOST_Init_PostTreatment */
  /* After VBUS is stable, enforce a port reset before enumeration */
  USBH_Delay(100);
  (void)USBH_LL_ResetPort(&hUsbHostHS);
  USBH_Delay(100);
  HMI_addSystemMessage("USB Host stack started (HS PHY)");
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
  static uint32_t enum_state_enter_ms = 0;
  static uint8_t reset_enum_debug = 0U;
  static uint8_t prev_req = 0xFF;
  static uint8_t prev_ctrl = 0xFF;
  static uint8_t prev_enum = 0xFF;
  static uint32_t enum_stuck_since = 0U;
  static uint8_t enum_stuck_reported = 0U;

  if (hUsbHostHS.gState != last_host_state)
  {
    last_host_state = hUsbHostHS.gState;
    char msg[64];
    snprintf(msg, sizeof(msg), "USBH state -> %s", usb_host_state_to_str(last_host_state));
    HMI_setUsbStateText(usb_host_state_to_str(last_host_state));
    HMI_addSystemMessage(msg);

    reset_enum_debug = (last_host_state == HOST_ENUMERATION) ? 1U : 0U;
  }

  if (hUsbHostHS.EnumState != last_enum_state)
  {
    last_enum_state = hUsbHostHS.EnumState;
    char msg[64];
    snprintf(msg, sizeof(msg), "Enum state -> %s", usb_enum_state_to_str(last_enum_state));
    HMI_addSystemMessage(msg);

    if (hUsbHostHS.gState == HOST_ENUMERATION)
    {
      enum_state_enter_ms = HAL_GetTick();
    }
  }

  /* If enumeration stalls too long, force a re-enumeration so re-plugging works reliably */
  if (hUsbHostHS.gState == HOST_ENUMERATION)
  {
    uint32_t now = HAL_GetTick();

    if (reset_enum_debug != 0U)
    {
      prev_req = 0xFF;
      prev_ctrl = 0xFF;
      prev_enum = 0xFF;
      enum_stuck_since = now;
      enum_stuck_reported = 0U;
      reset_enum_debug = 0U;
    }

    uint8_t changed = (hUsbHostHS.RequestState != prev_req) ||
                      (hUsbHostHS.Control.state != prev_ctrl) ||
                      (hUsbHostHS.EnumState != prev_enum);

    if (changed != 0U)
    {
      char dbg[64];
      snprintf(dbg, sizeof(dbg), "ENUM: req=%u ctrl=%u enum=%u",
               (unsigned)hUsbHostHS.RequestState,
               (unsigned)hUsbHostHS.Control.state,
               (unsigned)hUsbHostHS.EnumState);
      HMI_addSystemMessage(dbg);

      prev_req = hUsbHostHS.RequestState;
      prev_ctrl = hUsbHostHS.Control.state;
      prev_enum = hUsbHostHS.EnumState;
      enum_stuck_since = now;
      enum_stuck_reported = 0U;
    }
    else
    {
      if ((enum_stuck_reported == 0U) && (enum_stuck_since != 0U) &&
          ((now - enum_stuck_since) > 3000U)) // 3 seconds
      {
        log_enum_stuck_detail(now - enum_stuck_since);
        enum_stuck_reported = 1U;
      }
    }

    if (enum_state_enter_ms == 0)
    {
      enum_state_enter_ms = now;
    }
    /* Gated recovery: log but avoid forcing port/IRQ pokes here */
    if ((now - enum_state_enter_ms) > USB_ENUM_STALL_TIMEOUT)
    {
      enum_state_enter_ms = now;

      USBH_URBStateTypeDef urb_out = USBH_LL_GetURBState(&hUsbHostHS, hUsbHostHS.Control.pipe_out);
      USBH_URBStateTypeDef urb_in  = USBH_LL_GetURBState(&hUsbHostHS, hUsbHostHS.Control.pipe_in);
      char msg[112];
      snprintf(msg, sizeof(msg),
               "Enum stall timeout: state=%s req=%d ctrl=%d urb_out=%d urb_in=%d err=%d (no forced reset)",
               usb_enum_state_to_str(hUsbHostHS.EnumState),
               hUsbHostHS.RequestState,
               hUsbHostHS.Control.state,
               urb_out, urb_in,
               hUsbHostHS.Control.errorcount);
      HMI_addSystemMessage(msg);
    }
  }

  /* Handle deferred string descriptor logging outside of USBH_UserProcess */
  process_deferred_string_logs();
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
    string_log_stage = 1U; // defer string descriptor reads until control pipe is idle
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

/* USER CODE BEGIN 2 */
static void process_deferred_string_logs(void)
{
  if (string_log_stage == 0U)
  {
    return;
  }

  /* Only start a new control request when the engine is idle */
  if (hUsbHostHS.gState == HOST_ENUMERATION)
  {
    return;
  }
  if (hUsbHostHS.RequestState != CMD_SEND) // TODO: Is this correct approach? Investigate please!
  {
    return;
  }
  if ((hUsbHostHS.Control.state != CTRL_IDLE) && (hUsbHostHS.Control.state != CTRL_COMPLETE))
  {
    return;
  }

  uint8_t idx = 0U;
  const char *label = "";

  switch (string_log_stage)
  {
    case 1U:
      idx = hUsbHostHS.device.DevDesc.iManufacturer;
      label = "Manufacturer";
      break;
    case 2U:
      idx = hUsbHostHS.device.DevDesc.iProduct;
      label = "Product";
      break;
    case 3U:
      idx = hUsbHostHS.device.DevDesc.iSerialNumber;
      label = "Serial";
      break;
    default:
      string_log_stage = 0U;
      return;
  }

  /* Skip missing string indices */
  if (idx == 0U)
  {
    string_log_stage++;
    if (string_log_stage > 3U)
    {
      string_log_stage = 0U;
    }
    return;
  }

  USBH_StatusTypeDef st = USBH_Get_StringDesc(&hUsbHostHS, idx, usb_string_buf, sizeof(usb_string_buf));

  if (st == USBH_BUSY)
  {
    return;
  }

  if (st == USBH_OK)
  {
    char msg[90];
    snprintf(msg, sizeof(msg), "%s string: %s", label, (char *)usb_string_buf);
    HMI_addSystemMessage(msg);
    // HMI_appendDeviceInfo(msg);
  }
  else if (st == USBH_NOT_SUPPORTED)
  {
    char msg[90];
    snprintf(msg, sizeof(msg), "%s string not supported", label);
    HMI_addSystemMessage(msg);
    // HMI_appendDeviceInfo(msg);
  }
  else
  {
    char msg[90];
    snprintf(msg, sizeof(msg), "%s string read failed (%d)", label, st);
    HMI_addSystemMessage(msg);
    // HMI_appendDeviceInfo(msg);
  }

  string_log_stage++;
  if (string_log_stage > 3U)
  {
    string_log_stage = 0U;
  }
}
/* USER CODE END 2 */

/**
  * @}
  */

/**
  * @}
  */
