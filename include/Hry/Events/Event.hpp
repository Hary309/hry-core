#pragma once

#include <optional>
#include <variant>
#include <cstdint>

#include "Hry/Math/Vec2.hpp"
#include "Hry/System/System.hpp"
#include "Hry/System/Keyboard.hpp"
#include "Hry/System/Mouse.hpp"

namespace hry::events
{

struct ResizeEvent
{
    system::ResizeType type;
    math::Vec2u size;
};

struct KeyboardEvent
{
    system::Keyboard::Key key;
};

struct MouseButtonEvent
{
    system::Mouse::Button button;
};

struct MouseMoveEvent
{
    math::Vec2i offset;
};

struct MouseWheelEvent
{
    system::Mouse::Wheel wheel;
    short delta;
};

}
