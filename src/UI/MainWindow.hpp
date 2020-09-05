#pragma once

#include "Hry/Events/Event.hpp"
#include "Hry/Events/EventHandler.hpp"
#include "Hry/KeyBinding/KeyBinds.hpp"
#include "Hry/Namespace.hpp"
#include "Hry/System/System.hpp"
#include "Hry/Utils/Signal.hpp"

#include "KeyBinding/KeyBindsManager.hpp"
#include "Modules/ModuleManager.hpp"
#include "UI/Pages/AboutPage.hpp"
#include "UI/Pages/ConfigPage.hpp"
#include "UI/Pages/KeyBindsPage.hpp"
#include "UI/Pages/PluginsPage.hpp"

HRY_NS_BEGIN

class MainWindow
{
private:
    bool _isWindowEnabled = false;

    PluginsPage _pluginsPage;
    ConfigPage _configPage;
    KeyBindsPage _keyBindsPage;
    AboutPage _aboutPage;

    PageBase* _currentPage{ &_pluginsPage };

public:
    MainWindow(
        ModuleManager& moduleMgr,
        ConfigManager& configMgr,
        KeyBindsManager& keyBindsMgr,
        EventHandler& eventHandler);

    void initKeyBinds(KeyBinds& keyBinds);

    void imguiRender();

private:
    void showMainWindowKeyBind(hry::ButtonState);
};

HRY_NS_END
