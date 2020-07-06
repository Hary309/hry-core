#pragma once

namespace hry::events
{

class EventManager;

class EventBridgeBase
{
protected:
    EventManager& _eventMgr;

public:
    EventBridgeBase(EventManager& eventMgr) : _eventMgr(eventMgr) {}
};

}