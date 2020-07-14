#include <memory>
#include <windows.h>
#include <iostream>

#include <imgui.h>

#include <Hry/Plugin.hpp>
#include <Hry/Events/Event.hpp>
#include <Hry/Logger/ModuleLogger.hpp>
#include <Hry/System/Keyboard.hpp>

using namespace hry;

class SamplePlugin : public Plugin
{
private:
    PluginInfo _pluginInfo = {
            "Sample plugin",
            "hry-sample",
            "This is a example of plugin based on hry-core.",
            "Example implementation of plugin",
            1000,
    };

public:
    inline static std::unique_ptr<logger::ModuleLogger> Logger;

public:
    virtual ~SamplePlugin()
    {
        Logger->info("Unloading...");
    }

    virtual void init(std::unique_ptr<logger::ModuleLogger>&& logger)
    {
        Logger = std::move(logger);
        Logger->info("Created!");

        eventHandler->onMouseButtonPress.connect<&SamplePlugin::onKeyPressed>(this);
    }

    virtual void update(float deltaTime)
    {

    }

    virtual void imguiRender()
    {
        if (ImGui::Begin("hry-example"))
        {
            ImGui::Text("asdf");
        }

        ImGui::End();
    }

    virtual void imguiSettingsTab()
    {
        ImGui::Text("Settings tab");
    }

    virtual const PluginInfo& getPluginInfo() const
    {
        return _pluginInfo;
    }

private:
    void onKeyPressed(const events::MouseButtonEvent&& button)
    {
        Logger->info("Button pressed!", static_cast<int>(button.button));
    }
};

INIT_PLUGIN(SamplePlugin)
INIT_IMGUI()
