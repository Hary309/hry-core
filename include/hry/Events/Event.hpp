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
    int x;
    int y;
};

struct MouseWheelEvent
{
    system::Mouse::Wheel wheel;
    short delta;
    int x;
    int y;
};

struct Event
{
    enum class Type
    {
        WindowResized = 0, // struct: ResizeEvent

        KeyPressed, // struct: KeyboardEvent
        KeyReleased, // struct: KeyboardEvent

        MouseButtonPressed, // struct: MouseButtonEvent
        MouseButtonReleased, // struct: MouseButtonEvent
        MouseWheel, // struct: MouseWheelEvent
    };

    std::variant<ResizeEvent, KeyboardEvent, MouseButtonEvent, MouseWheelEvent> event;
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
