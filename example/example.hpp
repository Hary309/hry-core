#pragma once

#include <Hry/Plugin.hpp>

class ExamplePlugin : public hry::Plugin
{
public:
    inline static hry::Logger* Logger;

private:
    hry::PluginInfo _pluginInfo;

    bool _isKeyBindPressed{ false };

    bool _renderWindow{ false };
    float _windowOpacity{ 0.5f };

public:
    ExamplePlugin();

    Result init(const InitParams&& initParams) override;
    void initConfig(hry::Config* config) override;
    void initKeyBinds(hry::KeyBinds* keyBinds) override;
    void initEvents(hry::EventDispatcher* eventDispatcher) override;

    // TODO: replace with manifest.json
    [[nodiscard]] const hry::PluginInfo& getPluginInfo() const override;

private:
    void onConfigChangesApplied(const hry::ConfigCallbackData&);
    void imguiRender();
    void keyBindPress(hry::ButtonState);
};
