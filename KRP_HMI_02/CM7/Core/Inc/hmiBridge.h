#ifndef HMI_BRIDGE_H
#define HMI_BRIDGE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DEBUG_PRINTS 0U // Set to 1U / 0U to Enable / Disable the HMI debug prints

void HMI_setUsbStateText(const char* msg);
void HMI_setUsbRoleText(const char* msg);
void HMI_setDeviceInfo(const char* msg);
void HMI_appendDeviceInfo(const char* msg);

void HMI_addSystemMessage(const char* msg);
void HMI_addUsbStateGraphPoint(uint8_t stateValue);

// void HMI_SetSystemMessage(const char* msg);

#ifdef __cplusplus
}
#endif

#endif // HMI_BRIDGE_H
