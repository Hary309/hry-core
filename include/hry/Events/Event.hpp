#pragma once
#include <variant>

#include "KeyboardEvent.hpp"

namespace hry::events
{

class Event
{
public:
    enum class Type
    {
        Keyboard
    };

private:
    std::variant<KeyboardEvent> _event;

public:
    Type getType() { return static_cast<Type>(_event.index()); }

    template<typename T>
    T get()
    {
        return std::get<T>(_event);
    }
};

}
