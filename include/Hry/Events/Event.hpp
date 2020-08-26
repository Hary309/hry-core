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

HRY_NS_END
