#pragma once

#include "Hry/Events/Event.hpp"
#include "Hry/KeyBinding/KeyBinds.hpp"
#include "Hry/Utils/Signal.hpp"

#include "Events/EventManager.hpp"
#include "Modules/ModuleManager.hpp"
#include "KeyBinding/KeyBindsManager.hpp"


namespace hry::ui
{

class MainWindow
{
private:
    modules::ModuleManager& _moduleMgr;
    key_binding::KeyBindsManager& _keyBindsMgr;
    
    int _selectedPluginIndex = 0;

    bool _isEnabled = false;

public:
    MainWindow(
        modules::ModuleManager& moduleMgr,
        key_binding::KeyBindsManager& KeyBindsMgr);

    void setupKeyBinds(key_binding::KeyBinds& keyBinds);

    void renderImGui();

private:
    void renderPluginsTab();
    void renderSettingsTab();
    void renderPluginsSettingsTab();
    void renderBindsTab();
    void renderAboutTab();

    void showMainWindowKeyBind();
};

}
