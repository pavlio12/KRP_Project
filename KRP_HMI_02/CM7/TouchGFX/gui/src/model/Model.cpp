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
    devInfoText[0] = '\0';
    lastDevInfo[0] = '\0';
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

    if (hasNewDevInfo && modelListener) {
        modelListener->setDeviceInfo(devInfoText);
        hasNewDevInfo = false;
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

	strncpy(lastUsbRole, pendingUsbRole, sizeof(lastUsbRole) - 1);
	lastUsbRole[sizeof(lastUsbRole) - 1] = '\0';
	hasLastUsbRole = true;
}

void Model::setUsbStateText(const char* msg) {
	strncpy(pendingUsbState, msg, sizeof(pendingUsbState) - 1);
	pendingUsbState[sizeof(pendingUsbState) - 1] = '\0';
	hasNewUsbState = true;

	strncpy(lastUsbState, pendingUsbState, sizeof(lastUsbState) - 1);
	lastUsbState[sizeof(lastUsbState) - 1] = '\0';
	hasLastUsbState = true;
}

void Model::setDeviceInfo(const char* msg) {
	strncpy(devInfoText, msg, sizeof(devInfoText) - 1);
	devInfoText[sizeof(devInfoText) - 1] = '\0';
	hasNewDevInfo = true;

	strncpy(lastDevInfo, devInfoText, sizeof(lastDevInfo) - 1);
	lastDevInfo[sizeof(lastDevInfo) - 1] = '\0';
	hasLastDevInfo = true;
}

void Model::appendDeviceInfo(const char* msg) {
    if (!msg) {
        return;
    }

    size_t curLen = strlen(devInfoText);
    size_t msgLen = strlen(msg);
    if (msgLen >= DEVINFO_LOG_SIZE) {
        msgLen = DEVINFO_LOG_SIZE - 1;
    }

    // Add newline if existing text
    if (curLen > 0 && curLen < DEVINFO_LOG_SIZE - 1) {
        devInfoText[curLen++] = '\n';
    }

    size_t remaining = (DEVINFO_LOG_SIZE - 1 > curLen) ? (DEVINFO_LOG_SIZE - 1 - curLen) : 0;
    if (msgLen > remaining) {
        msgLen = remaining;
    }

    if (msgLen > 0) {
        memcpy(devInfoText + curLen, msg, msgLen);
        curLen += msgLen;
    }

    devInfoText[curLen] = '\0';
    hasNewDevInfo = true;

    strncpy(lastDevInfo, devInfoText, sizeof(lastDevInfo) - 1);
    lastDevInfo[sizeof(lastDevInfo) - 1] = '\0';
    hasLastDevInfo = true;
}

void Model::addSystemMessage(const char* msg) {
    if (!msg) {
            return;
    }

    prependToSystemLog(msg);

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

void Model::setScreen1Active(bool active)
{
    screen1Active = active;
    if (active) {
        if (modelListener) {
            if (hasLastUsbRole) {
                modelListener->setUsbRoleText(lastUsbRole);
            }
            if (hasLastUsbState) {
                modelListener->setUsbStateText(lastUsbState);
            }
            if (hasLastDevInfo) {
                modelListener->setDeviceInfo(lastDevInfo);
                /* Ensure UI refreshes even if no new messages arrive */
                hasNewDevInfo = true;
            }
        }
    } else {
        // Nothing?
    }
}
void Model::setScreen2Active(bool active)
{
    screen2Active = active;
    if (active) {
        needsFullSysMsgSync = true;
        if (modelListener) {
            if (hasLastUsbRole) {
                modelListener->setUsbRoleText(lastUsbRole);
            }
            if (hasLastUsbState) {
                modelListener->setUsbStateText(lastUsbState);
            }
        }
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

void Model::prependToSystemLog(const char* msg)
{
    if (!msg) {
        return;
    }

    size_t msgLen = strlen(msg);
    if (msgLen >= SYSMSG_TEXT_LEN) {
        msgLen = SYSMSG_TEXT_LEN - 1;
    }
    // We will prepend, so calculate space: new message + optional newline if existing text
    bool hasExisting = (sysMsgLogLen > 0);
    size_t totalAdd = msgLen + (hasExisting ? 1U : 0U);

    // Ensure space by trimming from the end (oldest text) if needed
    while ((sysMsgLogLen + totalAdd) >= SYSMSG_LOG_SIZE && sysMsgLogLen > 0) {
        // Find last newline manually (no memrchr in some libs)
        size_t lastNlIndex = sysMsgLogLen;
        bool found = false;
        while (lastNlIndex > 0) {
            if (sysMsgLog[lastNlIndex - 1] == '\n') {
                found = true;
                break;
            }
            lastNlIndex--;
        }

        if (found) {
            sysMsgLogLen = lastNlIndex - 1;
            if (sysMsgLogLen > SYSMSG_LOG_SIZE - 1) {
                sysMsgLogLen = SYSMSG_LOG_SIZE - 1;
            }
            sysMsgLog[sysMsgLogLen] = '\0';
        } else {
            sysMsgLogLen = 0;
            sysMsgLog[0] = '\0';
            break;
        }
    }
    hasExisting = (sysMsgLogLen > 0);

    // Shift existing content to make room for new message + optional newline
    // Recalculate msg length to fit
    size_t maxMsgLen = SYSMSG_LOG_SIZE - 1;
    if (hasExisting && maxMsgLen > 0) {
        maxMsgLen -= 1;
    }
    if (sysMsgLogLen < maxMsgLen && msgLen > (maxMsgLen - sysMsgLogLen)) {
        msgLen = maxMsgLen - sysMsgLogLen;
    }
    totalAdd = msgLen + (hasExisting ? 1U : 0U);

    size_t shiftLen = sysMsgLogLen;
    if (shiftLen > 0) {
        memmove(sysMsgLog + totalAdd, sysMsgLog, shiftLen);
    }

    // Copy new message at start
    memcpy(sysMsgLog, msg, msgLen);
    sysMsgLogLen = msgLen + shiftLen + (hasExisting ? 1U : 0U);

    // Insert newline after new message if needed
    if (hasExisting && msgLen < SYSMSG_LOG_SIZE - 1) {
        sysMsgLog[msgLen] = '\n';
    }

    // After possible newline, ensure null-termination
    if (sysMsgLogLen >= SYSMSG_LOG_SIZE) {
        sysMsgLogLen = SYSMSG_LOG_SIZE - 1;
    }
    sysMsgLog[sysMsgLogLen] = '\0';
}
