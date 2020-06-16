#pragma once
#include "PluginInfo.hpp"
#include "Event.hpp"

namespace hry 
{

class Plugin
{
public:
    virtual void update(float deltaTime) = 0;
    virtual void imguiRender() = 0;
    virtual void event(const Event& e) = 0;
    virtual const PluginInfo& getPluginInfo() = 0;
};

}
