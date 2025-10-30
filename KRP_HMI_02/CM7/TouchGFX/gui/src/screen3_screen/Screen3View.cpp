#include <gui/screen3_screen/Screen3View.hpp>

Screen3View::Screen3View()
{

}

void Screen3View::setupScreen()
{
    Screen3ViewBase::setupScreen();

    const uint8_t* history = presenter->getUSBStateHistory();
		uint16_t count = presenter->getUSBStateHistorySize();

		for (uint16_t i = 0; i < count; ++i)
				USB_State_Graph.addDataPoint(history[i]);
}

void Screen3View::tearDownScreen()
{
    Screen3ViewBase::tearDownScreen();
}

void Screen3View::addUsbStateGraphPoint(uint8_t stateValue) {

    // Push value to the dynamic graph
    USB_State_Graph.addDataPoint(static_cast<int>(stateValue));

    // Force redraw
    // USB_State_Graph.invalidate();
}
