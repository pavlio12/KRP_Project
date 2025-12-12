/*
 * usb_task.c
 *
 *  Created on: Oct 28, 2025
 *      Author: Ondrej Pavlin
 */
#include "usb_task.h"
#include "usb_fsm.h"
#include "usbd_cdc_if.h"

#include "usb_device.h"
#include "usbd_core.h"
#include "usb_host.h"
#include "usbh_core.h"

// For Macros
#include "stm32h7xx_hal_rcc.h"


#include "hmiBridge.h"
#include "cmsis_os.h"
#include "main.h"

volatile DRD_Mode_t g_usb_role;
volatile uint8_t g_role_switch_requested = 0;   // Set by button IRQ
// volatile tells the compiler: This value may change at ANY time, outside normal program flow. Do NOT optimize it!

// Function pointer used by OTG_HS_IRQHandler
void (*USBHS_IRQHandler_Func)(void) = 0;

extern USB_FSM g_usb;
extern USBD_HandleTypeDef hUsbDeviceHS;
extern USBH_HandleTypeDef hUsbHostHS;

void USBHS_IRQHandler_HOST(void)
{
    HAL_HCD_IRQHandler(&hhcd_USB_OTG_HS);
}

void USBHS_IRQHandler_DEVICE(void)
{
    HAL_PCD_IRQHandler(&hpcd_USB_OTG_HS);
}

/*
static void DRD_BackupDomainInit(void)
{
	// On H7 there is no __HAL_RCC_PWR_CLK_ENABLE macro; PWR is already accessible.
	// Just unlock backup domain and enable RTC APB clock.

	HAL_PWR_EnableBkUpAccess();        // unlock backup-domain writes

}*/


void USB_Device_Task(void *argument)
{
		HMI_setUsbRoleText("Device");
		g_usb.rxQ = osMessageQueueNew(4, 64, NULL); // 4×64B messages
		HMI_setUsbStateText(USB_GetStateString());
		HMI_addUsbStateGraphPoint(USB_GetStateID()); // Show initial USB state

		for (;;) {
				if (g_usb_role == DRD_MODE_DEVICE) {
						usb_do_actions();       // Do current-state actions
						usb_eval_transitions(); // Compute transitions + Update HMI screen
				}
				osDelay(10);                // 100 Hz update rate;
		}
}

void USB_Host_Task(void *argument)
{
	HMI_setUsbRoleText("Host");
  for (;;) {
  		/* Use the logging wrapper to see host/enumeration state transitions */
      MX_USB_HOST_Process();
      osDelay(1);                   // 1 kHz update rate; Host needs fast polling
  }
}


// USB Dual-Role-Device Task
void USB_DRD_Task(void *argument)
{
		DRD_BackupDomainInit();
		DRD_Mode_t boot_mode = DRD_ReadBootMode();

		osThreadId_t usbDeviceTaskHandle = NULL;
		osThreadId_t usbHostTaskHandle = NULL;

		if (boot_mode == DRD_MODE_HOST)
		{
				// Show on Touch-Screen
				HMI_addSystemMessage("Booting in USB Host mode");
				HMI_setUsbRoleText("Host");

				// 1) tell IRQ which handler to use
				USBHS_IRQHandler_Func = USBHS_IRQHandler_HOST;
				g_usb_role = DRD_MODE_HOST;

				// 2) then init host stack (enables IRQ)
				MX_USB_HOST_Init();

				// 3) Create Host task
				const osThreadAttr_t usbHostTask_attributes = {
						.name = "usbHostTask",
						.priority = osPriorityHigh,
						.stack_size = 1024 * 8
				};
				usbHostTaskHandle = osThreadNew(USB_Host_Task, NULL, &usbHostTask_attributes);
		}
		else // if (boot_mode == DRD_MODE_DEVICE)
		{
				// Show on Touch-Screen
				HMI_addSystemMessage("Booting in USB Device mode");
				HMI_setUsbRoleText("Device");

				// 1) tell IRQ which handler to use
				USBHS_IRQHandler_Func = USBHS_IRQHandler_DEVICE;
				g_usb_role = DRD_MODE_DEVICE;

				// 2) then init device stack (enables IRQ)
				MX_USB_DEVICE_Init();

				// 3) Create Device task
				const osThreadAttr_t usbDeviceTask_attributes = {
						.name = "usbDeviceTask",
						.priority = osPriorityNormal,
						.stack_size = 1024 * 8
				};
				usbDeviceTaskHandle = osThreadNew(USB_Device_Task, NULL, &usbDeviceTask_attributes);
		}

		// ---- DRD monitoring loop ----
    for (;;)
    {
        if (g_role_switch_requested)
        {
            g_role_switch_requested = 0;
            HMI_addSystemMessage("DRD: requesting role switch, system reset...");
            HMI_setUsbRoleText(">>>");
            // osDelay(100);               // Delay to show the messages on screen

            DRD_RequestModeSwitch();   // SHOULD NEVER RETURN
        }

        osDelay(10);
    }
}

void DRD_RequestModeSwitch(void)
{
    DRD_Mode_t current = DRD_ReadBootMode();
    DRD_Mode_t next = (current == DRD_MODE_DEVICE) ? DRD_MODE_HOST : DRD_MODE_DEVICE;

    // Write next mode
    DRD_MODE_REG = (uint32_t)next;
    DRD_MAGIC_REG = DRD_MAGIC_VALUE;

    // Ensure write completed
    __DSB();
    __ISB();

    __disable_irq();
    HAL_NVIC_SystemReset();
}




DRD_Mode_t DRD_ReadBootMode(void)
{
    // If no magic → default to DEVICE
    if (DRD_MAGIC_REG != DRD_MAGIC_VALUE)
    {
        DRD_MODE_REG = DRD_MODE_DEVICE;
        DRD_MAGIC_REG = DRD_MAGIC_VALUE;
        return DRD_MODE_DEVICE;
    }

    uint32_t mode = DRD_MODE_REG;

    if (mode == DRD_MODE_HOST)
        return DRD_MODE_HOST;

    // Default fallback
    return DRD_MODE_DEVICE;
}


void DRD_BackupDomainInit(void)
{
    // Enable access to backup registers
   //  __HAL_RCC_PWR_CLK_ENABLE();
    HAL_PWR_EnableBkUpAccess();

    // Enable RTC APB clock (required for BKPxR access)
    // __HAL_RCC_RTCAPB_CLK_ENABLE();

    // Enable LSI so RTC domain is stable (LSE unnecessary)
    __HAL_RCC_LSI_ENABLE();
    while (!__HAL_RCC_GET_FLAG(RCC_FLAG_LSIRDY))
        ;
}




void display_USB_state(bool force_update) {
	// char msg[64];
	// snprintf(msg, sizeof(msg), "USB State: %s", USB_GetStateString());
	// HMI_addSystemMessage(msg);

	HMI_addUsbStateGraphPoint(USB_GetStateID());

}


