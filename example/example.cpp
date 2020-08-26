#include <iostream>
#include <memory>

#include <imgui.h>
#include <windows.h>

#include "Hry/Events/EventHandler.hpp"
#include "Hry/System/System.hpp"
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
    hry::PluginInfo _pluginInfo = {
        "Example plugin",
        "hry-example",
        "This is a example of plugin based on hry-core.",
        "Example implementation of plugin",
        1000,
    };

public:
    inline static hry::Logger* Logger;

public:
    ~ExamplePlugin() override { Logger->info("Unloading..."); }

    void init(hry::Logger* logger) override
    {
        Logger = logger;
        Logger->info("Created!");
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
