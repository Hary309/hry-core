#pragma once

#include <optional>
#include <variant>
#include <cstdint>

#include "Hry/Math/Vec2.hpp"
#include "Hry/System/System.hpp"
#include "Hry/System/Keyboard.hpp"
#include "Hry/System/Mouse.hpp"

namespace hry
{

enum class ButtonState
{
    Pressed,
    Released
};

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

}
