#pragma once

#include "Hry/Namespace.hpp"

HRY_NS_BEGIN

class EventManager;

class EventBridgeBase
{
protected:
    EventManager& _eventMgr;

public:
    explicit EventBridgeBase(EventManager& eventMgr) : _eventMgr(eventMgr) {}
};

HRY_NS_END
