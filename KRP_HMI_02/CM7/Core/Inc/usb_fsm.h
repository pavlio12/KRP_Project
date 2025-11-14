/*
 * usb_fsm.h
 *
 *  Created on: Oct 30, 2025
 *      Author: Ondrej Pavlin
 *
 *  FSM - Finite State Machine. Deterministic behaviour of the USB application layer
 */

#pragma once
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "cmsis_os2.h"

typedef enum {
    USB_LS_INIT = 0,        // after reset/detach; no address
    USB_LS_ADDRESSED,       // host assigned address, not configured
    USB_LS_CONFIGURED,      // endpoints open, class ready
    USB_LS_SUSPENDED        // host suspended (no SOF)
} USB_LinkState;

// one-shot activity pulses for visualization / logic
typedef struct {
    volatile bool tx_done;      // set in TX-complete callback
    volatile bool rx_ready;     // set in RX callback
    volatile bool tx_busy;      // TX in flight (prevents double-send)
} USB_Activities;

typedef struct {
    USB_LinkState  state;         // current link state
    USB_LinkState  prev;          // previous link state
    uint32_t       last_hb_tick;  // last heartbeat tick
    USB_Activities activity;      // activity flags
    osMessageQueueId_t rxQ;       // queue of received CDC frames
} USB_FSM;

extern USB_FSM g_usb;

// functions

void usb_do_actions(void);
void do_INIT(void);
void do_ADDRESSED(void);
void do_CONFIGURED(void);
void do_SUSPENDED(void);

USB_LinkState classify_link_state(void);

void usb_eval_transitions(void);

const char* USB_GetStateString(void);
const uint8_t USB_GetStateID(void);


