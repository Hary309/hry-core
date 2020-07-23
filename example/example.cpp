#include <iostream>
#include <memory>

#include <imgui.h>
#include <windows.h>

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
    inline static std::unique_ptr<hry::Logger> Logger;

public:
    virtual ~ExamplePlugin() { Logger->info("Unloading..."); }

    virtual void init()
    {
        Logger = std::move(logger);
        Logger->info("Created!");

        eventHandler->onKeyPress.connect<&ExamplePlugin::onKeyPressed>(this);
        eventHandler->onImGuiRender.connect<&ExamplePlugin::imguiRender>(this);

        hry::KeyBind doSomethingBind;
        doSomethingBind.setConfigFieldName("do_something");
        doSomethingBind.setName("Do something");
        doSomethingBind.setDefaultKey(hry::Keyboard::Key::Q);
        doSomethingBind.setPressAction<&ExamplePlugin::onKeyBind>(this);
        keyBinds->addKeyBind(std::move(doSomethingBind));
    }

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

    virtual void imguiPage() { ImGui::Text("Test asdf"); }

    virtual const hry::PluginInfo& getPluginInfo() const { return _pluginInfo; }

private:
    void onKeyPressed(const hry::KeyboardEvent& key)
    {
        Logger->info("Key pressed!", static_cast<int>(key.key));
    }

    void onKeyBind() { Logger->info("Key bind works!"); }
};

INIT_PLUGIN(ExamplePlugin)
INIT_IMGUI()
