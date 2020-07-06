#pragma once

#include <queue>
#include <optional>
#include <vector>

#include "Events/Event.hpp"
#include "Events/EventBridgeBase.hpp"

namespace hry::events
{

class EventBridgeBase;

class EventManager
{
private:
    std::queue<Event> _events;

    std::vector<EventBridgeBase> _eventBridges;

public:
    EventManager();
    ~EventManager();

    void init();

    void pushEvent(Event&& event)
    {
        _events.push(std::move(event));
    }

    Event* front();
    void pop();
};

}
