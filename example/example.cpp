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

    void initEvents(hry::EventHandler* eventHandler) override
    {
        eventHandler->onKeyPress.connect<&ExamplePlugin::onKeyPressed>(this);
        eventHandler->onImGuiRender.connect<&ExamplePlugin::imguiRender>(this);
    }

    void initConfig(hry::Config* config) override
    {
        auto* intField = config->createField<hry::NumericField<int>>("Test", "test");
        intField->useDrag();
        intField->setDefaultValue(23);
    }

    void initKeyBinds(hry::KeyBinds* keyBinds) override
    {
        hry::KeyBind doSomethingBind;
        doSomethingBind.setConfigFieldName("do_something");
        doSomethingBind.setName("Do something");
        doSomethingBind.setDefaultKey(hry::Keyboard::Key::Q);
        doSomethingBind.pressAction.connect<&ExamplePlugin::onKeyBind>(this);
        doSomethingBind.releaseAction.connect<&ExamplePlugin::onKeyBind>(this);
        keyBinds->addKeyBind(std::move(doSomethingBind));

        hry::KeyBind holdBind(hry::KeyBind::Activator::Hold);
        holdBind.setConfigFieldName("hold");
        holdBind.setName("Hold button");
        holdBind.setDefaultKey(hry::Keyboard::Key::Q);
        holdBind.pressAction.connect<&ExamplePlugin::onKeyHold>(this);
        holdBind.releaseAction.connect<&ExamplePlugin::onKeyHold>(this);
        keyBinds->addKeyBind(std::move(holdBind));
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
        Logger->info("Key pressed!", static_cast<int>(key.key));
    }

    void onKeyBind(hry::ButtonState state)
    {
        switch (state)
        {
            case hry::ButtonState::Pressed:
            {
                Logger->info("Press!");
            }
            break;
            case hry::ButtonState::Released:
            {
                Logger->info("Release!");
            }
            break;
        }
    }

    void onKeyHold(hry::ButtonState state)
    {
        switch (state)
        {
            case hry::ButtonState::Pressed:
            {
                Logger->info("Holding!");
            }
            break;
            case hry::ButtonState::Released:
            {
                Logger->info("Released hold!");
            }
            break;
        }
    }
};

INIT_PLUGIN(ExamplePlugin)
