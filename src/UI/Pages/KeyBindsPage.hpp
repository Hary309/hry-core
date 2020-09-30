/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include "Hry/Events/Event.hpp"
#include "Hry/Namespace.hpp"
#include "Hry/Utils/Signal.hpp"

#include "KeyBinding/KeyBindsManager.hpp"

#include "PageBase.hpp"

HRY_NS_BEGIN

class KeyBindsPage : public PageBase
{
private:
    KeyBindsManager& _keyBindsMgr;

    KeyBind* _keyToSetBind = nullptr;

public:
    KeyBindsPage(KeyBindsManager& keyBindsMgr, InternalEventHandler& eventHandler);

    void imguiRender() override;

private:
    void handleKeyPress(const KeyboardEvent&&);
    void handleMouseButtonPress(const MouseButtonEvent&&);
    void handleJoystickButtonPress(const JoystickButtonEvent&&);

    void applyChanges();
};

HRY_NS_END
