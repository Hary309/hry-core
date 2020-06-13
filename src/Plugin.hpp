#pragma once

#include "Core.hpp"

#include "PluginInfo.hpp"

namespace hry 
{

class Event;

class Plugin
{
public:
    virtual void update(float deltaTime) = 0;
    virtual void imguiRender() = 0;
    virtual void event(const Event& e) = 0;
    virtual const PluginInfo& getPluginInfo() = 0;
};

}
