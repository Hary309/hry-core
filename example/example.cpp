#include <iostream>
#include <memory>

#include <imgui.h>
#include <windows.h>

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
    ~ExamplePlugin() override { logger->info("Unloading..."); }

    void init() override
    {
        logger->info("Created!");

        eventHandler->onKeyPress.connect<&ExamplePlugin::onKeyPressed>(this);
        eventHandler->onImGuiRender.connect<&ExamplePlugin::imguiRender>(this);
    }

    // OBS STUDIO

    void initConfig(hry::Config* config, hry::KeyBinds* keyBinds) override
    {
        auto intField = config->createField<hry::NumericField<int>>("Test", "test");
        intField->useDrag();
        intField->setDefaultValue(23);

        hry::KeyBind doSomethingBind;
        doSomethingBind.setConfigFieldName("do_something");
        doSomethingBind.setName("Do something");
        doSomethingBind.setDefaultKey(hry::Keyboard::Key::Q);
        doSomethingBind.pressAction.connect<&ExamplePlugin::onKeyBind>(this);
        keyBinds->addKeyBind(std::move(doSomethingBind));
    }

    void imguiPage() override { ImGui::Text("Test asdf"); }

    const hry::PluginInfo& getPluginInfo() const override { return _pluginInfo; }

    void imguiRender()
    {
        static float ads;
        if (ImGui::Begin("hry-example"))
        {
            ImGui::Text("asdf");
            ImGui::InputFloat("float", &ads);
        }

        ImGui::End();
    }

private:
    void onKeyPressed(const hry::KeyboardEvent& key)
    {
        logger->info("Key pressed!", static_cast<int>(key.key));
    }

    void onKeyBind() { logger->info("Key bind works!"); }
};

INIT_PLUGIN(ExamplePlugin)
INIT_IMGUI()
