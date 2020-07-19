#pragma once

#include <vector>
#include <variant>
#include <string>

#include "Hry/Export.hpp"
#include "Hry/System//Keyboard.hpp"
#include "Hry/System//Mouse.hpp"
#include "Hry/Utils/Delegate.hpp"

namespace hry
{

struct KeyBind
{
    using Key_t = std::variant<Keyboard::Key, Mouse::Button>; // TODO: Add controller's keys
    using Delegate_t = Delegate<void()>;

    std::string name;
    Key_t key;
    Key_t defaultKey;
    const Delegate_t& action;
};

class KeyBinds
{
private:
    std::string _name;
    std::vector<KeyBind> _keyBinds;

public:
    KeyBinds(const std::string& name);

    HRY_API void addBind(const char* name, const KeyBind::Key_t defaultKey, const KeyBind::Delegate_t& action);
    const std::string& getName() const { return _name; }

    const auto& getKeyBinds() const { return _keyBinds; }
}; 

}
