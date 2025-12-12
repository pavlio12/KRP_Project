#include <gui/screen2_screen/Screen2View.hpp>
// My includes
#include <touchgfx/Unicode.hpp>


Screen2View::Screen2View()
{
}

void Screen2View::setupScreen()
{
    Screen2ViewBase::setupScreen();
}

void Screen2View::tearDownScreen()
{
    Screen2ViewBase::tearDownScreen();
}

void Screen2View::setUsbRoleText(const char* msg)
{
		Unicode::fromUTF8(reinterpret_cast<const uint8_t*>(msg ? msg : ""), textUsbRoleBuffer, TEXTUSBROLE_SIZE);
		textUsbRole.setWildcard(textUsbRoleBuffer);
		textUsbRole.resizeToCurrentText();
		textUsbRole.invalidateContent();
}

void Screen2View::setUsbStateText(const char* msg)
{
		Unicode::fromUTF8(reinterpret_cast<const uint8_t*>(msg ? msg : ""), textUsbStateBuffer, TEXTUSBSTATE_SIZE);
		textUsbState.setWildcard(textUsbStateBuffer);
		textUsbState.resizeToCurrentText();
		textUsbState.invalidateContent();
}


void Screen2View::setSystemMessage(const char* msg)
{
		Unicode::fromUTF8(reinterpret_cast<const uint8_t*>(msg), textSysMessagesBuffer, TEXTSYSMESSAGES_SIZE);
		textSysMessages.invalidateContent();
		textSysMessages.setWildcard(textSysMessagesBuffer);
		textSysMessages.resizeToCurrentText();
		textSysMessages.invalidateContent();
		// textSysMessages.invalidate();
}

void Screen2View::appendSystemMessage(const char* msg)
{
    // Convert new message into temporary Unicode buffer
    Unicode::UnicodeChar newMsg[128];
    Unicode::fromUTF8(reinterpret_cast<const uint8_t*>(msg), newMsg, 128);

    // Find current length in sysMsgBuffer
    size_t len = Unicode::strlen(textSysMessagesBuffer);

    // Add a newline if there's already text
    if (len > 0 && len < TEXTSYSMESSAGES_SIZE - 2)
    	textSysMessagesBuffer[len++] = '\n';

    // Copy new message into sysMsgBuffer
    size_t remaining = TEXTSYSMESSAGES_SIZE - len - 1;
    Unicode::strncpy(textSysMessagesBuffer + len, newMsg, remaining);

    // Ensure termination
    textSysMessagesBuffer[TEXTSYSMESSAGES_SIZE - 1] = 0;

    textSysMessages.invalidateContent();
    textSysMessages.setWildcard(textSysMessagesBuffer);
    textSysMessages.resizeToCurrentText();
    textSysMessages.invalidateContent();

}

void Screen2View::prependSystemMessage(const char *msg) {
	textSysMessages.invalidateContent();
	Unicode::UnicodeChar newMsg[128];
	Unicode::fromUTF8(reinterpret_cast<const uint8_t*>(msg ? msg : ""), newMsg, 128);

	Unicode::UnicodeChar newBuffer[TEXTSYSMESSAGES_SIZE];
	newBuffer[0] = 0;

	// Copy new message first
	Unicode::strncpy(newBuffer, newMsg, TEXTSYSMESSAGES_SIZE - 1);

	// Add newline
	size_t len = Unicode::strlen(newBuffer);
	if (len < TEXTSYSMESSAGES_SIZE - 2) {
			newBuffer[len++] = '\n';
			newBuffer[len]   = 0;
	}

	// Append the old buffer *after* new message
	size_t remaining = TEXTSYSMESSAGES_SIZE - len - 1;
	Unicode::strncpy(newBuffer + len, textSysMessagesBuffer, remaining);
	newBuffer[TEXTSYSMESSAGES_SIZE - 1] = 0;

	// Copy back
	Unicode::strncpy(textSysMessagesBuffer, newBuffer, TEXTSYSMESSAGES_SIZE);

	// Update on screen
	textSysMessages.resizeToCurrentText();
	textSysMessages.invalidateContent();
}

