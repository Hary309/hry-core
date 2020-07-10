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

struct Event
{
    enum class Type
    {
        WindowResized = 0, // struct: ResizeEvent
        WindowGainedFocus, // no struct
        WindowLostFocus, // no struct

        KeyPressed, // struct: KeyboardEvent
        KeyReleased, // struct: KeyboardEvent

        MouseButtonPressed, // struct: MouseButtonEvent
        MouseButtonReleased, // struct: MouseButtonEvent
        MouseMoved, // struct: MouseMoveEvent
        MouseWheelScrolled, // struct: MouseWheelEvent
    };

    std::variant<ResizeEvent, KeyboardEvent, MouseButtonEvent, MouseMoveEvent, MouseWheelEvent> event;
    Type type;
 
    template<typename T>
    bool has()
    {
        return std::holds_alternative<T>(event);
    }

    template<typename T>
    T& get()
    {
        return std::get<T>(event);
    }
};

}
