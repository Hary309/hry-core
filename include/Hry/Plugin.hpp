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
