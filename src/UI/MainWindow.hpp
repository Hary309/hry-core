#pragma once

#include "Hry/Events/Event.hpp"
#include "Hry/KeyBinding/KeyBinds.hpp"
#include "Hry/Utils/Signal.hpp"

#include "Events/EventManager.hpp"
#include "Modules/ModuleManager.hpp"
#include "KeyBinding/KeyBindsManager.hpp"


namespace hry
{

class MainWindow
{
private:
    ModuleManager& _moduleMgr;
    KeyBindsManager& _keyBindsMgr;
    
    int _selectedPluginIndex = 0;

    bool _isEnabled = false;

public:
    MainWindow(
        ModuleManager& moduleMgr,
        KeyBindsManager& KeyBindsMgr);

    void setupKeyBinds(KeyBinds& keyBinds);

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
