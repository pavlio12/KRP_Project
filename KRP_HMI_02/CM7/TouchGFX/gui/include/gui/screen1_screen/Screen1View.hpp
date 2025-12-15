#ifndef SCREEN1VIEW_HPP
#define SCREEN1VIEW_HPP

#include <gui_generated/screen1_screen/Screen1ViewBase.hpp>
#include <gui/screen1_screen/Screen1Presenter.hpp>

class Screen1View : public Screen1ViewBase
{
public:
    Screen1View();
    virtual ~Screen1View() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    // USB Role Text
		virtual void setUsbRoleText(const char* msg);

		// USB State Text
		virtual void setUsbStateText(const char* msg);

		// USB Device Info TextArea
		virtual void setDeviceInfo(const char* msg);     // public setter
		virtual void appendDeviceInfo(const char* msg);

protected:
};

#endif // SCREEN1VIEW_HPP
