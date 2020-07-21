#include "Hry/KeyBinding/KeyBinds.hpp"

namespace hry
{

KeyBinds::KeyBinds(const std::string& name)
    : _name(name)
{

}

void KeyBind::setDefaultKey(const BindableKey::Key_t key) 
{
    auto it = std::find_if(BindableKeys.begin(), BindableKeys.end(),
        [&key](const BindableKey& bindableKey)
            { return key == bindableKey.key; }
    );

    if (it != BindableKeys.end())
    {
        setDefaultKey(&*it);
    }
}

void KeyBind::setKey(const BindableKey::Key_t key) 
{
    auto it = std::find_if(BindableKeys.begin(), BindableKeys.end(),
        [&key](const BindableKey& bindableKey)
            { return key == bindableKey.key; }
    );

    if (it != BindableKeys.end())
    {
        setKey(&*it);
    }
}

}