/*
 * usb_task.c
 *
 *  Created on: Oct 28, 2025
 *      Author: Ondrej Pavlin
 */

#pragma once
#include "usbd_def.h"
#include "usbh_def.h"
#include "usbd_conf.h"   // for hpcd_USB_OTG_HS
#include "usbh_conf.h"   // for hhcd_USB_OTG_HS
#include "cmsis_os2.h"

#include "stm32h7xx_hal.h"
// #include "stm32h7xx_hal_pcd.h"
// #include "stm32h7xx_hal_hcd.h"
#include <stdbool.h>

#include "stm32h7xx.h"   // for TAMP, PWR, etc.

/* Use backup registers instead of backup SRAM */
#define DRD_MAGIC_VALUE      0xA5A55A5AUL
#define DRD_MAGIC_REG        (RTC->BKP0R)   // 32-bit backup register 0
#define DRD_MODE_REG         (RTC->BKP1R)   // 32-bit backup register 1

// Selectable IRQ handler for OTG HS
extern void (*USBHS_IRQHandler_Func)(void);
void USBHS_IRQHandler_HOST(void);
void USBHS_IRQHandler_DEVICE(void);

typedef enum {
  USB_EVT_CONNECT,
  USB_EVT_DISCONNECT,
  USB_EVT_PORT_EN,
  USB_EVT_PORT_DIS,
	USB_EVT_PORT_RESET,
	USB_EVT_VBUS_ON,
	USB_EVT_VBUS_OFF
} usb_evt_t;
extern osMessageQueueId_t g_usbEvtQ;

typedef enum {
    DRD_MODE_DEVICE = 0,
    DRD_MODE_HOST   = 1
} DRD_Mode_t;
extern volatile DRD_Mode_t g_usb_role;
extern volatile uint8_t g_role_switch_requested;   // Set by button IRQ

extern PCD_HandleTypeDef hpcd_USB_OTG_HS;
extern HCD_HandleTypeDef hhcd_USB_OTG_HS;

DRD_Mode_t DRD_ReadBootMode(void);
void DRD_RequestModeSwitch(void);
void DRD_BackupDomainInit(void);


void USB_Device_Task(void *argument);
void USB_Host_Task(void *argument);

void USB_OnRx(uint8_t *buf, uint32_t len);
void USB_SendString(const char* str);

void USB_HandleConfigured(void);
void USB_HandleRx(void);
void USB_HandleError(void);

void USB_DRD_Task(void *argument);

void display_USB_state(bool force_update);



