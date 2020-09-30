/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include <cstdint>
#include <optional>
#include <variant>

#include <guiddef.h>

#include "Hry/Math/Vec2.hpp"
#include "Hry/Namespace.hpp"
#include "Hry/System/Joystick.hpp"
#include "Hry/System/Keyboard.hpp"
#include "Hry/System/Mouse.hpp"
#include "Hry/System/System.hpp"

HRY_NS_BEGIN

struct ResizeEvent
{
    ResizeType type;
    Vec2u size;
};

struct KeyboardEvent
{
    Keyboard::Key key;
    ButtonState state;
};

struct MouseButtonEvent
{
    Mouse::Button button;
    ButtonState state;
};

struct MouseMoveEvent
{
    Vec2i offset;
};

struct MouseWheelEvent
{
    Mouse::Wheel wheel;
    short delta;
};

struct GameStateEvent
{
    enum class Type
    {
        Paused,
        Resumed
    } type;
};

struct JoystickMoveEvent
{
    GUID deviceGUID;
    Joystick::Axis axis;
    double value; // range [-100 .. 100]
};

struct JoystickButtonEvent
{
    GUID deviceGUID;
    Joystick::Button button;
    ButtonState state;
};

struct OverlayStateEvent
{
    bool isEnabled;
};

struct FrameStartEvent
{
    double deltaTime;
    bool isGamePaused;
};

struct FrameEndEvent
{
    double deltaTime;
};

HRY_NS_END
