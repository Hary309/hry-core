#include "KeyBindsManager.hpp"

#include <cstdio>

#include "Core.hpp"
#include "Hry/KeyBinding/KeyBinds.hpp"
#include "Hry/System/System.hpp"
#include "Hry/Utils/Delegate.hpp"

namespace hry
{
KeyBindsManager::KeyBindsManager(EventManager& eventMgr)
    : _onKeyPress(eventMgr.keyPressSignal), _onKeyRelease(eventMgr.keyReleaseSignal),
      _onMouseButtonPress(eventMgr.mouseButtonPressSignal),
      _onMouseButtonRelease(eventMgr.mouseButtonReleaseSignal)
{
    _onKeyPress.connect<&KeyBindsManager::handleKeybaordEvent>(this);
    _onKeyRelease.connect<&KeyBindsManager::handleKeybaordEvent>(this);
    _onMouseButtonPress.connect<&KeyBindsManager::handleMouseButtonEvent>(this);
    _onMouseButtonRelease.connect<&KeyBindsManager::handleMouseButtonEvent>(this);
}

KeyBindsUniquePtr_t KeyBindsManager::createKeyBinds(const std::string& name)
{
    auto keyBinds = new KeyBinds(name);
    _keyBinds.push_back(keyBinds);

    // use custom deleter to remove from list when KeyBinds is removing
    return { keyBinds, { ConnectArg_v<&KeyBindsManager::keyBindsDeleter>, this } };
}

void KeyBindsManager::remove(const KeyBinds* keyBinds)
{
    _keyBinds.erase(std::remove(_keyBinds.begin(), _keyBinds.end(), keyBinds));
}

void KeyBindsManager::keyBindsDeleter(KeyBinds* ptr)
{
    remove(ptr);
    delete ptr;
}

void KeyBindsManager::handleKeybaordEvent(const KeyboardEvent&& keyboardEvent)
{
    processKey(keyboardEvent.key, keyboardEvent.state);
}

void KeyBindsManager::handleMouseButtonEvent(const MouseButtonEvent&& buttonEvent)
{
    processKey(buttonEvent.button, buttonEvent.state);
}

void KeyBindsManager::processKey(const BindableKey::Key_t key, ButtonState buttonState)
{
    for (auto& keyBindsSection : _keyBinds)
    {
        auto& keyBinds = keyBindsSection->getKeyBinds();

        for (auto& keyBind : keyBinds)
        {
            if (keyBind.getKey() != nullptr && keyBind.getKeyState() != buttonState &&
                keyBind.getKey()->key == key)
            {
                switch (buttonState)
                {
                    case ButtonState::Pressed: keyBind.callPressAction(); break;
                    case ButtonState::Released: keyBind.callReleaseAction(); break;
                }
                keyBind.setKeyState(buttonState);
            }
        }
    }
}

} // namespace hry
