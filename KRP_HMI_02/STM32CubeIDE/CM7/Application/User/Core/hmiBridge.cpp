#include "hmiBridge.h"

#include <gui/common/FrontendApplication.hpp>
#include <gui/screen2_screen/Screen2View.hpp>
#include <touchgfx/Application.hpp>
#include <gui/model/Model.hpp>


extern "C" void HMI_setUsbRoleText(const char* msg)
{
    auto* app = static_cast<FrontendApplication*>(touchgfx::Application::getInstance());
    if (app) {
        app->getModel().setUsbRoleText(msg);
    }
}

extern "C" void HMI_setUsbStateText(const char* msg)
{
    auto* app = static_cast<FrontendApplication*>(touchgfx::Application::getInstance());
    if (app) {
        app->getModel().setUsbStateText(msg);
    }
}

extern "C" void HMI_addSystemMessage(const char* msg)
{
    auto* app = static_cast<FrontendApplication*>(touchgfx::Application::getInstance());
    if (app) {
        app->getModel().addSystemMessage(msg);
    }
}

extern "C" void HMI_addUsbStateGraphPoint(uint8_t stateValue)
{
    auto* app = static_cast<FrontendApplication*>(touchgfx::Application::getInstance());
    if (app) {
        app->getModel().addUsbStateGraphPoint(stateValue);
    }
}
