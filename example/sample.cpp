#include <memory>
#include <windows.h>
#include <iostream>

#include <imgui.h>

#include <Hry/Plugin.hpp>
#include <Hry/Events/Event.hpp>
#include <Hry/Logger/Logger.hpp>
#include <Hry/System/Keyboard.hpp>

class SamplePlugin : public hry::Plugin
{
private:
    hry::PluginInfo _pluginInfo = {
            "Sample plugin",
            "hry-sample",
            "This is a example of plugin based on hry-core.",
            "Example implementation of plugin",
            1000,
    };

public:
    inline static std::unique_ptr<hry::Logger> Logger;

public:
    virtual ~SamplePlugin()
    {
        Logger->info("Unloading...");
    }

    virtual void init()
    {
        Logger = std::move(logger);
        Logger->info("Created!");

        eventHandler->onMouseButtonPress.connect<&SamplePlugin::onKeyPressed>(this);
    }
    
    void imguiRender()
    {
        if (ImGui::Begin("hry-example"))
        {
            ImGui::Text("asdf");
        }

        ImGui::End();
    }

    virtual void imguiPage()
    {
        ImGui::Text("Settings tab");
    }

    virtual const hry::PluginInfo& getPluginInfo() const
    {
        return _pluginInfo;
    }

private:
    void onKeyPressed(const hry::MouseButtonEvent&& button)
    {
        Logger->info("Button pressed!", static_cast<int>(button.button));
    }
};

INIT_PLUGIN(SamplePlugin)
INIT_IMGUI()
