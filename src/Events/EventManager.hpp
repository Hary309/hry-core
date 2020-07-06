#pragma once

#include <queue>
#include <optional>

#include "Events/Event.hpp"

namespace hry::events
{

class EventManager
{
private:
    std::queue<Event> _events;

public:
    EventManager();
    ~EventManager();

    void pushEvent(Event&& event)
    {
        _events.push(std::move(event));
    }

    Event* front();
    void pop();
};

}
