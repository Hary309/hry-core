#pragma once

#include "Hry/Events/Event.hpp"
#include "Hry/Events/EventHandler.hpp"
#include "Hry/KeyBinding/KeyBinds.hpp"
#include "Hry/Namespace.hpp"
#include "Hry/System/System.hpp"
#include "Hry/Utils/Signal.hpp"

#include "KeyBinding/KeyBindsManager.hpp"
#include "Modules/ModuleManager.hpp"
#include "UI/Pages/ConfigPage.hpp"
#include "UI/Pages/KeyBindsPage.hpp"

HRY_NS_BEGIN

class MainWindow
{
private:
    bool _isWindowEnabled = false;

    ModuleManager& _moduleMgr;

    int _selectedPluginIndex = 0;

    ConfigPage _configPage;
    KeyBindsPage _keyBindsPage;

public:
    MainWindow(
        ModuleManager& moduleMgr,
        ConfigManager& configMgr,
        KeyBindsManager& keyBindsMgr,
        EventHandler& eventHandler);

    void initKeyBinds(KeyBinds& keyBinds);

    void imguiRender();

private:
    void renderPluginsTab();
    void renderPluginsPageTab();
    void renderAboutTab();

    void showMainWindowKeyBind(hry::ButtonState);
};

HRY_NS_END
