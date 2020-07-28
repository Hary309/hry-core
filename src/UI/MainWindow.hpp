#pragma once

#include "Hry/Events/Event.hpp"
#include "Hry/KeyBinding/KeyBinds.hpp"
#include "Hry/Namespace.hpp"
#include "Hry/Utils/Signal.hpp"

#include "Events/EventManager.hpp"
#include "KeyBinding/KeyBindsManager.hpp"
#include "Modules/ModuleManager.hpp"
#include "UI/Pages//KeyBindsPage.hpp"

HRY_NS_BEGIN

class MainWindow
{
private:
    bool _isWindowEnabled = false;

    ModuleManager& _moduleMgr;

    int _selectedPluginIndex = 0;

    KeyBindsPage _keyBindsPage;

public:
    MainWindow(ModuleManager& moduleMgr, KeyBindsManager& keyBindsMgr, EventManager& eventMgr);

    void initKeyBinds(KeyBinds& keyBinds);

    void renderImGui();

private:
    void renderPluginsTab();
    void renderSettingsTab();
    void renderPluginsPageTab();
    void renderAboutTab();

    void showMainWindowKeyBind();
};

HRY_NS_END
