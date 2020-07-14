#pragma once

#include <memory>

#include "PluginInfo.hpp"
#include "Events/Event.hpp"
#include "Logger/ModuleLogger.hpp"

namespace hry 
{

class Plugin
{
public:
    virtual ~Plugin() {}
    virtual void update(float deltaTime) = 0;
    virtual void imguiRender() = 0;
    virtual void event(const events::Event& e) = 0;
    virtual const PluginInfo& getPluginInfo() = 0;
    virtual void initLogger(std::unique_ptr<logger::ModuleLogger>&& logger) = 0;
};

}

#define INIT_PLUGIN(PLUGIN_TYPE)                 \
extern "C"                                       \
{                                                \
    __declspec(dllexport) Plugin* CreatePlugin() \
    {                                            \
        return new PLUGIN_TYPE();                \
    }                                            \
}


#define INIT_IMGUI()                                        \
extern "C"                                                  \
{                                                           \
    __declspec(dllexport) void InitImGui(ImGuiContext* ctx) \
    {                                                       \
        ImGui::SetCurrentContext(ctx);                      \
    }                                                       \
}