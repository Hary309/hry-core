#pragma once

#include <queue>
#include <vector>
#include <memory>

#include "Hry/Events/Event.hpp"

#include "Events/EventBridgeBase.hpp"


namespace hry::events
{

class EventBridgeBase;

class EventManager
{
private:
    std::queue<Event> _events;

    std::vector<std::unique_ptr<EventBridgeBase>> _eventBridges;

public:
    EventManager() = default;
    ~EventManager() = default;

    void init();

    void pushEvent(Event&& event)
    {
        _events.push(std::move(event));
    }

    Event* front();
    void pop();
};

}
