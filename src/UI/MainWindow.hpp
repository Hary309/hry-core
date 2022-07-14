/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include "Events/EventManager.hpp"
#include "KeyBinding/KeyBindsManager.hpp"
#include "Modules/ModuleManager.hpp"
#include "UI/Pages/AboutPage.hpp"
#include "UI/Pages/ConfigPage.hpp"
#include "UI/Pages/KeyBindsPage.hpp"
#include "UI/Pages/PluginsPage.hpp"

#include "Hry/Events/Event.hpp"
#include "Hry/Events/EventDispatcher.hpp"
#include "Hry/KeyBinding/KeyBinds.hpp"
#include "Hry/System/System.hpp"
#include "Hry/Utils/Signal.hpp"

namespace hry
{
class MainWindow
{
private:
    bool _isWindowEnabled = false;

    PluginsPage _pluginsPage;
    ConfigPage _configPage;
    KeyBindsPage _keyBindsPage;
    AboutPage _aboutPage;

    PageBase* _currentPage{ &_pluginsPage };

    EventManager& _eventMgr;

    KeyBind* _showMainWindowKeyBind{};

public:
    MainWindow(
        ModuleManager& moduleMgr,
        ConfigManager& configMgr,
        KeyBindsManager& keyBindsMgr,
        EventManager& eventMgr,
        InternalEventDispatcher& eventDispatcher);

    void initKeyBinds(KeyBinds& keyBinds);

    void imguiRender();

private:
    void showMainWindowKeyBind(hry::ButtonState);
};
}
