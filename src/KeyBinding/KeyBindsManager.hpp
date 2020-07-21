#pragma once

#include <vector>
#include <memory>
#include <functional>

#include "Hry/Events/Event.hpp"
#include "Hry/KeyBinding/KeyBinds.hpp"

#include "Events/EventManager.hpp"

namespace hry
{

class KeyBindsManager
{
private:
    std::vector<std::unique_ptr<KeyBinds>> _keyBinds;

    Sink<void(const KeyboardEvent&&)> _onKeyPress;
    Sink<void(const MouseButtonEvent&&)> _onMouseButtonPress;

public:
    KeyBindsManager(EventManager& eventMgr);

    KeyBinds* createKeyBinds(const std::string& name);
    void remove(const KeyBinds* keyBind);

    const auto& getKeyBinds() const { return _keyBinds; }

private:
    void handleKeyPress(const KeyboardEvent&& keyboardEvent);
    void handleMouseButtonPress(const MouseButtonEvent&& buttonEvent);

    void iterateKeyBinds(std::function<bool(KeyBind&)> callback);
};

}