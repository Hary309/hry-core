#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "Hry/Events/Event.hpp"
#include "Hry/KeyBinding/BindableKeys.hpp"
#include "Hry/KeyBinding/KeyBinds.hpp"
#include "Hry/Namespace.hpp"
#include "Hry/System/System.hpp"
#include "Hry/Utils/Delegate.hpp"

#include "Events/EventManager.hpp"

HRY_NS_BEGIN

class KeyBindsManager
{
private:
    std::vector<KeyBinds*> _keyBinds;

    Sink<void(const KeyboardEvent&&)> _onKeyPress;
    Sink<void(const KeyboardEvent&&)> _onKeyRelease;
    Sink<void(const MouseButtonEvent&&)> _onMouseButtonPress;
    Sink<void(const MouseButtonEvent&&)> _onMouseButtonRelease;

public:
    KeyBindsManager(EventManager& eventMgr);

    KeyBindsUniquePtr_t createKeyBinds(const std::string& name);
    void remove(const KeyBinds* keyBind);

    const auto& getKeyBinds() const { return _keyBinds; }

private:
    void keyBindsDeleter(KeyBinds* ptr);

    void handleKeybaordEvent(const KeyboardEvent&& keyboardEvent);
    void handleMouseButtonEvent(const MouseButtonEvent&& buttonEvent);

    void processKey(const BindableKey::Key_t key, ButtonState buttonState);
};

HRY_NS_END