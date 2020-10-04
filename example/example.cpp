#include "example.hpp"

#include <Hry/Config/Fields/BoolField.hpp>
#include <Hry/Config/Fields/NumericField.hpp>
#include <Hry/Config/Fields/SelectionField.hpp>
#include <Hry/Config/Fields/TextField.hpp>

struct ConfigData
{
    bool renderWindow;
    float windowOpacity;
};

ExamplePlugin::ExamplePlugin()
{
    _pluginInfo.name = "hry-example";
    _pluginInfo.fullName = "Example Plugin";
    _pluginInfo.authorInfo = { "Foo Bar", "foo@bar.com" };
    _pluginInfo.website = "http://example.com";
    _pluginInfo.version = hry::Version{ 1, 0, 0 };
    _pluginInfo.desc = R"(
# About
Example plugin or sth
# Changelog
  * 1.0
    * First release
)";
}

ExamplePlugin::Result ExamplePlugin::init(const InitParams&& initParams)
{
    Logger = initParams.logger;
    Logger->info("Plugin initialized!");

    return ExamplePlugin::Result::Ok;
}

void ExamplePlugin::initConfig(hry::Config* config)
{
    config->setBindingType<ConfigData>();
    config->onChangesApplied.connect<&ExamplePlugin::onConfigChangesApplied>(this);

    config->add(hry::BoolFieldBuilder()
                    .setID("render_window")
                    .setLabel("Render window")
                    .setDefaultValue(true)
                    .bind(&ConfigData::renderWindow)
                    .build());

    config->add(hry::NumericFieldBuilder<float>()
                    .setID("window_opacity")
                    .setLabel("Window opacity")
                    .setDefaultValue(50.f)
                    .useDrag(0.001f, 0.f, 1.f)
                    .bind(&ConfigData::windowOpacity)
                    .build());
}

void ExamplePlugin::initKeyBinds(hry::KeyBinds* keyBinds)
{
    keyBinds->add(hry::KeyBindBuilder()
                      .setID("change_value")
                      .setLabel("Change value")
                      .setPressCallback(hry::Dlg<&ExamplePlugin::keyBindPress>(this))
                      .setReleaseCallback(hry::Dlg<&ExamplePlugin::keyBindPress>(this))
                      .setDefaultKey(hry::Keyboard::Key::Q)
                      .build());
}

void ExamplePlugin::initEvents(hry::EventDispatcher* eventDispatcher)
{
    eventDispatcher->system.onImGuiRender.connect<&ExamplePlugin::imguiRender>(this);
}

const hry::PluginInfo& ExamplePlugin::getPluginInfo() const
{
    return _pluginInfo;
}

void ExamplePlugin::onConfigChangesApplied(const hry::ConfigCallbackData& data)
{
    const auto& configData = data.getData<ConfigData>();
    _renderWindow = configData->renderWindow;
    _windowOpacity = configData->windowOpacity;
}

void ExamplePlugin::imguiRender()
{
    if (!_renderWindow)
    {
        return;
    }

    ImGui::SetNextWindowBgAlpha(_windowOpacity);

    if (ImGui::Begin("hry-example window"))
    {
        ImGui::Text("Is keybind pressed? %s", _isKeyBindPressed ? "true" : "false");
    }

    ImGui::End();
}

void ExamplePlugin::keyBindPress(hry::ButtonState state)
{
    _isKeyBindPressed = state == hry::ButtonState::Pressed;
}

INIT_PLUGIN(ExamplePlugin)
