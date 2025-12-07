#ifndef MODEL_HPP
#define MODEL_HPP

#include <cstdint>

#define USB_GRAPH_HISTORY 512

class ModelListener;

class Model
{
public:
    Model();

    void bind(ModelListener* listener)
    {
        modelListener = listener;
    }

    void tick();

    void setUsbRoleText(const char* msg);
    void setUsbStateText(const char* msg);

    void addSystemMessage(const char* msg);
    void addUsbStateGraphPoint(uint8_t stateValue);

    const uint8_t* getUSBStateHistory() const { return usbHistory; }
		uint16_t getUSBStateHistorySize() const { return usbHistoryCount; }


protected:
    ModelListener* modelListener;

    // USB Role Text
    bool hasNewUsbRole = false;
    char pendingUsbRole[16];

    // USB State Text
		bool hasNewUsbState = false;
		char pendingUsbState[16];

    // System Message TextArea (scrollable)
    bool hasNewMessage = false;
		char pendingMessage[64];

		// USB State Dynamic Graph
		bool hasNewUsbGraphPoint = false;
		uint8_t pendingUsbGraphPoint = 0;

		uint8_t  usbHistory[USB_GRAPH_HISTORY];
		uint16_t usbHistoryCount = 0;
};

#endif // MODEL_HPP
