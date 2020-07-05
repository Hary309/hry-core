#pragma once

#include <vector>

#include "Events/Event.hpp"

namespace hry::events
{

class EventManager
{
private:
    std::vector<Event> _events;

public:
    EventManager();
    ~EventManager();
};

}
