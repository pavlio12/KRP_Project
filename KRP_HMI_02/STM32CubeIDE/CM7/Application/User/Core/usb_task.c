/*
 * usb_task.c
 *
 *  Created on: Oct 28, 2025
 *      Author: Ondrej Pavlin
 */


#include "usb_task.h"
#include "usb_fsm.h"
#include "usbd_cdc_if.h"

#include "hmiBridge.h"
#include "cmsis_os.h"
#include "main.h"

extern USB_FSM g_usb;
extern USBD_HandleTypeDef hUsbDeviceHS;

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



void display_USB_state(bool force_update) {
	// char msg[64];
	// snprintf(msg, sizeof(msg), "USB State: %s", USB_GetStateString());
	// HMI_addSystemMessage(msg);

	HMI_addUsbStateGraphPoint(USB_GetStateID());

}


