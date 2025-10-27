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

    virtual void setSystemMessage(const char* msg);           // public setter
		static const uint16_t SYS_MSG_BUFFER_SIZE = 256;  // adjust!


protected:
    Unicode::UnicodeChar sysMsgBuffer[SYS_MSG_BUFFER_SIZE];  // actual buffer
};

#endif // SCREEN2VIEW_HPP
