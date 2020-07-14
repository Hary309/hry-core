#pragma once

#include <optional>
#include <variant>
#include <cstdint>

#include "../System/System.hpp"
#include "../System/Keyboard.hpp"
#include "../System/Mouse.hpp"

namespace hry::events
{

struct ResizeEvent
{
    system::ResizeType type;
    uint32_t width;
    uint32_t height;
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
    int offsetX;
    int offsetY;
};

struct MouseWheelEvent
{
    system::Mouse::Wheel wheel;
    short delta;
};

}
