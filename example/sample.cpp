#include <iostream>
#include <memory>

#include <imgui.h>
#include <windows.h>

#include <Hry/Config/Config.hpp>
#include <Hry/Events/Event.hpp>
#include <Hry/Logger/Logger.hpp>
#include <Hry/Plugin.hpp>
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
    inline static hry::Logger* Logger;

public:
    ~SamplePlugin() override { Logger->info("Unloading..."); }

    void init(hry::Logger* logger) override
    {
        Logger = logger;
        Logger->info("Created!");
    }

    void initEvents(hry::EventHandler* eventHandler) override
    {
        eventHandler->onMouseButtonPress.connect<&SamplePlugin::onKeyPressed>(this);
    }
    void initConfig(hry::Config* /*config*/) override {}
    void initKeyBinds(hry::KeyBinds* /*keyBinds*/) override {}

    void imguiPage() override { ImGui::Text("Settings tab"); }

    const hry::PluginInfo& getPluginInfo() const override { return _pluginInfo; }

private:
    void imguiRender()
    {
        if (ImGui::Begin("hry-example"))
        {
            ImGui::Text("asdf");
        }

        ImGui::End();
    }

    void onKeyPressed(const hry::MouseButtonEvent&& button)
    {
        Logger->info("Button pressed!", static_cast<int>(button.button));
    }
};

INIT_PLUGIN(SamplePlugin)
INIT_IMGUI()
