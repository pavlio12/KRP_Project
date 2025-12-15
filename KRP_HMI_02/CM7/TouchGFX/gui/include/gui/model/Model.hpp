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

    void setScreen1Active(bool active);
    void setDeviceInfo(const char* msg);
		void appendDeviceInfo(const char* msg);

    void addSystemMessage(const char* msg);
    void addUsbStateGraphPoint(uint8_t stateValue);

    const uint8_t* getUSBStateHistory() const { return usbHistory; }
		uint16_t getUSBStateHistorySize() const { return usbHistoryCount; }
    const char* getSystemMessageLog() const { return sysMsgLog; }
    void setScreen2Active(bool active);
    void resetPendingSystemMessages();


protected:
    ModelListener* modelListener;

    // USB Role Text
    bool hasNewUsbRole = false;
    char pendingUsbRole[16];
    char lastUsbRole[16] = {0};
    bool hasLastUsbRole = false;

    // USB State Text
		bool hasNewUsbState = true;
		char pendingUsbState[16];
    char lastUsbState[16] = {0};
    bool hasLastUsbState = false;

    // Screen1 USB Device Info TextArea
    bool screen1Active = false;
    bool hasNewDevInfo = false;
    static constexpr uint16_t DEVINFO_LOG_SIZE = 1024;
		char devInfoText[DEVINFO_LOG_SIZE];
		char lastDevInfo[DEVINFO_LOG_SIZE] = {0};
		bool hasLastDevInfo = false;

    // System Message TextArea (scrollable)
    static constexpr uint8_t SYSMSG_QUEUE_DEPTH = 10;
    static constexpr uint8_t SYSMSG_TEXT_LEN    = 64;
    char pendingMessages[SYSMSG_QUEUE_DEPTH][SYSMSG_TEXT_LEN];
    uint8_t sysMsgHead = 0; // write index
    uint8_t sysMsgTail = 0; // read index
    uint8_t sysMsgCount = 0;
    static constexpr uint16_t SYSMSG_LOG_SIZE = 1024;
    char sysMsgLog[SYSMSG_LOG_SIZE];
    uint16_t sysMsgLogLen = 0;
    bool screen2Active = false;
    bool needsFullSysMsgSync = true;

		// USB State Dynamic Graph
		bool hasNewUsbGraphPoint = false;
		uint8_t pendingUsbGraphPoint = 0;

		uint8_t  usbHistory[USB_GRAPH_HISTORY];
		uint16_t usbHistoryCount = 0;

private:
    void prependToSystemLog(const char* msg);
};

#endif // MODEL_HPP
