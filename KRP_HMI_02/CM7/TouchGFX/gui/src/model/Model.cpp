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

		if (hasNewMessage && modelListener) {
				modelListener->prependSystemMessage(pendingMessage); // Add to the beginning of TextArea
				hasNewMessage = false;
		}

		if (hasNewUsbGraphPoint && modelListener) {
				modelListener->addUsbStateGraphPoint(pendingUsbGraphPoint);
				hasNewUsbGraphPoint = false;
		}

}

void Model::addSystemMessage(const char* msg) {
	strncpy(pendingMessage, msg, sizeof(pendingMessage) - 1);
	pendingMessage[sizeof(pendingMessage) - 1] = '\0';
	hasNewMessage = true;

	/*
	if (modelListener) {
		modelListener->setSystemMessage(msg); // - Broken code line!
	}
	*/
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



