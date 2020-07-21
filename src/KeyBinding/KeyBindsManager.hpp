#pragma once

#include <vector>
#include <memory>
#include <functional>

#include "Hry/Events/Event.hpp"
#include "Hry/KeyBinding/BindableKeys.hpp"
#include "Hry/KeyBinding/KeyBinds.hpp"

#include "Events/EventManager.hpp"
#include "Hry/System/System.hpp"

namespace hry
{

class KeyBindsManager
{
private:
    std::vector<std::unique_ptr<KeyBinds>> _keyBinds;

    Sink<void(const KeyboardEvent&&)> _onKeyPress;
    Sink<void(const KeyboardEvent&&)> _onKeyRelease;
    Sink<void(const MouseButtonEvent&&)> _onMouseButtonPress;
    Sink<void(const MouseButtonEvent&&)> _onMouseButtonRelease;

public:
    KeyBindsManager(EventManager& eventMgr);

    KeyBinds* createKeyBinds(const std::string& name);
    void remove(const KeyBinds* keyBind);

    const auto& getKeyBinds() const { return _keyBinds; }

private:
    void handleKeybaordEvent(const KeyboardEvent&& keyboardEvent);
    void handleMouseButtonEvent(const MouseButtonEvent&& buttonEvent);

    void processKey(const BindableKey::Key_t key, ButtonState buttonState);
};

}