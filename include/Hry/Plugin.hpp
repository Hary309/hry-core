#pragma once

#include <algorithm>
#include <memory>

#include "Events/EventHandler.hpp"
#include "Hry/Logger/Logger.hpp"
#include "Hry/Utils/Delegate.hpp"
#include "KeyBinding/KeyBinds.hpp"
#include "PluginInfo.hpp"

namespace hry
{
class Plugin
{
public:
    // these variables are set before calling init()
    std::unique_ptr<Logger> logger;
    std::unique_ptr<EventHandler> eventHandler;
    KeyBindsUniquePtr_t keyBinds;

public:
    virtual ~Plugin() {}
    virtual void init() = 0;

    virtual void imguiPage() = 0;

    virtual const PluginInfo& getPluginInfo() const = 0;
};

} // namespace hry

#define INIT_PLUGIN(PLUGIN_TYPE)                                                                   \
    extern "C"                                                                                     \
    {                                                                                              \
        __declspec(dllexport) hry::Plugin* CreatePlugin() { return new PLUGIN_TYPE(); }            \
    }

#define INIT_IMGUI()                                                                               \
    extern "C"                                                                                     \
    {                                                                                              \
        __declspec(dllexport) void InitImGui(ImGuiContext* ctx) { ImGui::SetCurrentContext(ctx); } \
    }
