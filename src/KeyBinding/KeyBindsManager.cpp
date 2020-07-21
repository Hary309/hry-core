#include "KeyBindsManager.hpp"
#include "Hry/KeyBinding/KeyBinds.hpp"
#include <cstdio>

namespace hry
{

KeyBindsManager::KeyBindsManager(EventManager& eventMgr)
    : 
    _onKeyPress(eventMgr.keyPressSignal),
    _onMouseButtonPress(eventMgr.mouseButtonPressSignal)
{
    _onKeyPress.connect<&KeyBindsManager::handleKeyPress>(this);
    _onMouseButtonPress.connect<&KeyBindsManager::handleMouseButtonPress>(this);
    // TODO: add release key
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

void KeyBindsManager::handleKeyPress(const KeyboardEvent&& keyboardEvent) 
{
    iterateKeyBinds([&keyboardEvent](KeyBind& keyBind) {
        auto& key = keyBind.getKey();

        if (keyBind.isKeyPressState() &&
            key->has<Keyboard::Key>() &&
            std::get<Keyboard::Key>(key->key) == keyboardEvent.key)
        {
            keyBind.callPressAction();
            keyBind.setKeyPressState(true);
            return true;
        }

        return false;
    });
}

void KeyBindsManager::handleMouseButtonPress(const MouseButtonEvent&& buttonEvent) 
{
    iterateKeyBinds([&buttonEvent](KeyBind& keyBind) {
        auto& key = keyBind.getKey();

        if (keyBind.isKeyPressState() &&
            key->has<Mouse::Button>() &&
            std::get<Mouse::Button>(key->key) == buttonEvent.button)
        {
            keyBind.callPressAction();
            keyBind.setKeyPressState(true);
            return true;
        }

        return false;
    });
}

void KeyBindsManager::iterateKeyBinds(std::function<bool(KeyBind&)> callback) 
{
    for (auto& keyBindsSection : _keyBinds)
    {
        auto& keyBinds = keyBindsSection->getKeyBinds();

        for (auto& keyBind : keyBinds)
        {
            if (callback(keyBind))
            {
                return;
            }
        }
    }
}


}