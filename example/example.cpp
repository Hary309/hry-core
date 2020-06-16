#include <windows.h>
#include <iostream>

#include <imgui.h>

#include <hry/Plugin.hpp>

class ExamplePlugin : public hry::Plugin
{
private:
    hry::PluginInfo _pluginInfo = {
            "Example plugin",
            "Example implementation of plugin",
            1000,
    };

public:
    virtual void update(float deltaTime)
    {

    }

    virtual void imguiRender()
    {

    }

    virtual void event(const hry::Event& e)
    {

    }

    virtual const hry::PluginInfo& getPluginInfo()
    {
        return _pluginInfo;
    }
};

hry::Plugin* CreatePlugin()
{
    return new ExamplePlugin();
}
