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

#include "hmiBridge.h"
#include "cmsis_os.h"
#include "main.h"

volatile DRD_RoleTypeDef g_usb_role;
volatile uint8_t g_role_switch_requested = 0;   // Set by button IRQ
// volatile tells the compiler: This value may change at ANY time, outside normal program flow. Do NOT optimize it!

extern USB_FSM g_usb;
extern USBD_HandleTypeDef hUsbDeviceHS;
extern USBH_HandleTypeDef hUsbHostHS;


void USB_Task(void *argument)
{
		g_usb.rxQ = osMessageQueueNew(4, 64, NULL); // 4×64B messages
		HMI_addUsbStateGraphPoint(USB_GetStateID()); // Show initial USB state

		for (;;) {
				usb_do_actions();       // Do current-state actions
				usb_eval_transitions(); // Compute transitions + Update HMI screen
				osDelay(10);            // 100 Hz update rate;
		}
}


// Start USB Dual-Role-Device Task
void USB_DRD_Task(void *argument)
{
    // Start in Device mode
    MX_USB_DEVICE_Init();
    g_usb_role = DRD_ROLE_DEVICE;

    for (;;)
    {
        // Role change requested by button IRQ
        if (g_role_switch_requested)
        {
            g_role_switch_requested = 0;

            if (g_usb_role == DRD_ROLE_DEVICE)
            {
                DRD_SwitchToHost();
            }
            else
            {
                DRD_SwitchToDevice();
            }
        }

        // When we are host, we must pump the host state machine
        if (g_usb_role == DRD_ROLE_HOST)
        {
            USBH_Process(&hUsbHostHS);
        }

        osDelay(10);
    }
}

void DRD_SwitchToHost(void)
{
		HMI_addSystemMessage("Switching to USB Host...");
    // 1) Stop & deinit USBD (PCD owns the core in device mode)
    USBD_Stop(&hUsbDeviceHS);
    USBD_DeInit(&hUsbDeviceHS);

    // 2) Short settle time so PHY/bus go idle
    osDelay(100);

    // 3) Manually reinitialize USBH_HandleTypeDef
    Reset_USBH_Handle(&hUsbHostHS);

		osDelay(10);

    // 4) Init host stack (HCD takes ownership)
    MX_USB_HOST_Init();

    g_usb_role = DRD_ROLE_HOST;
    HMI_addSystemMessage("Switched to USB Host");
}

void DRD_SwitchToDevice(void)
{
		HMI_addSystemMessage("Switching to USB Device...");
    // 1) Stop & deinit USBH (HCD owns the core in host mode)
    // USBH_Stop(&hUsbHostHS); Caused runtime errors
    USBH_DeInit(&hUsbHostHS);

    // 2) Short settle time
    osDelay(100);

    // 3) Init device stack again
    MX_USB_DEVICE_Init();

    g_usb_role = DRD_ROLE_DEVICE;
    HMI_addSystemMessage("Switched to USB Device");
}

void Reset_USBH_Handle(USBH_HandleTypeDef *phost)
{
    memset(phost, 0, sizeof(USBH_HandleTypeDef));
}



void display_USB_state(bool force_update) {
	// char msg[64];
	// snprintf(msg, sizeof(msg), "USB State: %s", USB_GetStateString());
	// HMI_addSystemMessage(msg);

	HMI_addUsbStateGraphPoint(USB_GetStateID());

}


