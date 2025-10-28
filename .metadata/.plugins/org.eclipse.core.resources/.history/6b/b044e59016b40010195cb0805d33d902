#include "hmiBridge.h"

#include <gui/common/FrontendApplication.hpp>
#include <gui/screen2_screen/Screen2View.hpp>
#include <touchgfx/Application.hpp>
#include <gui/model/Model.hpp>


extern "C" void HMI_SetSystemMessage(const char* msg)
{
    auto* app = static_cast<FrontendApplication*>(touchgfx::Application::getInstance());
    if (app) {
        app->getModel().setSystemMessage(msg);
    }
}
