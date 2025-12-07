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


void USB_Task(void *argument)
{
		/*g_usb.rxQ = osMessageQueueNew(4, 64, NULL); // 4×64B messages
		HMI_addUsbStateGraphPoint(USB_GetStateID()); // Show initial USB state

		for (;;) {
				usb_do_actions();       // Do current-state actions
				usb_eval_transitions(); // Compute transitions + Update HMI screen
				osDelay(10);            // 100 Hz update rate;
		}*/
		for (;;) {
			osDelay(10);
		}
}


// USB Dual-Role-Device Task
void USB_DRD_Task(void *argument)
{
		DRD_BackupDomainInit();
		DRD_Mode_t boot_mode = DRD_ReadBootMode();

		if (boot_mode == DRD_MODE_HOST)
		{
				HMI_addSystemMessage("Booting in USB Host mode");

				// Show on Touch-Screen
				HMI_setUsbRoleText("Host");

				// 1) tell IRQ which handler to use
				USBHS_IRQHandler_Func = USBHS_IRQHandler_HOST;
				g_usb_role = DRD_MODE_HOST;

				// 2) then init host stack (enables IRQ)
				MX_USB_HOST_Init();
		}
		else
		{
				HMI_addSystemMessage("Booting in USB Device mode");

				// Show on Touch-Screen
				HMI_setUsbRoleText("Device");

				// 1) tell IRQ which handler to use
				USBHS_IRQHandler_Func = USBHS_IRQHandler_DEVICE;
				g_usb_role = DRD_MODE_DEVICE;

				// 2) then init device stack (enables IRQ)
				MX_USB_DEVICE_Init();
		}

		//A) Force HOST
		/*
		USBHS_IRQHandler_Func = USBHS_IRQHandler_HOST;
		g_usb_role = DRD_MODE_HOST;

		HMI_addSystemMessage("Force booting in Host mode");
		MX_USB_HOST_Init();
		*/

		// B) Force DEVICE
		/*
		HMI_addSystemMessage("Force Booting in Device mode");
		MX_USB_DEVICE_Init();
		g_usb_role = DRD_MODE_DEVICE;
		*/


    for (;;)
    {
        if (g_role_switch_requested)
        {
            g_role_switch_requested = 0;
            HMI_addSystemMessage("DRD: requesting role switch, system reset...");
            HMI_setUsbRoleText(">>>");
            osDelay(10);               // Delay to show the messages on screen
            DRD_RequestModeSwitch();   // SHOULD NEVER RETURN
        }

        if (g_usb_role == DRD_MODE_HOST)
        {
            /* Use the logging wrapper to see host/enumeration state transitions */
            MX_USB_HOST_Process();
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


