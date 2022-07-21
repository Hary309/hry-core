/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include "PageBase.hpp"

#include "KeyBinding/KeyBindsManager.hpp"

#include "Hry/Events/Event.hpp"
#include "Hry/Utils/Signal.hpp"
#include "Hry/Utils/TaskScheduler.hpp"

namespace hry
{
class KeyBindsPage : public PageBase
{
private:
    KeyBindsManager& _keyBindsMgr;

    KeyBind* _keyToSetBind = nullptr;

    TaskScheduler<void()> _taskScheduler;
    std::optional<JoystickButtonEvent> _deviceToBind;

public:
    KeyBindsPage(KeyBindsManager& keyBindsMgr, InternalEventDispatcher& dispatcher);

    void imguiRender() override;

private:
    void handleKeyPress(const KeyboardEvent&&);
    void handleMouseButtonPress(const MouseButtonEvent&&);
    void handleJoystickButtonPress(const JoystickButtonEvent&&);

    void applyChanges();

    void onBindDInput();
};
}
