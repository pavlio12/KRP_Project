#ifndef SCREEN2VIEW_HPP
#define SCREEN2VIEW_HPP

#include <gui_generated/screen2_screen/Screen2ViewBase.hpp>
#include <gui/screen2_screen/Screen2Presenter.hpp>

class Screen2View : public Screen2ViewBase
{
public:
    Screen2View();
    virtual ~Screen2View() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    // USB Role Text
    virtual void setUsbRoleText(const char* msg);

    // USB State Text
    virtual void setUsbStateText(const char* msg);

    // System Messages TextArea
    virtual void setSystemMessage(const char* msg);     // public setter
    virtual void appendSystemMessage(const char* msg);
    virtual void prependSystemMessage(const char *msg);




protected:
		// --- No need to define the buffers myself. They are defined in Screen2ViewBase.hpp >> Wildcard Buffers
    // static const uint16_t USB_ROLE_BUFFER_SIZE = 16;
		// static const uint16_t USB_STATE_BUFFER_SIZE = 16;
		// static const uint16_t SYS_MSG_BUFFER_SIZE = 1024;
		// Unicode::UnicodeChar UsbRoleBuffer[USB_ROLE_BUFFER_SIZE];
		// Unicode::UnicodeChar UsbStateBuffer[USB_STATE_BUFFER_SIZE];
    // Unicode::UnicodeChar sysMsgBuffer[SYS_MSG_BUFFER_SIZE];  // actual buffer
};

#endif // SCREEN2VIEW_HPP
