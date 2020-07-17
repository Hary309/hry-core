#pragma once

#include "Events/EventManager.hpp"
#include "Hry/Events/Event.hpp"
#include "Hry/Utils/Signal.hpp"
#include "Modules/ModuleManager.hpp"

namespace hry::ui
{

class MainWindow
{
private:
    modules::ModuleManager& _moduleMgr;
    utils::Sink<void(const events::KeyboardEvent&&)> _onKeyPress;

    int _selectedPluginIndex = 0;

    bool _isEnabled = false;

public:
    MainWindow(modules::ModuleManager& moduleMgr, events::EventManager& eventMgr);

    void renderImGui();

private:
    void renderPluginsTab();
    void renderSettingsTab();
    void renderPluginsSettingsTab();
    void renderAboutTab();

    void onKeyPress(const events::KeyboardEvent&& keyboard);
};

}
