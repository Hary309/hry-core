#pragma once

#include "Hry/Events/Event.hpp"
#include "Hry/Namespace.hpp"
#include "Hry/Utils/Signal.hpp"

#include "KeyBinding/KeyBindsManager.hpp"

HRY_NS_BEGIN

class KeyBindsPage
{
private:
    KeyBindsManager& _keyBindsMgr;

    Sink<void(const KeyboardEvent&&)> _onKeyPress;
    Sink<void(const MouseButtonEvent&&)> _onMouseButtonPress;

    KeyBind* _keyToSetBind = nullptr;

public:
    KeyBindsPage(KeyBindsManager& keyBindsMgr, EventManager& eventMgr);

    void drawImGuiPage();

private:
    void handleKeyPress(const KeyboardEvent&&);
    void handleMouseButtonPress(const MouseButtonEvent&&);
};

HRY_NS_END
