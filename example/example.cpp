#include <memory>
#include <windows.h>
#include <iostream>

#include <imgui.h>

#include <Hry/Plugin.hpp>
#include <Hry/Events/Event.hpp>
#include <Hry/Logger/ModuleLogger.hpp>
#include <Hry/System/Keyboard.hpp>

using namespace hry;

class ExamplePlugin : public Plugin
{
private:
    PluginInfo _pluginInfo = {
            "Example plugin",
            "hry-example",
            "This is a example of plugin based on hry-core.",
            "Example implementation of plugin",
            1000,
    };

public:
    inline static std::unique_ptr<logger::ModuleLogger> Logger;

public:
    virtual ~ExamplePlugin()
    {
        Logger->info("Unloading...");
    }

    virtual void init(std::unique_ptr<logger::ModuleLogger>&& logger)
    {
        Logger = std::move(logger);
        Logger->info("Created!");

        eventHandler->onKeyPress.connect<&ExamplePlugin::onKeyPressed>(this);
        eventHandler->onImGuiRender.connect<&ExamplePlugin::imguiRender>(this);
    }

    virtual void update(float deltaTime)
    {

    }

    virtual void imguiRender()
    {
        static float ads;
        if (ImGui::Begin("hry-example"))
        {
            ImGui::Text("asdf");
            ImGui::InputFloat("float", &ads);
        }

        ImGui::End();
    }

    virtual void imguiSettingsTab()
    {
        ImGui::Text("Test asdf");
    }

    virtual const PluginInfo& getPluginInfo() const
    {
        return _pluginInfo;
    }

private:
    void onKeyPressed(const events::KeyboardEvent& key)
    {
        Logger->info("Key pressed!", static_cast<int>(key.key));
    }
};

INIT_PLUGIN(ExamplePlugin)
INIT_IMGUI()
