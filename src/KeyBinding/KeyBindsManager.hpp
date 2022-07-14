/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include "Events/EventManager.hpp"

#include "Hry/Events/Event.hpp"
#include "Hry/Events/EventDispatcher.hpp"
#include "Hry/KeyBinding/BindableKeys.hpp"
#include "Hry/KeyBinding/KeyBinds.hpp"
#include "Hry/System/System.hpp"
#include "Hry/Utils/Delegate.hpp"
#include "Hry/Utils/TaskScheduler.hpp"
#include "Hry/Utils/Utils.hpp"

#include <chrono>
#include <functional>
#include <memory>
#include <vector>

namespace hry
{
class KeyBindsManager
{
public:
    inline static constexpr auto LongPressTimeout = std::chrono::milliseconds(400);

private:
    std::vector<KeyBinds*> _keyBinds;
    TaskScheduler<void(KeyBind*, std::chrono::system_clock::time_point)> _taskScheduler;

public:
    explicit KeyBindsManager(InternalEventDispatcher& dispatcher);

    HryPtr<KeyBinds> createKeyBinds(const std::string& name);
    void remove(const KeyBinds* keyBind);

    void update(const FrameEndEvent&&);

    [[nodiscard]] const auto& getKeyBinds() const { return _keyBinds; }

    void saveAll() const;

private:
    void keyBindsDeleter(KeyBinds* ptr);

    void handleKeyboardEvent(const KeyboardEvent&& keyboardEvent);
    void handleMouseButtonEvent(const MouseButtonEvent&& buttonEvent);
    void handleJoystickButtonEvent(const JoystickButtonEvent&& buttonEvent);

    void processKey(BindableKey::Key_t key, ButtonState buttonState, std::optional<GUID> guid);

    static void handleClickActivator(ButtonState buttonState, KeyBind* keyBind);
    void handleHoldActivator(ButtonState buttonState, KeyBind* keyBind);

    void onTaskHold(KeyBind* keyBind, std::chrono::system_clock::time_point);
};
}