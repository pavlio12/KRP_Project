#include <gui/screen1_screen/Screen1View.hpp>
#include <gui/screen1_screen/Screen1Presenter.hpp>

Screen1Presenter::Screen1Presenter(Screen1View& v)
    : view(v)
{

}

void Screen1Presenter::activate()
{
	if (model) {
		model->setScreen1Active(true);
	}
}

void Screen1Presenter::deactivate()
{
	if (model) {
		model->setScreen1Active(false);
	}
}

void Screen1Presenter::setUsbRoleText(const char* msg) {
	view.setUsbRoleText(msg);
}

void Screen1Presenter::setUsbStateText(const char* msg) {
	view.setUsbStateText(msg);
}

void Screen1Presenter::setDeviceInfo(const char* msg) {
	view.setDeviceInfo(msg);
}

void Screen1Presenter::appendDeviceInfo(const char* msg) {
	view.appendDeviceInfo(msg);
}
