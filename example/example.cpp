#include <iostream>
#include <memory>

#include <imgui.h>
#include <windows.h>

#include "Hry/Events/EventHandler.hpp"
#include "Hry/System/System.hpp"
#include "Hry/Version.hpp"
#include <Hry/Config/Config.hpp>
#include <Hry/Config/Fields/NumericField.hpp>
#include <Hry/Events/Event.hpp>
#include <Hry/KeyBinding/KeyBinds.hpp>
#include <Hry/Logger/Logger.hpp>
#include <Hry/Plugin.hpp>
#include <Hry/System/Keyboard.hpp>
#include <Hry/Utils/Delegate.hpp>

class ExamplePlugin : public hry::Plugin
{
private:
    hry::PluginInfo _pluginInfo{ "hry-example",
                                 "Example plugin",
                                 { "Hary309", "piotrkrupa06@gmail.com" },
                                 "This is a example of plugin based on hry-core.",
                                 hry::Version{ 1, 0, 0 } };

public:
    inline static hry::Logger* Logger;

public:
    ~ExamplePlugin() override { Logger->info("Unloading..."); }

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

    void initEvents(hry::EventHandler* eventHandler) override {}

    void initConfig(hry::Config* config) override
    {
        auto* intField = config->createField<hry::NumericField<int>>("Test", "test");
        intField->useDrag();
        intField->setDefaultValue(23);
    }

    void initKeyBinds(hry::KeyBinds* keyBinds) override {}

    void imguiPage() override { ImGui::Text("Test asdf"); }

    const hry::PluginInfo& getPluginInfo() const override { return _pluginInfo; }
};

INIT_PLUGIN(ExamplePlugin)
