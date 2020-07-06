#include "EventManager.hpp"

namespace hry::events
{


EventManager::EventManager() 
{

}

EventManager::~EventManager() 
{

}

Event* EventManager::front() 
{
    if (!_events.empty())
    {  
        return &_events.front();
    }

    return nullptr;
}

void EventManager::pop() 
{
    if (_events.empty())
    {
        _events.pop();
    }
}

}