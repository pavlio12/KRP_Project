#include "hmiBridge.h"

#include <gui/common/FrontendApplication.hpp>
#include <gui/screen2_screen/Screen2View.hpp>
#include <touchgfx/Application.hpp>
#include <gui/model/Model.hpp>
#include <cstring>

static char lastSystemMessage[128] = {0};
static size_t lastSystemMessageLen = 0;

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
    if (!msg) {
        return;
    }
    // Simple dedup: skip if identical to previous message content (length + data)
    size_t len = std::strlen(msg);
    if (len >= sizeof(lastSystemMessage)) {
        len = sizeof(lastSystemMessage) - 1;
    }
    if (len == lastSystemMessageLen &&
        std::strncmp(msg, lastSystemMessage, len) == 0) {
        return;
    }
    std::strncpy(lastSystemMessage, msg, sizeof(lastSystemMessage) - 1);
    lastSystemMessage[sizeof(lastSystemMessage) - 1] = '\0';
    lastSystemMessageLen = std::strlen(lastSystemMessage);

    // Pass the message to the TouchGFX model
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
