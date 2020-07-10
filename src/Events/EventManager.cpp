#include "EventManager.hpp"

#include "Core.hpp"

#include "WndProcEventBridge.hpp"
#include "DInput8EventBridge.hpp"

namespace hry::events
{


void EventManager::init() 
{
    Core::Logger->info("Initializing EventManager...");

    _eventBridges.push_back(std::make_unique<WndProcEventBridge>(*this));
    _eventBridges.push_back(std::make_unique<DInput8EventBridge>(*this));
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