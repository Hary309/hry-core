#include "EventManager.hpp"

#include "WndProcEventBridge.hpp"

namespace hry::events
{


EventManager::EventManager() 
{

}

EventManager::~EventManager() 
{

}

void EventManager::init() 
{
    _eventBridges.push_back(std::make_unique<WndProcEventBridge>(*this));
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
    if (!_events.empty())
    {
        _events.pop();
    }
}

}