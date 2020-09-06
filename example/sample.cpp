#include <iostream>
#include <memory>

#include <imgui.h>
#include <windows.h>

#include "Hry/Version.hpp"
#include <Hry/Config/Config.hpp>
#include <Hry/Events/Event.hpp>
#include <Hry/Logger/Logger.hpp>
#include <Hry/Plugin.hpp>
#include <Hry/System/Keyboard.hpp>

class SamplePlugin : public hry::Plugin
{
private:
    hry::PluginInfo _pluginInfo{ "hry-sample",
                                 "Simple plugin",
                                 { "Hary309", "piotrkrupa06@gmail.com" },
                                 "This is a example of plugin based on hry-core.",
                                 hry::Version{ 1, 0, 0 } };

public:
    inline static hry::Logger* Logger;

public:
    ~SamplePlugin() override
    {
        if (Logger != nullptr)
        {
            Logger->info("Unloading...");
        }
    }

    Result init(const InitParams&& initParams) override
    {
        if (!hry::IsApiCompatible(initParams.apiVersion))
        {
            return Result::ApiNotSupported;
        }

        Logger = initParams.logger;
        Logger->info("Created!");

        return Result::Ok;
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
        Logger->info("Button pressed! ID: {}", static_cast<int>(button.button));
    }
};

INIT_PLUGIN(SamplePlugin)
