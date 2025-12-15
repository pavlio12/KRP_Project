#include <gui/screen1_screen/Screen1View.hpp>

Screen1View::Screen1View()
{

}

void Screen1View::setupScreen()
{
    Screen1ViewBase::setupScreen();

}

void Screen1View::tearDownScreen()
{
    Screen1ViewBase::tearDownScreen();
}


void Screen1View::setUsbRoleText(const char* msg)
{
		Unicode::fromUTF8(reinterpret_cast<const uint8_t*>(msg ? msg : ""), textUsbRole1Buffer, TEXTUSBROLE1_SIZE);
		textUsbRole1.setWildcard(textUsbRole1Buffer);
		textUsbRole1.resizeToCurrentText();
		textUsbRole1.invalidateContent();
}

void Screen1View::setUsbStateText(const char* msg)
{
		Unicode::fromUTF8(reinterpret_cast<const uint8_t*>(msg ? msg : ""), textUsbState1Buffer, TEXTUSBSTATE1_SIZE);
		textUsbState1.setWildcard(textUsbState1Buffer);
		textUsbState1.resizeToCurrentText();
		textUsbState1.invalidateContent();
}


void Screen1View::setDeviceInfo(const char* msg)
{
		Unicode::fromUTF8(reinterpret_cast<const uint8_t*>(msg), textDevInfoBuffer, TEXTDEVINFO_SIZE);
		textDevInfo.invalidateContent();
		textDevInfo.setWildcard(textDevInfoBuffer);
		textDevInfo.resizeToCurrentText();
		textDevInfo.invalidateContent();
		// textSysMessages.invalidate();
}

void Screen1View::appendDeviceInfo(const char* msg)
{
    // Convert new message into temporary Unicode buffer
    Unicode::UnicodeChar newMsg[128];
    Unicode::fromUTF8(reinterpret_cast<const uint8_t*>(msg), newMsg, 128);

    // Find current length in sysMsgBuffer
    size_t len = Unicode::strlen(textDevInfoBuffer);

    // Add a newline if there's already text
    if (len > 0 && len < TEXTDEVINFO_SIZE - 2)
    	textDevInfoBuffer[len++] = '\n';

    // Copy new message into sysMsgBuffer
    size_t remaining = TEXTDEVINFO_SIZE - len - 1;
    Unicode::strncpy(textDevInfoBuffer + len, newMsg, remaining);

    // Ensure termination
    textDevInfoBuffer[TEXTDEVINFO_SIZE - 1] = 0;

    textDevInfo.invalidateContent();
    textDevInfo.setWildcard(textDevInfoBuffer);
    textDevInfo.resizeToCurrentText();
    textDevInfo.invalidateContent();

}
