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

struct ExampleConfigData
{
    int value;
};

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
    ~ExamplePlugin() override
    {
        if (Logger != nullptr)
        {
            Logger->info("Unloading...");
        }
    }

    Result init(const InitParams&& initParams) override
    {
        Logger = initParams.logger;
        Logger->info("Created!");

        return Result::Ok;
    }

    void initEvents(hry::EventHandler* /*eventHandler*/) override {}

    void initConfig(hry::Config* config) override
    {
        config->setBindingType<ExampleConfigData>();

        config->add(hry::NumericFieldBuilder<int>()
                        .bind(&ExampleConfigData::value)
                        .setID("test")
                        .setLabel("Test")
                        .useDrag()
                        .setDefaultValue(23)
                        .build());
    }

    void initKeyBinds(hry::KeyBinds* keyBinds) override
    {
        keyBinds->add(hry::KeyBindBuilder()
                          .setID("do_sth")
                          .setLabel("Do sth")
                          .setDefaultKey(hry::Keyboard::Key::Q)
                          .build());
    }

    const hry::PluginInfo& getPluginInfo() const override { return _pluginInfo; }
};

INIT_PLUGIN(ExamplePlugin)
