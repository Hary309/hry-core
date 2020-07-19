#pragma once

#include <vector>
#include <memory>

#include "Hry/Events/Event.hpp"
#include "Hry/KeyBinding/KeyBinds.hpp"

#include "Events/EventManager.hpp"

namespace hry::key_binding
{

class KeyBindsManager
{
private:
    std::vector<std::unique_ptr<KeyBinds>> _keyBinds;

    utils::Sink<void(const events::KeyboardEvent&&)> _onKeyPress;
    utils::Sink<void(const events::MouseButtonEvent&&)> _onMouseButtonPress;

public:
    KeyBindsManager(events::EventManager& eventMgr);

    KeyBinds* createKeyBinds(const std::string& name);
    void remove(const KeyBinds* keyBind);

    const auto& getKeyBinds() const { return _keyBinds; }
};

}