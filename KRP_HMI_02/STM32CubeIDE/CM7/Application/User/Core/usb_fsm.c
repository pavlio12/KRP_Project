/*
 * usb_fsm.c
 *
 *  Created on: Oct 30, 2025
 *      Author: Ondrej Pavlin
 */

#include "usb_fsm.h"
#include "usbd_cdc_if.h"
#include "hmiBridge.h"


USB_FSM g_usb = { .state = USB_LS_INIT,
									.prev = USB_LS_INIT,
									.last_hb_tick = 0,
									.activity = {0},
									.rxQ = NULL };

static inline uint32_t now_ms(void) { return osKernelGetTickCount(); }

extern USBD_HandleTypeDef hUsbDeviceHS;

static __attribute__((aligned(4))) uint8_t txbuf[64];
static uint8_t rxbuf[64];
static char msg[64];
static bool hb_pending = false;
static uint16_t hb_len = 0;
static uint32_t last_tx_attempt = 0;

// Top-level actions dispatcher (called every RTOS tick of the USB task)
void usb_do_actions(void) {
    switch (g_usb.state) {
        case USB_LS_INIT:       do_INIT();       break;
        case USB_LS_ADDRESSED:  do_ADDRESSED();  break;
        case USB_LS_CONFIGURED: do_CONFIGURED(); break;
        case USB_LS_SUSPENDED:  do_SUSPENDED();  break;
        default:                do_INIT();       break;
    }
}

void do_INIT(void) {
    // nothing active; wait for enumeration to progress
}

void do_ADDRESSED(void) {
    // EP0 in use; nothing class-level to send here
}

void do_CONFIGURED(void) {
    // Heartbeat message once per second if TX not busy
	  // TODO: Is this approach correct?
    uint32_t t = now_ms();
    if ((t - g_usb.last_hb_tick) >= 1000) {
        g_usb.last_hb_tick = t;

        hb_len = (uint16_t)snprintf(txbuf, sizeof(txbuf), "Hello from STM %lu\r\n", t);
        hb_pending = true;
    }

    // Send when pending; retry on BUSY with backoff, no guard clearing
    if (hb_pending && (t - last_tx_attempt) >= 10) { // 10ms retry spacing
        last_tx_attempt = t;
        uint8_t res = CDC_Transmit_HS((uint8_t*)txbuf, hb_len);
        if (res == USBD_OK) {
            g_usb.activity.tx_busy = true;
            hb_pending = false;
        } else if (res == USBD_BUSY) {
            // stay pending, retry later
        } else { // FAIL or not configured
            hb_pending = false;
            g_usb.activity.tx_busy = false;
        }
    }

    // Drain RX queue (non-blocking)
    if (g_usb.rxQ && osMessageQueueGet(g_usb.rxQ, rxbuf, NULL, 0) == osOK) {
        // Do something with msg (parse commands, update HMI, etc.)
    		// TODO: Check if the rxbuf is safe to print and terminated with \0
        HMI_addSystemMessage((char*)rxbuf);
        g_usb.activity.rx_ready = true;            // pulse for graph
    }
}

void do_SUSPENDED(void) {
    // low-power friendly area; no SOF; do nothing except minimal housekeeping
}


// Map the core USBD state to our link state (FSM)
USB_LinkState classify_link_state(void) {
    switch (hUsbDeviceHS.dev_state) {
        case USBD_STATE_DEFAULT:    return USB_LS_INIT;
        case USBD_STATE_ADDRESSED:  return USB_LS_ADDRESSED;
        case USBD_STATE_CONFIGURED: return USB_LS_CONFIGURED;
        case USBD_STATE_SUSPENDED:  return USB_LS_SUSPENDED;
        default:                    return USB_LS_INIT;
    }
}

void usb_eval_transitions(void) {
		bool hmi_updated = false;
    USB_LinkState next = classify_link_state();
    if (next != g_usb.state) {
        g_usb.prev = g_usb.state;
        g_usb.state   = next;

				snprintf(msg, sizeof(msg), "USBD state -> %s", USB_GetStateString());
				HMI_addSystemMessage(msg);

        hmi_updated = true;
        // housekeeping on entry
        if (g_usb.state != USB_LS_CONFIGURED) {
            g_usb.activity.tx_busy = false; // clear activity flags if we left CONFIGURED
        }
    }

    // Emit TX/RX pulses for the plot without changing link state
    if (g_usb.activity.tx_done) { // TX pulse
        hmi_updated = true;
        g_usb.activity.tx_done = false;
    }
    if (g_usb.activity.rx_ready) { // RX pulse
        hmi_updated = true;
        g_usb.activity.rx_ready = false;
    }

    if (hmi_updated) {
    	HMI_setUsbStateText(USB_GetStateString());
    	HMI_addUsbStateGraphPoint(USB_GetStateID());
    }


}


const char* USB_GetStateString(void)
{
    switch (g_usb.state)
    {
        case USB_LS_INIT:                   return "INIT";
        case USB_LS_ADDRESSED:              return "ADDRESSED";
        case USB_LS_CONFIGURED:
        	if (g_usb.activity.tx_busy)       return "TX BUSY";
        	else if (g_usb.activity.tx_done)  return "TX DONE";
        	else if (g_usb.activity.rx_ready) return "RX READY";
        	else                              return "CONFIGURED";
        case USB_LS_SUSPENDED:              return "SUSPENDED";
        default:                            return "UNKNOWN";
    }
}

const uint8_t USB_GetStateID(void)
{
		switch (g_usb.state)
		{
				case USB_LS_INIT:                   return 0;
				case USB_LS_ADDRESSED:              return 1;
				case USB_LS_CONFIGURED:
					if (g_usb.activity.tx_busy)       return 3;
					else if (g_usb.activity.tx_done)  return 4;
					else if (g_usb.activity.rx_ready) return 5;
					else                              return 2;  // 2 here because of graph
				case USB_LS_SUSPENDED:              return 6;
				default:                            return 7;
		}
}
