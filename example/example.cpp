#include "Hry/Logger/ModuleLogger.hpp"
#include <windows.h>
#include <iostream>

#include <imgui.h>

#include <Hry/Plugin.hpp>

using namespace hry;

class ExamplePlugin : public Plugin
{
private:
    PluginInfo _pluginInfo = {
            "Example plugin",
            "hry-example",
            "This is a example of plugin based on hry-core.",
            "Example implementation of plugin",
            1000,
    };

public:
    inline static std::unique_ptr<logger::ModuleLogger> Logger;

public:
    virtual ~ExamplePlugin()
    {

    }

    virtual void update(float deltaTime)
    {

    }

    virtual void imguiRender()
    {
        
    }

    virtual void event(const events::Event& e)
    {

    }

    virtual const PluginInfo& getPluginInfo()
    {
        return _pluginInfo;
    }

    virtual void initLogger(std::unique_ptr<logger::ModuleLogger>&& logger) 
    {
        Logger = std::move(logger);
    }
};

extern "C"
{
    // TODO: add dllexport macro
    __declspec(dllexport) Plugin* CreatePlugin()
    {
        return new ExamplePlugin();
    }
}