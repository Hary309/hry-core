#pragma once

#include <algorithm>
#include <memory>

#include "Hry/Logger/Logger.hpp"
#include "PluginInfo.hpp"
#include "Events/EventHandler.hpp"
#include "KeyBinding/KeyBinds.hpp"

namespace hry 
{

class Plugin
{
public:
    // never nullptr
    std::unique_ptr<Logger> logger;
    std::unique_ptr<EventHandler> eventHandler;
    KeyBinds* keyBinds;

public:
    virtual ~Plugin() {}
    virtual void init() = 0;

    virtual void imguiPage() = 0;
    
    virtual const PluginInfo& getPluginInfo() const = 0;
};

}

#define INIT_PLUGIN(PLUGIN_TYPE)                      \
extern "C"                                            \
{                                                     \
    __declspec(dllexport) hry::Plugin* CreatePlugin() \
    {                                                 \
        return new PLUGIN_TYPE();                     \
    }                                                 \
}

#define INIT_IMGUI()                                        \
extern "C"                                                  \
{                                                           \
    __declspec(dllexport) void InitImGui(ImGuiContext* ctx) \
    {                                                       \
        ImGui::SetCurrentContext(ctx);                      \
    }                                                       \
}

