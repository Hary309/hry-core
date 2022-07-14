/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#include "KeyBindsManager.hpp"

#include <chrono>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <ios>
#include <ratio>

#include <nlohmann/json.hpp>

#include "Hry/KeyBinding/KeyBinds.hpp"
#include "Hry/System/System.hpp"
#include "Hry/Utils/Delegate.hpp"

#include "Core.hpp"

using system_clock = std::chrono::system_clock;

namespace hry
{
KeyBindsManager::KeyBindsManager(InternalEventDispatcher& dispatcher)
{
    dispatcher.system.onKeyPress.connect<&KeyBindsManager::handleKeyboardEvent>(this);
    dispatcher.system.onKeyRelease.connect<&KeyBindsManager::handleKeyboardEvent>(this);

    dispatcher.system.onMouseButtonPress.connect<&KeyBindsManager::handleMouseButtonEvent>(this);
    dispatcher.system.onMouseButtonRelease.connect<&KeyBindsManager::handleMouseButtonEvent>(
        this);
    dispatcher.system.onJoystickButtonPress.connect<&KeyBindsManager::handleJoystickButtonEvent>(
        this);

    dispatcher.system.onJoystickButtonRelease
        .connect<&KeyBindsManager::handleJoystickButtonEvent>(this);

    dispatcher.game.onFrameEnd.connect<&KeyBindsManager::update>(this);
}

HryPtr<KeyBinds> KeyBindsManager::createKeyBinds(const std::string& name)
{
    auto* keyBinds = new KeyBinds(name);
    _keyBinds.push_back(keyBinds);

    // use custom deleter to remove from list when KeyBinds is removing
    return { keyBinds, Dlg<&KeyBindsManager::keyBindsDeleter>(this) };
}

void KeyBindsManager::remove(const KeyBinds* keyBinds)
{
    _keyBinds.erase(std::remove(_keyBinds.begin(), _keyBinds.end(), keyBinds));
}

void KeyBindsManager::update(const FrameEndEvent&& /*unused*/)
{
    _taskScheduler.update();
}

void KeyBindsManager::keyBindsDeleter(KeyBinds* ptr)
{
    remove(ptr);
    delete ptr;
}

void KeyBindsManager::handleKeyboardEvent(const KeyboardEvent&& keyboardEvent)
{
    processKey(keyboardEvent.key, keyboardEvent.state, {});
}

void KeyBindsManager::handleMouseButtonEvent(const MouseButtonEvent&& buttonEvent)
{
    processKey(buttonEvent.button, buttonEvent.state, {});
}

void KeyBindsManager::handleJoystickButtonEvent(const JoystickButtonEvent&& buttonEvent)
{
    processKey(buttonEvent.button, buttonEvent.state, buttonEvent.deviceGUID);
}

void KeyBindsManager::processKey(
    BindableKey::Key_t key, ButtonState buttonState, std::optional<GUID> guid)
{
    for (auto& keyBindsSection : _keyBinds)
    {
        auto& keyBinds = keyBindsSection->getKeyBinds();

        for (auto& keyBind : keyBinds)
        {
            if (keyBind->key == nullptr || keyBind->state == buttonState ||
                keyBind->key->key != key || keyBind->joystickGUID != guid)
            {
                continue;
            }

            switch (keyBind->activator)
            {
                case KeyBind::Activator::Click:
                    handleClickActivator(buttonState, keyBind.get());
                    break;
                case KeyBind::Activator::Hold:
                    handleHoldActivator(buttonState, keyBind.get());
                    break;
            }

            keyBind->state = buttonState;
        }
    }
}

void KeyBindsManager::handleClickActivator(ButtonState buttonState, KeyBind* keyBind)
{
    switch (buttonState)
    {
        case ButtonState::Pressed: keyBind->pressAction.call(ButtonState::Pressed); break;
        case ButtonState::Released: keyBind->releaseAction.call(ButtonState::Released); break;
    }
}

void KeyBindsManager::handleHoldActivator(ButtonState buttonState, KeyBind* keyBind)
{
    switch (buttonState)
    {
        case ButtonState::Pressed:
        {
            auto endTimePoint = system_clock::now() + LongPressTimeout;

            _taskScheduler.addTask(
                LongPressTimeout, { ConnectArg_v<&KeyBindsManager::onTaskHold>, this }, keyBind,
                endTimePoint);

            keyBind->keyPressTimePoint = endTimePoint;
        }
        break;
        case ButtonState::Released:
        {
            if (keyBind->keyPressTimePoint - system_clock::now() <
                system_clock::time_point::duration::zero())
            {
                keyBind->releaseAction(ButtonState::Released);
            }
        }
        break;
    }
}

void KeyBindsManager::onTaskHold(KeyBind* keyBind, system_clock::time_point timePoint)
{
    if (keyBind->state == ButtonState::Pressed && keyBind->keyPressTimePoint == timePoint)
    {
        keyBind->pressAction(ButtonState::Pressed);
    }
}

void KeyBindsManager::saveAll() const
{
    for (const auto& k : _keyBinds)
    {
        k->saveToFile();
    }
}
}
