/*
 * usb_task.c
 *
 *  Created on: Oct 28, 2025
 *      Author: Ondrej Pavlin
 */

#pragma once
#include "usbd_def.h"
#include <stdbool.h>

void USB_Task(void *argument);
void USB_OnRx(uint8_t *buf, uint32_t len);
void USB_SendString(const char* str);

void USB_HandleConfigured(void);
void USB_HandleRx(void);
void USB_HandleError(void);

void display_USB_state(bool force_update);



