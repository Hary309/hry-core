#pragma once

#include "Hry/Events/Event.hpp"
#include "Hry/KeyBinding/KeyBinds.hpp"
#include "Hry/Utils/Signal.hpp"

#include "Events/EventManager.hpp"
#include "KeyBinding/KeyBindsManager.hpp"
#include "Modules/ModuleManager.hpp"

namespace hry
{
class MainWindow
{
private:
    bool _isWindowEnabled = false;

    ModuleManager& _moduleMgr;
    KeyBindsManager& _keyBindsMgr;

    Sink<void(const KeyboardEvent&&)> _onKeyPress;
    Sink<void(const MouseButtonEvent&&)> _onMouseButtonPress;

    int _selectedPluginIndex = 0;

    KeyBind* _keyToSetBind = nullptr;

public:
    MainWindow(ModuleManager& moduleMgr, KeyBindsManager& KeyBindsMgr, EventManager& eventMgr);

    void initKeyBinds(KeyBinds& keyBinds);

    void renderImGui();

private:
    void renderPluginsTab();
    void renderSettingsTab();
    void renderPluginsPageTab();
    void renderKeyBindsTab();
    void renderAboutTab();

    void renderModal();

    void showMainWindowKeyBind();

    void handleKeyPress(const KeyboardEvent&&);
    void handleMouseButtonPress(const MouseButtonEvent&&);
};

} // namespace hry
