#include <gui/screen2_screen/Screen2View.hpp>
#include <gui/screen2_screen/Screen2Presenter.hpp>

Screen2Presenter::Screen2Presenter(Screen2View& v)
    : view(v)
{

}

void Screen2Presenter::activate()
{
	if (model) {
		model->setScreen2Active(true);
	}
}

void Screen2Presenter::deactivate()
{
	if (model) {
		model->setScreen2Active(false);
	}
}

void Screen2Presenter::setUsbRoleText(const char* msg) {
	view.setUsbRoleText(msg);
}

void Screen2Presenter::setUsbStateText(const char* msg) {
	view.setUsbStateText(msg);
}

void Screen2Presenter::setSystemMessage(const char* msg) {
	view.setSystemMessage(msg);
}

void Screen2Presenter::appendSystemMessage(const char* msg) {
	view.appendSystemMessage(msg);
}

void Screen2Presenter::prependSystemMessage(const char* msg) {
	view.prependSystemMessage(msg);
}

