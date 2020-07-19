#include "Hry/KeyBinding/KeyBinds.hpp"

namespace hry
{

KeyBinds::KeyBinds(const std::string& name)
    : _name(name)
{

}

void KeyBinds::addBind(const char* name, KeyBind::Key_t defaultKey, const KeyBind::Delegate_t& action) 
{
    _keyBinds.push_back(KeyBind {std::string(name), defaultKey, defaultKey, action });
}

}