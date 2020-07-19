#include "KeyBindsManager.hpp"
#include "Hry/KeyBinding/KeyBinds.hpp"
#include <cstdio>

namespace hry::key_binding
{

KeyBindsManager::KeyBindsManager(events::EventManager& eventMgr)
    : 
    _onKeyPress(eventMgr.keyPressSignal),
    _onMouseButtonPress(eventMgr.mouseButtonPressSignal)
{
    
}

KeyBinds* KeyBindsManager::createKeyBinds(const std::string& name) 
{
    auto keyBinds = new KeyBinds(name);
    _keyBinds.push_back(std::unique_ptr<KeyBinds>(keyBinds));
    return keyBinds;
}

void KeyBindsManager::remove(const KeyBinds* keyBinds) 
{
    _keyBinds.erase(std::remove_if(_keyBinds.begin(), _keyBinds.end(), 
            [keyBinds](const std::unique_ptr<KeyBinds>& a) { 
                return a.get() == keyBinds;
            }
        ));
}

}