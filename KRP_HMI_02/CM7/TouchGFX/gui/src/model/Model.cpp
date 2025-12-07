#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

#include "main.h"
#include "string.h"
#include <stdio.h>

// static uint32_t ModelCounter = 0;


Model::Model() : modelListener(0)
{

}

void Model::tick()
{
    /*// Works nicely. Runs about 60Hz
		char buffer[64];
    snprintf(buffer, sizeof(buffer), "Model tick message %lu", ModelCounter++);
    modelListener->setSystemMessage(buffer);
    */

		// USB Role Text
		if (hasNewUsbRole && modelListener) {
				modelListener->setUsbRoleText(pendingUsbRole); // Add to the beginning of TextArea
				hasNewUsbRole = false;
		}

		// USB State Text
		if (hasNewUsbState && modelListener) {
				modelListener->setUsbStateText(pendingUsbState); // Add to the beginning of TextArea
				hasNewUsbState = false;
		}

		// Log System Messages - Add to the beginning (Prepend instead of Append)
		if (sysMsgCount > 0 && modelListener) {
        // Pop oldest
        const char* msg = pendingMessages[sysMsgTail];
        sysMsgTail = (uint8_t)((sysMsgTail + 1) % SYSMSG_QUEUE_DEPTH);
        sysMsgCount--;

        modelListener->prependSystemMessage(msg);
    }

		// Add the Usb State as a new Point to Graph
		if (hasNewUsbGraphPoint && modelListener) {
				modelListener->addUsbStateGraphPoint(pendingUsbGraphPoint);
				hasNewUsbGraphPoint = false;
		}

}


void Model::setUsbRoleText(const char* msg) {
	strncpy(pendingUsbRole, msg, sizeof(pendingUsbRole) - 1);
	pendingUsbRole[sizeof(pendingUsbRole) - 1] = '\0';
	hasNewUsbRole = true;
}

void Model::setUsbStateText(const char* msg) {
	strncpy(pendingUsbState, msg, sizeof(pendingUsbState) - 1);
	pendingUsbState[sizeof(pendingUsbState) - 1] = '\0';
	hasNewUsbState = true;
}

void Model::addSystemMessage(const char* msg) {
	if (!msg) {
			return;
	}

	// If queue is full, drop the oldest to keep the most recent messages
	if (sysMsgCount == SYSMSG_QUEUE_DEPTH) {
			sysMsgTail = (uint8_t)((sysMsgTail + 1) % SYSMSG_QUEUE_DEPTH);
			sysMsgCount--;
	}

	char* slot = pendingMessages[sysMsgHead];
	strncpy(slot, msg, SYSMSG_TEXT_LEN - 1);
	slot[SYSMSG_TEXT_LEN - 1] = '\0';
	sysMsgHead = (uint8_t)((sysMsgHead + 1) % SYSMSG_QUEUE_DEPTH);
	sysMsgCount++;
}

void Model::addUsbStateGraphPoint(uint8_t stateValue) {
	pendingUsbGraphPoint = stateValue;
	hasNewUsbGraphPoint = true;

	if (usbHistoryCount < USB_GRAPH_HISTORY)
			usbHistory[usbHistoryCount++] = stateValue;
	else {
			// Shift left (sliding window)
			memmove(usbHistory, usbHistory + 1, USB_GRAPH_HISTORY - 1);
			usbHistory[USB_GRAPH_HISTORY - 1] = stateValue;
	}
}



