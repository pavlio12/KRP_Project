#include <gui/screen3_screen/Screen3View.hpp>
#include <gui/screen3_screen/Screen3Presenter.hpp>

Screen3Presenter::Screen3Presenter(Screen3View& v)
    : view(v)
{

}

void Screen3Presenter::activate()
{

}

void Screen3Presenter::deactivate()
{

}

void Screen3Presenter::addUsbStateGraphPoint(uint8_t stateValue) {
	view.addUsbStateGraphPoint(stateValue);
}

const uint8_t* Screen3Presenter::getUSBStateHistory() const
{
    return model->getUSBStateHistory();
}

uint16_t Screen3Presenter::getUSBStateHistorySize() const
{
    return model->getUSBStateHistorySize();
}

