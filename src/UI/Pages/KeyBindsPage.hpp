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

    KeyBind* _keyToSetBind = nullptr;

public:
    KeyBindsPage(KeyBindsManager& keyBindsMgr, EventHandler& eventHandler);

    void renderImGuiPage();

private:
    void handleKeyPress(const KeyboardEvent&&);
    void handleMouseButtonPress(const MouseButtonEvent&&);
    void handleJoystickButtonPress(const JoystickButtonEvent&&);

    void applyChanges();
};

HRY_NS_END
