#pragma once
#include <variant>

#include "Events/KeyboardEvent.hpp"

namespace hry
{

class Event
{
private:
    std::variant<events::KeyboardEvent> _event;
};

}
