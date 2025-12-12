#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

#include "main.h"
#include "string.h"
#include <stdio.h>

// static uint32_t ModelCounter = 0;


Model::Model() : modelListener(0)
{
    sysMsgLog[0] = '\0';
    sysMsgLogLen = 0;
    addSystemMessage("HMI initialized");
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

    if (screen2Active && needsFullSysMsgSync && modelListener) {
        modelListener->setSystemMessage(sysMsgLog);
        resetPendingSystemMessages();
        needsFullSysMsgSync = false;
    }

		// Log System Messages - Add to the beginning (Prepend instead of Append)
		if (screen2Active && sysMsgCount > 0 && modelListener) {
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

    appendToSystemLog(msg);

    if (!screen2Active) {
        needsFullSysMsgSync = true;
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

void Model::setScreen2Active(bool active)
{
    screen2Active = active;
    if (active) {
        needsFullSysMsgSync = true;
    } else {
        resetPendingSystemMessages();
    }
}

void Model::resetPendingSystemMessages()
{
    sysMsgHead = 0;
    sysMsgTail = 0;
    sysMsgCount = 0;
}

void Model::appendToSystemLog(const char* msg)
{
    if (!msg) {
        return;
    }

    size_t msgLen = strlen(msg);
    if (msgLen >= SYSMSG_TEXT_LEN) {
        msgLen = SYSMSG_TEXT_LEN - 1;
    }
    bool needsNewline = (sysMsgLogLen > 0);
    size_t totalAdd = msgLen + (needsNewline ? 1U : 0U);

    // Ensure we have room; drop oldest line(s) if necessary
    while ((sysMsgLogLen + totalAdd) >= SYSMSG_LOG_SIZE && sysMsgLogLen > 0) {
        char* firstNewline = static_cast<char*>(memchr(sysMsgLog, '\n', sysMsgLogLen));
        if (firstNewline) {
            size_t dropLen = static_cast<size_t>(firstNewline - sysMsgLog) + 1U;
            memmove(sysMsgLog, firstNewline + 1, sysMsgLogLen - dropLen);
            sysMsgLogLen -= dropLen;
        } else {
            sysMsgLogLen = 0;
            break;
        }
    }

    if (needsNewline && sysMsgLogLen < SYSMSG_LOG_SIZE - 1) {
        sysMsgLog[sysMsgLogLen++] = '\n';
    }

    size_t copyLen = (msgLen < (SYSMSG_LOG_SIZE - 1 - sysMsgLogLen))
                     ? msgLen
                     : (SYSMSG_LOG_SIZE - 1 - sysMsgLogLen);
    memcpy(sysMsgLog + sysMsgLogLen, msg, copyLen);
    sysMsgLogLen += copyLen;
    sysMsgLog[sysMsgLogLen] = '\0';
}



